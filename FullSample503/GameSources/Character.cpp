/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

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
		m_LocalDataVec.resize(NUM_ELEMENTS);
		//球体を作成
		vector<VertexPosition> vertices;
		vector<VertexPositionNormalTexture> seedV;
		vector<uint16_t> seedI;
		float diam = 1.0f;
		for (int i = 0; i < 9; i++) {
			MeshUtill::CreateSphere(diam, 30, seedV, seedI);
			for (auto& v : seedV) {
				VertexPosition p;
				p.position = v.position;
				vertices.push_back(p);
			}
			seedV.clear();
			seedI.clear();
			diam -= 0.1f;
		}
		m_NumVertices = (UINT)vertices.size();
		for (size_t i = 0; i < m_NumVertices; i++) {
			LocalData tempLocalData;
			tempLocalData.m_LocalPosition = vertices[i].position;
			m_LocalDataVec[i] = tempLocalData;
		}
		m_ResultDataVec.resize(m_NumVertices);
		//全体の位置関連
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(Vec3(m_Scale));
		ptrTransform->SetRotation(Vec3(0));
		ptrTransform->SetPosition(m_Position);
		//各頂点ごとに行列を作成
		for (size_t i = 0; i < m_NumVertices; i++) {
			Mat4x4 tempMat;
			tempMat.affineTransformation(
				Vec3(1.0f),
				Vec3(0.0f),
				Vec3(0.0f),
				m_LocalDataVec[i].m_LocalPosition
			);
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
		srvbuffer_desc.Buffer.NumElements = NUM_ELEMENTS;
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

		//描画用メッシュの作成
		//位置情報と色
		vector<VertexPositionColor> meshVertices;
		float colG = 1.0f;
		for (auto& v : vertices) {
			VertexPositionColor temp;
			temp.position = v.position;
			temp.color = Col4(1, colG, 0, 1);
			meshVertices.push_back(temp);
			colG -= 0.00002f;
			if (colG < 0) {
				colG = 0;
			}
		}
		m_MeshRes = MeshResource::CreateMeshResource(meshVertices,false);

		//描画用エレメンツバッファ
		D3D11_BUFFER_DESC draw_buffer_desc = { 0 };
		draw_buffer_desc.ByteWidth = NUM_ELEMENTS * sizeof(DrawData);
		draw_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		draw_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		draw_buffer_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		draw_buffer_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		draw_buffer_desc.StructureByteStride = sizeof(DrawData);
		ThrowIfFailed(
			pDx11Device->CreateBuffer(&draw_buffer_desc, nullptr, &m_DrawBuffer),
			L"m_DrawBuffer作成に失敗しました",
			L"pDx11Device->CreateBuffer()",
			L"PointsBall::OnCreate()"
		);

		//描画用シェーダリソースビュー
		D3D11_SHADER_RESOURCE_VIEW_DESC draw_srvbuffer_desc = {};
		draw_srvbuffer_desc.Format = DXGI_FORMAT_UNKNOWN;
		draw_srvbuffer_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER ;
		draw_srvbuffer_desc.Buffer.FirstElement = 0;
		draw_srvbuffer_desc.Buffer.NumElements = NUM_ELEMENTS;
		ThrowIfFailed(
			pDx11Device->CreateShaderResourceView(m_DrawBuffer.Get(), &draw_srvbuffer_desc, &m_DrawSRV),
			L"m_DrawSRV作成に失敗しました",
			L"pDx11Device->CreateShaderResourceView()",
			L"PointsBall::OnCreate()"
		);
	}


	void PointsBall::OnUpdate() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		auto ptrTransform = GetComponent<Transform>();
		//全体を回転させる
		auto worldQt = ptrTransform->GetQuaternion();
		Quat spanQt(Vec3(0, 1, 0), elapsedTime);
		worldQt *= spanQt;
		ptrTransform->SetQuaternion(worldQt);
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		CBCS cbcs = {};
		auto playerSh = GetStage()->GetSharedGameObject<Player>(L"Player");
		cbcs.m_PlayerPosition = playerSh->GetComponent<Transform>()->GetWorldPosition();
		cbcs.m_PlayerPosition.w = 1.0f;
		cbcs.m_WorldMatrix = ptrTransform->GetWorldMatrix();
		// paramf[0] == elapsedTime
		// paramf[1] == velocityPower
		// paramf[2] == gravity_y
		// paramf[3] == baseY
		cbcs.paramf[0] = elapsedTime;
		cbcs.paramf[1] = 3.0f;
		cbcs.paramf[2] = -9.8f;
		cbcs.paramf[3] = m_Scale * 0.02f;
		ID3D11Buffer* pConstantBuffer = CBCalcPointsBall::GetPtr()->GetBuffer();
		pID3D11DeviceContext->UpdateSubresource(pConstantBuffer, 0, nullptr, &cbcs, 0, 0);
		pID3D11DeviceContext->CSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->CSSetShader(CSPointsBall::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->UpdateSubresource(m_InBuffer.Get(), 0, nullptr, &m_LocalDataVec[0], 0, 0);
		pID3D11DeviceContext->CSSetShaderResources(0, 1, m_SRV.GetAddressOf());
		pID3D11DeviceContext->CSSetUnorderedAccessViews(0, 1, m_UAV.GetAddressOf(), nullptr);
		pID3D11DeviceContext->Dispatch((UINT)m_NumVertices, 1, 1);
		//結果の読み取り
		D3D11_MAPPED_SUBRESOURCE MappedResource = { 0 };
		pID3D11DeviceContext->CopyResource(m_ReadBackBuffer.Get(), m_OutBuffer.Get());
		if (SUCCEEDED(pID3D11DeviceContext->Map(m_ReadBackBuffer.Get(), 0, D3D11_MAP_READ, 0, &MappedResource)))
		{
			memcpy(&m_ResultDataVec[0], MappedResource.pData, m_NumVertices * sizeof(LocalData));
			pID3D11DeviceContext->Unmap(m_ReadBackBuffer.Get(), 0);
			//行列の配列をクリア
			m_MatVec.clear();
			DrawData drawdata;
			for (auto& v : m_ResultDataVec) {
				v.m_WorldPosition.w = 1.0f;
				drawdata.m_Pos = v.m_WorldPosition;
				m_MatVec.push_back(drawdata);
			}
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

	void PointsBall::OnDraw() {
		auto ptrTrans = GetComponent<Transform>();
		//カメラを得る
		auto cameraPtr = OnGetDrawCamera();

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//NULLのシェーダリソースの準備
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		//サンプラーの準備
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		//各オブジェクト共通処理
		//シェーダの設定
		//頂点シェーダ
		pD3D11DeviceContext->VSSetShader(VSPointsBall::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPointsBall::GetPtr()->GetInputLayout());
		//ジオメトリシェーダ
		pD3D11DeviceContext->GSSetShader(GSPointsBall::GetPtr()->GetShader(), nullptr, 0);
		//シェーダーリソース
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		if (SUCCEEDED(pD3D11DeviceContext->Map(m_DrawBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource)))
		{
			auto pDrawBuff = reinterpret_cast<DrawData*>(MappedResource.pData);
			memcpy(pDrawBuff, &m_MatVec[0], m_NumVertices * sizeof(DrawData));
			pD3D11DeviceContext->Unmap(m_DrawBuffer.Get(), 0);
		}
		pD3D11DeviceContext->VSSetShaderResources(0, 1, m_DrawSRV.GetAddressOf());
		//ピクセルシェーダ
		pD3D11DeviceContext->PSSetShader(PSPointsBall::GetPtr()->GetShader(), nullptr, 0);
		auto shTex = App::GetApp()->GetResource<TextureResource>(L"SPARK_TX");
		auto ptrSh = shTex->GetShaderResourceView().Get();
		pD3D11DeviceContext->PSSetShaderResources(0, 1, &ptrSh);
		auto ptr = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &ptr);
		//個別処理
		CBVS cbvs;
		cbvs.m_World = ptrTrans->GetWorldMatrix();
		cbvs.m_World.transpose();
		cbvs.m_View = cameraPtr->GetViewMatrix();
		cbvs.m_View.transpose();
		cbvs.m_Proj = cameraPtr->GetProjMatrix();
		cbvs.m_Proj.transpose();
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBDrawPointsBall::GetPtr()->GetBuffer(), 0, nullptr, &cbvs, 0, 0);
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBDrawPointsBall::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//ジオメトリシェーダに渡す
		pD3D11DeviceContext->GSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//描画方法のセット
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//各レンダリングステートの設定
		//ブレンドステート
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthRead(), 0);
		//ラスタライザステートと描画
		//透明処理用
		//ラスタライザステート(裏描画)
		pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
		//描画
		pD3D11DeviceContext->Draw(m_NumVertices, 0);
		//ラスタライザステート（表描画）
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
		//描画
		pD3D11DeviceContext->Draw(m_NumVertices, 0);
		Dev->InitializeStates();
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

//		SetDrawActive(false);

	}

	void TilingFixedBox::OnUpdate() {
	}


}
//end basecross
