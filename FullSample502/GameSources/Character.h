/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	struct CB
	{
		Mat4x4 m_WorldMatrix;
		Vec4 m_PlayerPosition;
		// paramf[0] == elapsedTime
		// paramf[1] == velocityPower
		// paramf[2] == gravity_y
		// paramf[3] == baseY
		float paramf[4];
	};


	DECLARE_DX11_COMPUTE_SHADER(ComputeSaderCalcbody)

	DECLARE_DX11_CONSTANT_BUFFER(ConstantBufferCalcbody, CB)


	//--------------------------------------------------------------------------------------
	//　点で構成された球体
	//-----------------------------------------------ix---------------------------------------
	class PointsBall : public GameObject {
		const UINT NUM_ELEMENTS = 64 * 32;
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
		vector<Mat4x4> m_MatVec;
		float m_Scale;
		Vec3 m_Position;
		//メッシュ
		shared_ptr<MeshResource> m_MeshRes;
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
