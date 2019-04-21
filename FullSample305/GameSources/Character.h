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
		enum class LocalState {
			Roll,
			Down,
			Fix
		};
		struct LocalData {
			LocalState m_State;
			Vec3 m_LocalPosition;
			Mat4x4 m_FixedMatrix;
			Vec3 m_Velocity;
			LocalData() :
				m_State(LocalState::Roll),
				m_LocalPosition(0.0f),
				m_FixedMatrix(),
				m_Velocity(0.0f)
			{}
		};
		vector<LocalData> m_LocalDataVec;
		vector<Mat4x4> m_MatVec;
		float m_Scale;
		Vec3 m_Position;
		//メッシュ
		shared_ptr<MeshResource> m_MeshRes;
		//ステートのUpdate
		void UpdateState();
		//各インスタンスのUpdate
		void UpdateInstances();
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
	};


}
//end basecross
