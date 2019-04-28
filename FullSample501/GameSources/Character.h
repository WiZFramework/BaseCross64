/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	//　コンスタントバッファ
	//--------------------------------------------------------------------------------------
	struct CB
	{
		// paramf[0] == Totaltime
		float paramf[4];
	};
	//コンスタントバッファのヘッダ部
	DECLARE_DX11_CONSTANT_BUFFER(ConstantBufferCalcbody, CB)

	//--------------------------------------------------------------------------------------
	//　コンピュートシェーダー
	//--------------------------------------------------------------------------------------
	//CSのヘッダ部
	DECLARE_DX11_COMPUTE_SHADER(ComputeSaderCalcbody)


	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject {
		// エレメントデータの構造体
		struct Element
		{
			XMFLOAT4 pos;
		};
		//エレメントバッファ
		ComPtr<ID3D11Buffer> m_Buffer;
		//アクセスビュー
		ComPtr < ID3D11UnorderedAccessView>  m_UAV;
		//リードバックバッファ
		ComPtr<ID3D11Buffer> m_ReadBackBuffer;
		//トータルアイム
		float m_TotalTime;
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		Vec3 m_Velocity;
	public:
		//構築と破棄
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~MoveBox();
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
	};


}
//end basecross
