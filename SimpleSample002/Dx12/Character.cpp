/*!
@file Character.cpp
@brief キャラクターなど実体
@copyright shike.y
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//三角オブジェクト
	TriangleObject::TriangleObject() :
		ObjectInterface(),
		ShapeInterface(),
		m_Pos(0.f, 0.f, 0.f),
		m_MoveSpan(0.01f, 0.f, 0.f)
	{
	}

	TriangleObject::~TriangleObject()
	{
	}

	//初期化
	void TriangleObject::OnCreate()
	{
		//頂点データからメッシュリソースを作成
		vector<VertexPositionColor> vertices = 
		{
			{ Vec3(0.0f, 0.5f , 0.0f),Col4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vec3(0.5f, -0.5f, 0.5f),Col4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vec3(-0.5f, -0.5f, 0.5f),Col4(0.0f, 0.0f, 1.0f, 1.0f) }
		};

		//頂点座標から三角形のメッシュを作成
		/*原点が X0 Y0として頂点3箇所を上記の配列で作成
				.
			   / \
			  /   \
			 /     \
			.______ \.
		*/
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices, false);

		//ルートシグネチャの作成
		CreateRootSingnature();

		//デスクリプタヒープの作成
		CreateDescriptorHeap();

		//コンスタントバッファの作成
		CreateConstantBuffer();

		//パイプラインステートの作成
		CreatePipelineState();

		//コマンドリストの作成
		CreateCommandList();

		//コンスタントバッファの更新
		UpdateConstantBuffer();
	}

	//更新
	void TriangleObject::OnUpdate()
	{
		m_Pos += m_MoveSpan;
		if (abs(m_Pos.x) >= 0.5f)
		{
			m_MoveSpan *= -1.0f;
		}
	}

	//描画更新
	void TriangleObject::OnDraw()
	{
		//コンスタントバッファの更新
		UpdateConstantBuffer();
		//m_TriangleMeshの描画
		DrawObject();
	}

	//ルートシグネチャの作成
	void TriangleObject::CreateRootSingnature()
	{
		//一番シンプルなルートシグネチャの作成
		m_RootSignature = RootSignature::CreateCbv();
	}

	//デスクリプタヒープの作成
	void TriangleObject::CreateDescriptorHeap()
	{
		auto Device = App::GetApp()->GetDeviceResources();

		m_CbvSrvDescriptorHandleIncrementSize
			= Device->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//CbvSrvデスクリプタヒープ(コンスタントバッファのみ)
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);

		//GPU側デスクリプタヒープのハンドル配列の作成
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}

	//コンスタントバッファの作成
	void TriangleObject::CreateConstantBuffer()
	{
		auto Device = App::GetApp()->GetDeviceResources();

		ThrowIfFailed(Device->GetD3DDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteConstantBuffer) + 255) & ~255),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"コンスタントバッファ用のアップリードヒープ作成に失敗しました",
			L"Device->GetDevice()->CreateCommittedResource()",
			L"TriangleObject::CreateConstantBuffer()"
		);
		
		//コンスタントバッファビューの作成
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();

		//コンスタントバッファは255バイトにアラインメント
		cbvDesc.SizeInBytes = (sizeof(SpriteConstantBuffer) + 255) & ~255;

		//コンスタントバッファビューを作成すべきデスクリプタヒープ上のハンドルを取得
		//シェーダーリソースがある場合、コンスタントバッファはシェーダーリソースビューのあとに配置する
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		Device->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);

		//コンスタントバッファのアップロードヒープのマップ
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"コンスタントバッファのマップ作成に失敗しました",
			L"m_ConstantBufferUploadHeap->Map",
			L"TriangleObject::CreateConstantBuffer()"
		);
	}

	//パイプラインステートの作成
	void TriangleObject::CreatePipelineState()
	{
		//パイプラインステートの定義
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		m_PipelineState 
			= PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(m_RootSignature, PipeLineDesc);
	}

	//コマンドリストの作成
	void TriangleObject::CreateCommandList()
	{
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	//描画処理
	void TriangleObject::DrawObject()
	{
		auto Device = App::GetApp()->GetDeviceResources();

		//コマンドリストのリセット
		CommandList::Reset(m_PipelineState, m_CommandList);

		//メッシュの更新があれば、リソースの更新
		m_TriangleMesh->UpdateResources<VertexPositionColor>(m_CommandList);

		//ルートシグネチャのセット
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());

		//デスクリプタヒープのセット
		ID3D12DescriptorHeap* ppHeap[] = { m_CbvSrvUavDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);
		
		//GPUデスクリプタヒープにセット
		for (UINT i = 0; i < m_GPUDescriptorHandleVec.size(); i++)
		{
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}

		m_CommandList->RSSetViewports(1, &Device->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Device->GetScissorRect());

		//レンダーターゲットビューハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Device->GetRtvHandle();

		//デプスステンシルビューのハンドルを取得
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Device->GetDsvHandle();
		//取得したハンドルをセット
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetVertexBuffers(0, 1, &m_TriangleMesh->GetVertexBufferView());
		m_CommandList->DrawInstanced(m_TriangleMesh->GetNumVertices(), 1, 0, 0);

		//コマンドリストのクローズ
		CommandList::Close(m_CommandList);

		//デバイスにコマンドリストを送る
		Device->InsertDrawCommandLists(m_CommandList.Get());
	}

	//コンスタントバッファの更新
	void TriangleObject::UpdateConstantBuffer()
	{
		//コンスタントバッファの準備
		m_SpriteConstantBuffer.Emissive = Col4(0.f, 0.f, 0.f, 1.f);

		Mat4x4 mat;
		mat.translation(m_Pos);
		m_SpriteConstantBuffer.World = mat;

		//更新
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_SpriteConstantBuffer), sizeof(m_SpriteConstantBuffer));
	}
}
//end basecross
