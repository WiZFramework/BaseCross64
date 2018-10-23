/*!
@file Character.h
@brief キャラクターなど
@copyright shike.y
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	四角形スプライト
	//--------------------------------------------------------------------------------------
	class SquareSprite : public ObjectInterface, public ShapeInterface {

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		SquareSprite(const wstring& TextureFileName, bool Trace, const Vec2& StartPos);

		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSprite();

		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;

	private:
		// メッシュ
		shared_ptr<MeshResource> m_SquareMesh;

		Vec2 m_Scale;				///<スケーリング
		float m_Rot;				///<回転角度
		Vec2 m_Pos;					///<位置
		Vec2 m_PosSpan;				///<位置変更間隔

		float m_TotalTime;			///<タイム


		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		wstring m_TextureFileName;						///<テクスチャファイル名
		bool m_Trace;									///<透明処理するかどうか
		Col4 m_Diffuse;									///<変化させるデフィーズ


		//コンスタントバッファ更新
		void UpdateConstantBuffer();

		//ルートシグネチャ
		ComPtr<ID3D12RootSignature> m_RootSignature;

		//CbvSrvのデスクプリタハンドルのインクリメントサイズ
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };

		//デスクプリタヒープ
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;

		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;


		//GPU側デスクプリタのハンドルの配列
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

		//コンスタントバッファ
		struct DiffuseSpriteConstantBuffer
		{
			Mat4x4 World;
			Col4 Emissive;
			Col4 Diffuse;
			DiffuseSpriteConstantBuffer() {
				memset(this, 0, sizeof(DiffuseSpriteConstantBuffer));
			};
		};

		///コンスタントバッファデータ
		DiffuseSpriteConstantBuffer m_DiffuseSpriteConstantBuffer;
		///コンスタントバッファアップロードヒープ
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///コンスタントバッファのGPU側変数
		void* m_pConstantBuffer{ nullptr };
		///パイプラインステート
		ComPtr<ID3D12PipelineState> m_PipelineState;
		///コマンドリスト
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
	


		//ルートシグネチャ作成
		void CreateRootSignature();

		//デスクプリタヒープ作成
		void CreateDescriptorHeap();

		//サンプラー作成
		void CreateSampler();

		//シェーダーリソースビュー作成
		void CreateShaderResourceView();

		//コンスタントバッファ作成
		void CreateConstantBuffer();

		//パイプラインステート作成
		void CreatePipelineState();

		//コマンドリスト作成
		void CreateCommandList();

		//描画処理
		void DrawObject();
	};
}
//end basecross