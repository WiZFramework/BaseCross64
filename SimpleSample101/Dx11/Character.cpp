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
	CubeObject::CubeObject(const wstring& TextureFileName, bool Trace, const Vec3& Pos, bool Flat) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(Pos),
		m_Flat(Flat)
	{}
	CubeObject::~CubeObject() {}

	void CubeObject::CreateBuffers() {
		float HelfSize = 0.5f;
		vector<Vec3> PosVec = {
			{ Vec3(-HelfSize, HelfSize, -HelfSize) },
			{ Vec3(HelfSize, HelfSize, -HelfSize) },
			{ Vec3(-HelfSize, -HelfSize, -HelfSize) },
			{ Vec3(HelfSize, -HelfSize, -HelfSize) },
			{ Vec3(HelfSize, HelfSize, HelfSize) },
			{ Vec3(-HelfSize, HelfSize, HelfSize) },
			{ Vec3(HelfSize, -HelfSize, HelfSize) },
			{ Vec3(-HelfSize, -HelfSize, HelfSize) },
		};
		vector<UINT> PosIndeces = {
			0, 1, 2, 3,
			1, 4, 3, 6,
			4, 5, 6, 7,
			5, 0, 7, 2,
			5, 4, 0, 1,
			2, 3, 7, 6,
		};


		vector<Vec3> FaceNormalVec = {
			{ Vec3(0, 0, -1.0f) },
			{ Vec3(1.0f, 0, 0) },
			{ Vec3(0, 0, 1.0f) },
			{ Vec3(-1.0f, 0, 0) },
			{ Vec3(0, 1.0f, 0) },
			{ Vec3(0, -1.0f, 0) }
		};

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		UINT BasePosCount = 0;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				VertexPositionNormalTexture Data;
				Data.position = PosVec[PosIndeces[BasePosCount + j]];
				if (m_Flat) {
					//�t���b�g�\���̏ꍇ�͖@���͒��_�����ɂ���
					Data.normal = Data.position;
					Data.normal.normalize();
				}
				else {
					//�t���b�g�\�����Ȃ��ꍇ�́A�@���͖ʂ̌���
					Data.normal = FaceNormalVec[i];
				}
				switch (j) {
				case 0:
					Data.textureCoordinate = Vec2(0, 0);
					break;
				case 1:
					Data.textureCoordinate = Vec2(1.0f, 0);
					break;
				case 2:
					Data.textureCoordinate = Vec2(0, 1.0f);
					break;
				case 3:
					Data.textureCoordinate = Vec2(1.0f, 1.0f);
					break;
				}
				vertices.push_back(Data);
			}

			indices.push_back((uint16_t)BasePosCount + 0);
			indices.push_back((uint16_t)BasePosCount + 1);
			indices.push_back((uint16_t)BasePosCount + 2);
			indices.push_back((uint16_t)BasePosCount + 1);
			indices.push_back((uint16_t)BasePosCount + 3);
			indices.push_back((uint16_t)BasePosCount + 2);

			BasePosCount += 4;
		}

		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_CubeMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}


	void CubeObject::OnCreate() {
		CreateBuffers();
		//�e�N�X�`���̍쐬
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		m_Scale = Vec3(1.0f, 1.0f, 1.0f);
		m_Qt.identity();
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
		PNTStaticConstantBuffer sb;
		sb.World = World;
		sb.View = View;
		sb.Projection = Proj;
		//���C�e�B���O
		Vec4 LightDir(0.5f, -1.0f, 0.5f, 0.0f);
		LightDir.normalize();
		sb.LightDir = LightDir;
		//�f�B�t���[�Y
		sb.Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		//�G�~�b�V�u���Z�B
		sb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_CubeMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(m_CubeMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//���_�V�F�[�_�ɓn��
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�s�N�Z���V�F�[�_�ɓn��
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());

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
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);

		//�e�N�X�`���ƃT���v���[�̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureResource->GetShaderResourceView().GetAddressOf());
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		if (m_Trace) {
			//���������̏ꍇ�́A���X�^���C�U�X�e�[�g��ύX����2��`��
			//���X�^���C�U�X�e�[�g�i���ʕ`��j
			pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
			//�`��
			pD3D11DeviceContext->DrawIndexed(m_CubeMesh->GetNumIndicis(), 0, 0);
			//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
			//�`��
			pD3D11DeviceContext->DrawIndexed(m_CubeMesh->GetNumIndicis(), 0, 0);
		}
		else {
			//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
			//�`��
			pD3D11DeviceContext->DrawIndexed(m_CubeMesh->GetNumIndicis(), 0, 0);
		}
		//��n��
		Dev->InitializeStates();
	}





}
//end basecross
