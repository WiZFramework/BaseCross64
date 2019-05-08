/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{



	//--------------------------------------------------------------------------------------
	//　点で構成された球体
	//-----------------------------------------------ix---------------------------------------
	class PointsBall : public GameObject {
		const UINT NUM_ELEMENTS = 32768;
		UINT m_NumVertices;
		struct LocalData {
			Vec4 m_LocalPosition;
			Vec4 m_WorldPosition;
			Vec4 m_Velocity;
			UINT m_State[4];
			LocalData() :
				m_LocalPosition(0.0f),
				m_WorldPosition(0.0f),
				m_Velocity(0.0f)
			{
				for (auto& v : m_State) {
					v = 0;
				}
			}
		};
		//エレメントバッファ
		ComPtr<ID3D11Buffer> m_InBuffer;
		ComPtr<ID3D11Buffer> m_OutBuffer;
		//シェーダーリソースビュー
		ComPtr<ID3D11ShaderResourceView> m_SRV;
		//アクセスビュー
		ComPtr < ID3D11UnorderedAccessView>  m_UAV;
		//リードバックバッファ
		ComPtr<ID3D11Buffer> m_ReadBackBuffer;
		vector<LocalData> m_LocalDataVec;
		vector<LocalData> m_ResultDataVec;
		float m_Scale;
		Vec3 m_Position;
		//以下描画用
		struct DrawData {
			Vec4 m_Pos;
		};
		vector<DrawData> m_MatVec;
		//メッシュ
		shared_ptr<MeshResource> m_MeshRes;
		//エレメントバッファ
		ComPtr<ID3D11Buffer> m_DrawBuffer;
		//シェーダーリソースビュー
		ComPtr<ID3D11ShaderResourceView> m_DrawSRV;
	public:
		//構築と破棄
		PointsBall(const shared_ptr<Stage>& StagePtr,
			float Scale,
			const Vec3& Position
		);
		virtual ~PointsBall();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
		//描画
		virtual void OnDraw() override;
	};




	//--------------------------------------------------------------------------------------
	//　タイリングする固定のボックス
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
	};


}
//end basecross
