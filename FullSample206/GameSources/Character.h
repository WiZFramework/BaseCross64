/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	///	発射する球体
	//--------------------------------------------------------------------------------------
	class FireSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
		float m_ActiveMaxY;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Emitter	発射位置
		@param[in]	Velocity	速度
		*/
		//--------------------------------------------------------------------------------------
		FireSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
		);
		virtual ~FireSphere();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(const CollisionPair& Pair) override;
		virtual void OnCollisionExcute(const CollisionPair& Pair) override;
		//物体をリセットする
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};


}
//end basecross
