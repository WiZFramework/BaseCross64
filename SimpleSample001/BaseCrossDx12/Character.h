/*!
@file Character.h
@brief キャラクターなど
@copyright shike.y
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//三角形オブジェクト
	class TriangleObject : public ObjectInterface, public ShapeInterface
	{

	public:
		TriangleObject();
		virtual ~TriangleObject();

		//初期化
		virtual void OnCreate() override;

		//更新
		void OnUpdate() override;

		//描画更新
		void OnDraw() override;

	private:
		
		//ルートシグネチャの作成
		void CreateRootSingnature();

		//パイプラインステートの作成
		void CreatePipelineState();

		//コマンドリストの作成
		void CreateCommandList();

		//描画処理
		void DrawObject();

		//メッシュ
		shared_ptr<MeshResource> m_TriangleMesh;

		//ルートシグネチャ
		ComPtr<ID3D12RootSignature> m_RootSignature;

		//パイプラインステート
		ComPtr<ID3D12PipelineState> m_PipelineState;

		//コマンドライン
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
	};
}
//end basecross
