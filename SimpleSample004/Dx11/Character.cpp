/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
@copyright shike.y
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross 
{
	//--------------------------------------------------------------------------------------
	//	�l�p�`�X�v���C�g�̃O���[�v����
	//--------------------------------------------------------------------------------------
	SquareSpriteGroup::SquareSpriteGroup() :
		ObjectInterface(),
		ShapeInterface(),
		m_TotalTime(0)
	{}

	SquareSpriteGroup::~SquareSpriteGroup() {}
	
	void SquareSpriteGroup::OnCreate() {

		float HelfSize = 0.5f;

		//���_�z��
		m_BackupVertices = {
			{ VertexPositionColor(Vec3(-HelfSize, HelfSize, 0), Col4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, HelfSize, 0), Col4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, -HelfSize, 0), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 0.0f, 1.0f, 1.0f)) },
		};

		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };

		//���b�V���̍쐬
		//���_�ύX�ł���
		m_SquareSpriteMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);

		//�O���[�v�̔z��̍쐬
		m_SquareSpriteVec.assign(100, SquareSprite());

		for (auto& v : m_SquareSpriteVec) {
			v.m_LocalRot = Util::RandZeroToOne(true);
			v.m_LocalRotVelocity = Util::RandZeroToOne(true) * 20.0f - 10.0f;
			v.m_LocalPos = Vec2(0, 0);
			v.m_LocalVelocity = Vec2(Util::RandZeroToOne(true) * 200.0f - 100.0f, 100 + Util::RandZeroToOne(true) * 100.0f);
			v.m_LocalGravityVelocity = Vec2(0, 0);
		}
	}

	//�e�I�u�W�F�N�g�̈ʒu���̕ύX
	void SquareSpriteGroup::UpdateObjects(float ElapsedTime) {
		
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		
		h /= 2.0f;
		
		for (auto& v : m_SquareSpriteVec) {
			
			if (v.m_LocalPos.y < -h) {
				v.m_LocalRot = Util::RandZeroToOne(true);
				v.m_LocalRotVelocity = Util::RandZeroToOne(true) * 20.0f - 10.0f;
				v.m_LocalPos = Vec2(0, 0);
				v.m_LocalVelocity = Vec2(Util::RandZeroToOne(true) * 200.0f - 100.0f, 100 + Util::RandZeroToOne(true) * 100.0f);
				v.m_LocalGravityVelocity = Vec2(0, 0);
			}
			else {
				v.m_LocalRot += v.m_LocalRotVelocity * ElapsedTime;
				v.m_LocalPos += v.m_LocalVelocity * ElapsedTime;
				v.m_LocalGravityVelocity += Vec2(0, -98.0f) * ElapsedTime;
				v.m_LocalPos += v.m_LocalGravityVelocity * ElapsedTime;
			}

		}
	}

	void SquareSpriteGroup::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateObjects(ElapsedTime);
	}

	void SquareSpriteGroup::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();


		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;

		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_SquareSpriteMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);

		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(m_SquareSpriteMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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

		//�O���[�v���܂Ƃ߂ĕ`��
		for (auto& v : m_SquareSpriteVec) {
			//�s��̒�`
			Mat4x4 World, Proj;

			//���[���h�s��̌���
			World.affineTransformation2D(
				v.m_LocalScale,			//�X�P�[�����O
				Vec2(0, 0),		//��]�̒��S�i�d�S�j
				v.m_LocalRot,				//��]�p�x
				v.m_LocalPos				//�ʒu
			);

			//�ˉe�s��̌���
			float w = static_cast<float>(App::GetApp()->GetGameWidth());

			float h = static_cast<float>(App::GetApp()->GetGameHeight());

			Proj = XMMatrixOrthographicLH(w, h, -1.0, 1.0f);

			//�s��̍���
			World *= Proj;

			//�R���X�^���g�o�b�t�@�̏���
			SpriteConstantBuffer sb;

			//�G�~�b�V�u���Z�͍s��Ȃ��B
			sb.Emissive = Col4(0, 0, 0, 0);

			//�s��̐ݒ�
			sb.World = World;

			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;

			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

			//�`��
			pD3D11DeviceContext->DrawIndexed(m_SquareSpriteMesh->GetNumIndicis(), 0, 0);

		}
		//��n��
		Dev->InitializeStates();

	}


}
//end basecross
