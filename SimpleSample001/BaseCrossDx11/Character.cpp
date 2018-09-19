/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	�O�p�`�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	TriangleObject::TriangleObject() :
		ObjectInterface(),
		ShapeInterface()
	{
	}
	TriangleObject::~TriangleObject() {}

	void TriangleObject::OnCreate() {
		//���_���쐬���邽�߂̔z��
		vector<VertexPositionColor> vertices = {
			{ Vec3(0.0f, 0.5f , 0.0f),Col4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vec3(0.5f, -0.5f, 0.5f),Col4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vec3(-0.5f, -0.5f, 0.5f),Col4(0.0f, 0.0f, 1.0f, 1.0f) }
		};
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices, false);
	}

	void TriangleObject::OnUpdate() {
		//�������Ȃ�
	}

	void TriangleObject::OnDraw() {
		//�`��
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�^�X�̃|�C���^
		auto RenderStatePtr = Dev->GetRenderState();

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_TriangleMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//null�e�N�X�`����ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		pID3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
		//null�T���v���[��ݒ�
		pID3D11DeviceContext->PSSetSamplers(0, 1, pNullSR);
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
		//�V�F�[�_�̐ݒ�
		//���_�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSPCDirect::GetPtr()->GetShader(), nullptr, 0);
		//�s�N�Z���V�F�[�_�̐ݒ�
		pID3D11DeviceContext->PSSetShader(PSPCDirect::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSPCDirect::GetPtr()->GetInputLayout());

		//�h��Ԃ�
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//�`��
		pID3D11DeviceContext->Draw(3, 0);
		//��n��
		Dev->InitializeStates();

	}



}
//end basecross
