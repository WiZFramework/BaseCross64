/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
@copyright shike.y
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	�l�p�`�X�v���C�g����
	//--------------------------------------------------------------------------------------
	SquareSprite::SquareSprite(const wstring& TextureFileName, bool Trace, const Vec2& StartPos) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(StartPos)
	{}
	SquareSprite::~SquareSprite() {}

	void SquareSprite::OnCreate() {
		float HelfSize = 0.5f;
		
		//���_�z��
		vector<VertexPositionTexture> vertices = {
			{ VertexPositionTexture(Vec3(-HelfSize, HelfSize, 0), Vec2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Vec3(HelfSize, HelfSize, 0), Vec2(1.0f, 0.0f)) },
			{ VertexPositionTexture(Vec3(-HelfSize, -HelfSize, 0), Vec2(0.0f, 1.0f)) },
			{ VertexPositionTexture(Vec3(HelfSize, -HelfSize, 0), Vec2(1.0f, 1.0f)) },
		};
		
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		
		//���b�V���̍쐬
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//�e�N�X�`���̍쐬
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");

		//��`�̑傫��
		m_Scale = Vec2(128.0f, 128.0f);

		//��]�p
		m_Rot = 0;

		m_PosSpan = Vec2(3.0f, 0);
	}

	void SquareSprite::OnUpdate() {
		if (m_PosSpan.x > 0) {
			m_Rot -= 0.05f;
		}
		else {
			m_Rot += 0.05f;
		}
		m_Pos += m_PosSpan;
		if (abs(m_Pos.x) > 400.0f) {
			m_PosSpan *= -1.0f;
		}
	}

	void SquareSprite::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//�s��̒�`
		Mat4x4 World, Proj;

		//���[���h�s��̌���
		World.affineTransformation2D(
			m_Scale,			//�X�P�[�����O
			Vec2(0, 0),		//��]�̒��S�i�d�S�j
			m_Rot,				//��]�p�x
			m_Pos				//�ʒu
		);

		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj = XMMatrixOrthographicLH(w, h, -1.0, 1.0f);

		//�s��̍���
		World *= Proj;

		//�R���X�^���g�o�b�t�@�̏���
		DiffuseSpriteConstantBuffer sb;

		//�G�~�b�V�u���Z�͍s��Ȃ��B
		sb.Emissive = Col4(0, 0, 0, 0);

		//�f�t�B�[�Y�͂��ׂĒʂ�
		sb.Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);

		//�s��̐ݒ�
		sb.World = World;

		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBDiffuseSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionTexture);
		UINT offset = 0;

		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_SquareMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);

		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(m_SquareMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBDiffuseSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;

		//���_�V�F�[�_�ɓn��
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);

		//�s�N�Z���V�F�[�_�ɓn��
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPTSprite::GetPtr()->GetShader(), nullptr, 0);

		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPTSprite::GetPtr()->GetInputLayout());

		//�u�����h�X�e�[�g
		if (m_Trace) {
			//��������
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		else {
			//�����������Ȃ�
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		}

		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);

		//���X�^���C�U�X�e�[�g
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//�e�N�X�`���ƃT���v���[�̐ݒ�
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureResource->GetShaderResourceView().GetAddressOf());
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		//�`��
		pD3D11DeviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);

		//��n��
		Dev->InitializeStates();

		
	}
	
}
//end basecross
