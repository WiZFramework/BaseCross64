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
	//	四角形スプライト(構造体)
	//--------------------------------------------------------------------------------------
	struct SquareSprite
	{
		Vec2	m_LocalScale;			//ローカルスケール
		float	m_LocalRot;				//ローカル回転角度
		float	m_LocalRotVelocity;		//ローカル回転速度
		Vec2	m_LocalPos;				//ローカル位置
		Vec2	m_LocalVelocity;		//ローカル速度
		Vec2	m_LocalGravityVelocity;	//ローカル加速度
		Col4	m_RandomVertexColor;			//ローカル頂点カラー

		//CbvSrvのデスクプリタハンドルのインクリメントサイズ
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };

		//デスクリプタヒープ
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;

		//GPU側デスクリプタのハンドル配列
		vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

		//コンスタントバッファアップロードヒープ
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;

		//コンスタントバッファのGPU側変数
		void* m_pConstantBuffer{ nullptr };

		//コンスタントバッファ
		struct SpriteConstantBuffer
		{
			Mat4x4 World;
			Col4 Emissive;
			SpriteConstantBuffer()
			{
				memset(this, 0, sizeof(SpriteConstantBuffer));
			};
		};

		//コンスタントバッファデータ
		SpriteConstantBuffer m_SpriteConstantBuffer;
		SquareSprite() :
			m_LocalScale(64.f, 64.f),
			m_LocalRot(0.f),
			m_LocalRotVelocity(0.f),
			m_LocalPos(0.f, 0.f),
			m_LocalVelocity(0.f, 0.f),
			m_LocalGravityVelocity(0.f, 0.f),
			m_RandomVertexColor(0.0f, 0.0f, 0.0f, 0.0f)
		{
		}

		//デスクリプタヒープの作成
		void CreateDescriptorHeap();

		//コンスタントバッファの作成
		void CreateConstantBuffer();

		//コンスタントバッファの更新
		void UpdateConstantBuffer();
	};
	

	//--------------------------------------------------------------------------------------
	//	四角形スプライトのオブジェクトグループ
	//--------------------------------------------------------------------------------------
	class SquareSpriteGroup : public ObjectInterface, public ShapeInterface
	{

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		SquareSpriteGroup();

		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSpriteGroup();

		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw() override;


	private:
		//ルートシグネチャの作成
		void CreateRootSignature();

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

		//メッシュ
		shared_ptr<MeshResource> m_SquareSpriteMesh;

		//オブジェクト管理コンテナ
		vector<SquareSprite> m_SquareSpriteVec;

		float m_TotalTime;

		//各オブジェクトの位置等の変更
		void UpdateObjects(float ElapsedTime);

		//コンスタントバッファの更新
		void UpdateConstantBuffer();

		//ルートシグネチャ
		ComPtr<ID3D12RootSignature> m_RootSignature;

		//パイプラインステート
		ComPtr<ID3D12PipelineState> m_PipelineState;

		//コマンドリスト
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

	
	};


}
//end basecross