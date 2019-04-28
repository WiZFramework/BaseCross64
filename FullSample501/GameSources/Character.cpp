/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//�R���X�^���g�o�b�t�@�̎��̕�
	IMPLEMENT_DX11_CONSTANT_BUFFER(ConstantBufferCalcbody)

	//CS�̎��̕�
	IMPLEMENT_DX11_COMPUTE_SHADER(ComputeSaderCalcbody, App::GetApp()->GetShadersPath() + L"CSCalcbody.cso")

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MoveBox::MoveBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position),
		m_TotalTime(0),
		m_Velocity(0)
	{
	}
	MoveBox::~MoveBox() {}

	//������
	void MoveBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//OBB�Փ�j�����t����
		auto ptrColl = AddComponent<CollisionObb>();
		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();
		//�e������
		auto shadowPtr = AddComponent<Shadowmap>();
		shadowPtr->SetMeshResource(L"DEFAULT_CUBE");
		//�`�揈��
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"WALL_TX");
		ptrDraw->SetOwnShadowActive(true);

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�G�������g�o�b�t�@
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(Element);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffer_desc.StructureByteStride = sizeof(Element);
		ThrowIfFailed(
			pDx11Device->CreateBuffer(&buffer_desc, nullptr, &m_Buffer),
			L"�G�������g�o�b�t�@�쐬�Ɏ��s���܂���",
			L"pDx11Device->CreateBuffer()",
			L"MoveBox::OnCreate()"
		);
		//�A�N�Z�X�r���[
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavbuffer_desc = {};
		uavbuffer_desc.Format = DXGI_FORMAT_UNKNOWN;
		uavbuffer_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavbuffer_desc.Buffer.NumElements = 1;
		ThrowIfFailed(
			pDx11Device->CreateUnorderedAccessView(m_Buffer.Get(), &uavbuffer_desc, &m_UAV),
			L"�A�N�Z�X�r���[�쐬�Ɏ��s���܂���",
			L"pDx11Device->CreateUnorderedAccessView()",
			L"MoveBox::OnCreate()"
		);
		//���[�h�o�b�N�o�b�t�@
		D3D11_BUFFER_DESC readback_buffer_desc = {};
		readback_buffer_desc.ByteWidth = sizeof(Element);
		readback_buffer_desc.Usage = D3D11_USAGE_STAGING;
		readback_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		readback_buffer_desc.StructureByteStride = sizeof(Element);
		ThrowIfFailed(
			pDx11Device->CreateBuffer(&readback_buffer_desc, nullptr, &m_ReadBackBuffer),
			L"���[�h�o�b�N�o�b�t�@�쐬�Ɏ��s���܂���",
			L"pDx11Device->CreateBuffer()",
			L"MoveBox::OnCreate()"
		);
	}

	void MoveBox::OnUpdate() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += elapsedTime;
		if (m_TotalTime >= XM_2PI) {
			m_TotalTime = 0;
		}
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�R���X�^���g�o�b�t�@�̐ݒ�
		CB cb = {};
		cb.paramf[0] = m_TotalTime;
		ID3D11Buffer* pConstantBuffer = ConstantBufferCalcbody::GetPtr()->GetBuffer();
		pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);
		pID3D11DeviceContext->CSSetConstantBuffers(0, 1, &pConstantBuffer);
		//���݂̈ʒu���̎擾
		auto ptrTransform = GetComponent<Transform>();
		//�G�������g�̓���
		Element elemData;
		Vec4 pos4(ptrTransform->GetPosition(), 0);
		elemData.pos = pos4;
		pID3D11DeviceContext->UpdateSubresource(m_Buffer.Get(), 0, nullptr, &elemData, 0, 0);
		//CS�̐ݒ�
		pID3D11DeviceContext->CSSetShader(ComputeSaderCalcbody::GetPtr()->GetShader(), nullptr, 0);
		//�A�N�Z�X�r���[�̐ݒ�
		pID3D11DeviceContext->CSSetUnorderedAccessViews(0, 1, m_UAV.GetAddressOf(), nullptr);
		//CS�̎��s
		pID3D11DeviceContext->Dispatch(1, 1, 1);
		//���ʂ̓ǂݎ��
		D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };
		pID3D11DeviceContext->CopyResource(m_ReadBackBuffer.Get(), m_Buffer.Get());
		if (SUCCEEDED(pID3D11DeviceContext->Map(m_ReadBackBuffer.Get(), 0, D3D11_MAP_READ, 0, &MappedResource)))
		{
			memcpy(&elemData, MappedResource.pData, sizeof(Element));
			pID3D11DeviceContext->Unmap(m_ReadBackBuffer.Get(), 0);
			Vec3 resuPos;
			resuPos.x = elemData.pos.x;
			resuPos.y = elemData.pos.y;
			resuPos.z = elemData.pos.z;
			ptrTransform->SetPosition(resuPos);
		}
	}


	//--------------------------------------------------------------------------------------
	//�@�^�C�����O����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	TilingFixedBox::TilingFixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position),
		m_UPic(UPic),
		m_VPic(VPic)
	{}
	TilingFixedBox::~TilingFixedBox() {}
	//������
	void TilingFixedBox::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rotation);
		PtrTrans->SetPosition(m_Position);
		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.z / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				float FrontBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, 1, 0));
				float BackBetween = bsm::angleBetweenNormals(vertices[i].normal, Vec3(0, -1, 0));
				if (FrontBetween < 0.01f || BackBetween < 0.01f) {
					vertices[i].textureCoordinate.y = VCount;
				}
			}
		}
		//�`��R���|�[�l���g�̒ǉ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`��R���|�[�l���g�Ɍ`��i���b�V���j��ݒ�
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//�����ɉe���f�肱�ނ悤�ɂ���
		PtrDraw->SetOwnShadowActive(true);
		//�`��R���|�[�l���g�e�N�X�`���̐ݒ�
		PtrDraw->SetTextureResource(L"SKY_TX");
		//�^�C�����O�ݒ�
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);

	}

}
//end basecross
