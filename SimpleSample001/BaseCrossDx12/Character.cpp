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
		ShapeInterface()
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

		//パイプラインステートの作成
		CreatePipelineState();

		//コマンドリストの作成
		CreateCommandList();
	}

	//更新
	void TriangleObject::OnUpdate()
	{
		//なにもなし
	}

	//描画更新
	void TriangleObject::OnDraw()
	{
		//m_TriangleMeshの描画
		DrawObject();
	}

	//ルートシグネチャの作成
	void TriangleObject::CreateRootSingnature()
	{
		//一番シンプルなルートシグネチャの作成
		m_RootSignature = RootSignature::CreateSimple();
	}

	//パイプラインステートの作成
	void TriangleObject::CreatePipelineState()
	{
		//パイプラインステートの定義
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		m_PipelineState 
			= PipelineState::CreateDefault2D<VertexPositionColor, VSPCDirect, PSPCDirect>(m_RootSignature, PipeLineDesc);
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

		//コマンドリストの設定
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		m_CommandList->RSSetViewports(1, &Device->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Device->GetScissorRect());

		//レンダリングターゲットビューのハンドルを取得
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
}
//end basecross
