/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�����̎���
	//--------------------------------------------------------------------------------------
	CubeObject::CubeObject() :
		ObjectInterface(),
		ShapeInterface()
	{}
	CubeObject::~CubeObject() {}

	void CubeObject::CreateBuffers() {
		float HelfSize = 0.5f;
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(Vec3(-HelfSize, HelfSize, -HelfSize), Col4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, HelfSize, -HelfSize), Col4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, -HelfSize, -HelfSize), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, -HelfSize, -HelfSize), Col4(1.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, HelfSize, HelfSize), Col4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, HelfSize, HelfSize), Col4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, -HelfSize, HelfSize), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, -HelfSize, HelfSize), Col4(1.0f, 0.0f, 1.0f, 1.0f)) }
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
			1, 4, 3,
			4, 6, 3,
			4, 5, 6,
			5, 7, 6,
			5, 0, 7,
			0, 2, 7,
			5, 4, 0,
			4, 1, 0,
			6, 7, 3,
			7, 2, 3
		};
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_CubeMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}


	void CubeObject::OnCreate() {
		CreateBuffers();
		m_Scale = Vec3(1.0f, 1.0f, 1.0f);
		m_Qt.identity();
		m_Pos = Vec3(0, 0, 0.0);
	}
	void CubeObject::OnUpdate() {
		Quat QtSpan;
		QtSpan.rotation(0.02f, Vec3(0, 1.0f, 0));
		m_Qt *= QtSpan;
		m_Qt.normalize();
	}
	void CubeObject::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//�s��̒�`
		Mat4x4 World, View, Proj;
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�]�u����
		World.transpose();
		//�r���[�s��̌���
		View = XMMatrixLookAtLH(Vec3(0, 2.0, -5.0f), Vec3(0, 0, 0), Vec3(0, 1.0f, 0));
		//�]�u����
		View.transpose();
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//�]�u����
		Proj.transpose();
		//�R���X�^���g�o�b�t�@�̏���
		PCStaticConstantBuffer sb;
		sb.World = World;
		sb.View = View;
		sb.Projection = Proj;
		//�G�~�b�V�u���Z�͍s��Ȃ��B
		sb.Emissive = Col4(0, 0, 0, 0);
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBPCStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_CubeMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(m_CubeMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBPCStatic::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//���_�V�F�[�_�ɓn��
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�s�N�Z���V�F�[�_�ɓn��
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPCStatic::GetPtr()->GetInputLayout());
		//�u�����h�X�e�[�g
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//���X�^���C�U�X�e�[�g
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//�`��
		pD3D11DeviceContext->DrawIndexed(m_CubeMesh->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();
	}


}
//end basecross
