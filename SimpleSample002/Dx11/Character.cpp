/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�O�p�`�X�v���C�g
	//--------------------------------------------------------------------------------------
		TriangleSprite::TriangleSprite() :
			ObjectInterface(),
			ShapeInterface(),
			m_Pos(0, 0, 0),
			m_MoveSpan(0.01f, 0, 0)
		{
		}
		TriangleSprite::~TriangleSprite() {}

		void TriangleSprite::OnCreate() {
			//���_���쐬���邽�߂̔z��
			vector<VertexPositionColor> vertices = {
				{ VertexPositionColor(Vec3(0.0f, 0.5f, 0.0f), Col4(1.0f,0.0f,0.0f,1.0f)) },
				{ VertexPositionColor(Vec3(0.5f, -0.5f, 0.0f), Col4(0.0f, 1.0f, 0.0f, 1.0f)) },
				{ VertexPositionColor(Vec3(-0.5f, -0.5f, 0.0f), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			};
			m_TriangleMesh = MeshResource::CreateMeshResource(vertices, false);
		}

		void TriangleSprite::OnUpdate() {
			m_Pos += m_MoveSpan;
			if (abs(m_Pos.x) >= 0.5f) {
				m_MoveSpan *= -1.0f;
			}
		}

		void TriangleSprite::OnDraw() {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();

			//�R���X�^���g�o�b�t�@�̏���
			SpriteConstantBuffer sb;
			sb.Emissive = Col4(0.0f, 0.0f, 0, 1.0f);
			Mat4x4 mat;
			mat.translation(m_Pos);
			sb.World = mat;
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = sizeof(VertexPositionColor);
			UINT offset = 0;
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_TriangleMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//�`����@�i3�p�`�j
			pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�V�F�[�_�̐ݒ�
			pD3D11DeviceContext->VSSetShader(VSPCSprite::GetPtr()->GetShader(), nullptr, 0);
			pD3D11DeviceContext->PSSetShader(PSPCSprite::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(VSPCSprite::GetPtr()->GetInputLayout());

			//�u�����h�X�e�[�g
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
			//�f�v�X�X�e���V���X�e�[�g
			pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
			//���X�^���C�U�X�e�[�g
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

			//�`��
			pD3D11DeviceContext->Draw(m_TriangleMesh->GetNumVertices(), 0);
			//��n��
			Dev->InitializeStates();

		}


}
//end basecross
