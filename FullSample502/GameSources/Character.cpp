/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	IMPLEMENT_DX11_COMPUTE_SHADER(ComputeSaderCalcbody, App::GetApp()->GetShadersPath() + L"CSCalcbody.cso")

	IMPLEMENT_DX11_CONSTANT_BUFFER(ConstantBufferCalcbody)


	//--------------------------------------------------------------------------------------
	//　点で構成された球体
	//-----------------------------------------------ix---------------------------------------
	PointsBall::PointsBall(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Position(Position)
	{}

	PointsBall::~PointsBall() {}
	//初期化
	void PointsBall::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		m_LocalDataVec.resize(NUM_ELEMENTS);
		//球体を作成
		MeshUtill::CreateSphere(1.0f, 28, vertices, indices);
		m_NumVertices = (UINT)vertices.size();
		for (size_t i = 0; i < m_NumVertices; i++) {
			LocalData tempLocalData;
			tempLocalData.m_LocalPosition = vertices[i].position;
			m_LocalDataVec[i] = tempLocalData;
		}
		m_ResultDataVec.resize(m_NumVertices);
		//描画用メッシュの作成
		float helfSize = 0.04f;
		Col4 col(1.0f, 1.0f, 0.0f, 1.0f);
		//頂点配列
		vector<VertexPositionColorTexture> meshVertices = {
			{ VertexPositionColorTexture(Vec3(-helfSize, helfSize, 0),col, Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, helfSize, 0),col, Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-helfSize, -helfSize, 0),col, Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(helfSize, -helfSize, 0),col, Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> meshIndex = { 0, 1, 2, 1, 3, 2 };
		//2次元平面とする（頂点数が少ないため）
		m_MeshRes = MeshResource::CreateMeshResource(meshVertices, meshIndex, true);
		//全体の位置関連
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(Vec3(m_Scale));
		ptrTransform->SetRotation(Vec3(0));
		ptrTransform->SetPosition(m_Position);
		//描画コンポーネントの追加（インスタンス描画）
		auto PtrDraw = AddComponent<PCTStaticInstanceDraw>();
		PtrDraw->SetMeshResource(m_MeshRes);
		PtrDraw->SetTextureResource(L"SPARK_TX");
		PtrDraw->SetDepthStencilState(DepthStencilState::Read);
		//各頂点ごとに行列を作成
		for (size_t i = 0; i < m_NumVertices; i++) {
			Mat4x4 tempMat;
			tempMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				m_LocalDataVec[i].m_LocalPosition
			);
			//インスタンス描画の行列として設定
			PtrDraw->AddMatrix(tempMat);
		}
		SetAlphaActive(true);

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();

		//エレメンツバッファ
		D3D11_BUFFER_DESC buffer_desc = { 0 };
		buffer_desc.ByteWidth = NUM_ELEMENTS * sizeof(LocalData);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
		buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		buffer_desc.StructureByteStride = sizeof(LocalData);
		ThrowIfFailed(
			pDx11Device->CreateBuffer(&buffer_desc, nullptr, &m_InBuffer),
			L"m_InBuffer作成に失敗しました",
			L"pDx11Device->CreateBuffer()",
			L"PointsBall::OnCreate()"
		);
		ThrowIfFailed(
			pDx11Device->CreateBuffer(&buffer_desc, nullptr, &m_OutBuffer),
			L"m_OutBuffer作成に失敗しました",
			L"pDx11Device->CreateBuffer()",
			L"PointsBall::OnCreate()"
		);
		//シェーダーリソースビュー
		D3D11_SHADER_RESOURCE_VIEW_DESC srvbuffer_desc = {};
		srvbuffer_desc.Format = DXGI_FORMAT_UNKNOWN;
		srvbuffer_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srvbuffer_desc.Buffer.FirstElement = 0;
		srvbuffer_desc.Buffer.NumElements = buffer_desc.ByteWidth / buffer_desc.StructureByteStride;
		srvbuffer_desc.Buffer.ElementWidth = NUM_ELEMENTS;
		ThrowIfFailed(
			pDx11Device->CreateShaderResourceView(m_InBuffer.Get(), &srvbuffer_desc, &m_SRV),
			L"m_SRV作成に失敗しました",
			L"pDx11Device->CreateShaderResourceView()",
			L"PointsBall::OnCreate()"
		);
		//アクセスビュー
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavbuffer_desc = {};
		uavbuffer_desc.Format = DXGI_FORMAT_UNKNOWN;
		uavbuffer_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uavbuffer_desc.Buffer.NumElements = NUM_ELEMENTS;
		ThrowIfFailed(
			pDx11Device->CreateUnorderedAccessView(m_OutBuffer.Get(), &uavbuffer_desc, &m_UAV),
			L"m_UAV作成に失敗しました",
			L"pDx11Device->CreateUnorderedAccessView()",
			L"PointsBall::OnCreate()"
		);
		//リードバックバッファ
		D3D11_BUFFER_DESC readback_buffer_desc = {};
		readback_buffer_desc.ByteWidth = NUM_ELEMENTS * sizeof(LocalData);
		readback_buffer_desc.Usage = D3D11_USAGE_STAGING;
		readback_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		readback_buffer_desc.StructureByteStride = sizeof(LocalData);
		ThrowIfFailed(
			pDx11Device->CreateBuffer(&readback_buffer_desc, nullptr, &m_ReadBackBuffer),
			L"m_ReadBackBuffer作成に失敗しました",
			L"pDx11Device->CreateBuffer()",
			L"PointsBall::OnCreate()"
		);
	}


	void PointsBall::OnUpdate() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		auto ptrDraw = GetComponent<PCTStaticInstanceDraw>();
		auto ptrTransform = GetComponent<Transform>();
		//全体を回転させる
		auto worldQt = ptrTransform->GetQuaternion();
		Quat spanQt(Vec3(0, 1, 0), elapsedTime);
		worldQt *= spanQt;
		ptrTransform->SetQuaternion(worldQt);
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		CB cb = {};
		auto playerSh = GetStage()->GetSharedGameObject<Player>(L"Player");
		cb.m_PlayerPosition = playerSh->GetComponent<Transform>()->GetWorldPosition();
		cb.m_PlayerPosition.w = 1.0f;
		cb.m_WorldMatrix = ptrTransform->GetWorldMatrix();
		// paramf[0] == elapsedTime
		// paramf[1] == velocityPower
		// paramf[2] == gravity_y
		// paramf[3] == baseY
		cb.paramf[0] = elapsedTime;
		cb.paramf[1] = 3.0f;
		cb.paramf[2] = -9.8f;
		cb.paramf[3] = m_Scale * 0.02f;
		ID3D11Buffer* pConstantBuffer = ConstantBufferCalcbody::GetPtr()->GetBuffer();
		pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cb, 0, 0);
		pID3D11DeviceContext->CSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->CSSetShader(ComputeSaderCalcbody::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->UpdateSubresource(m_InBuffer.Get(), 0, nullptr, &m_LocalDataVec[0], 0, 0);
		pID3D11DeviceContext->CSSetShaderResources(0, 1, m_SRV.GetAddressOf());
		pID3D11DeviceContext->CSSetUnorderedAccessViews(0, 1, m_UAV.GetAddressOf(), nullptr);
		pID3D11DeviceContext->Dispatch((UINT)m_NumVertices, 1, 1);

		//カメラのレイを作成しておく
		auto camera = OnGetDrawCamera();
		auto lay = camera->GetAt() - camera->GetEye();
		lay.normalize();
		Quat qtCamera;
		//回転は常にカメラを向くようにする
		qtCamera.facing(lay);
		//結果の読み取り
		D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };
		pID3D11DeviceContext->CopyResource(m_ReadBackBuffer.Get(), m_OutBuffer.Get());
		if (SUCCEEDED(pID3D11DeviceContext->Map(m_ReadBackBuffer.Get(), 0, D3D11_MAP_READ, 0, &MappedResource)))
		{
			memcpy(&m_ResultDataVec[0], MappedResource.pData, m_NumVertices * sizeof(LocalData));
			pID3D11DeviceContext->Unmap(m_ReadBackBuffer.Get(), 0);
			//行列の配列をクリア
			m_MatVec.clear();
			Mat4x4 worldMat;
			for (auto& v : m_ResultDataVec) {
				worldMat.affineTransformation(
					Vec3(m_Scale),
					Vec3(0.0f),
					qtCamera,
					v.m_WorldPosition
				);
				m_MatVec.push_back(worldMat);
			}
			//インスタンス行列の更新
			ptrDraw->UpdateMultiMatrix(m_MatVec);
			//結果を次の入力にコピー
			for (size_t i = 0; i < m_ResultDataVec.size(); i++) {
				m_LocalDataVec[i] = m_ResultDataVec[i];
			}
		}
		// CSの開放
		ID3D11UnorderedAccessView* ppUAViewNULL[1] = { nullptr };
		pID3D11DeviceContext->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, nullptr);
		ID3D11ShaderResourceView* ppSRVNULL[1] = { nullptr };
		pID3D11DeviceContext->CSSetShaderResources(0, 1, ppSRVNULL);
	}



	//--------------------------------------------------------------------------------------
	//　タイリングする固定のボックス
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
	//初期化
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
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		PtrDraw->SetOwnShadowActive(true);
		//描画コンポーネントテクスチャの設定
		PtrDraw->SetTextureResource(L"SKY_TX");
		//タイリング設定
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);

	}

	void TilingFixedBox::OnUpdate() {
	}


}
//end basecross
