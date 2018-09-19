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

		//デスクリプタヒープの作成
		void CreateDescriptorHeap();

		//コンスタントバッファの作成
		void CreateConstantBuffer();

		//パイプラインステートの作成
		void CreatePipelineState();

		//コマンドリストの作成
		void CreateCommandList();

		//描画処理
		void DrawObject();

		//コンスタントバッファの更新
		void UpdateConstantBuffer();

		//メッシュ
		shared_ptr<MeshResource> m_TriangleMesh;

		//現在の位置
		Vec3 m_Pos;
		//移動変更値
		Vec3 m_MoveSpan;	


		//ルートシグネチャ
		ComPtr<ID3D12RootSignature> m_RootSignature;
		//CbvSrvのデスクリプタハンドルのインクリメントサイズ
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };

		//デスクリプタヒープ
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;

		//GPU側デスクリプタのハンドル配列
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

		//コンスタントバッファ
		struct SpriteConstantBuffer
		{
			Mat4x4	World;
			Col4	Emissive;
			SpriteConstantBuffer()
			{
				memset(this, 0, sizeof(SpriteConstantBuffer));
			};
		};

		//コンスタントバッファのデータ
		SpriteConstantBuffer m_SpriteConstantBuffer;

		//コンスタントバッファアプロードヒープ
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;

		//コンスタントバッファのGPU側変数
		void* m_pConstantBuffer{ nullptr };

		//パイプラインステート
		ComPtr<ID3D12PipelineState> m_PipelineState;

		//コマンドライン
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
	};
}
//end basecross
