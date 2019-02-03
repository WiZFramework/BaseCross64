/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//用途: 複数のスパーククラス
	//--------------------------------------------------------------------------------------
	class MultiSpark : public MultiParticle {
	public:
		//構築と破棄
		MultiSpark(shared_ptr<Stage>& StagePtr);
		virtual ~MultiSpark();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
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


	//--------------------------------------------------------------------------------------
	//	敵の箱
	//--------------------------------------------------------------------------------------
	class EnemyBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		EnemyBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~EnemyBox();
		//初期化
		virtual void OnCreate() override;
		//操作
		bool IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos, TRIANGLE& tri, Vec3& HitPoint);
		bool IsHitSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp, TRIANGLE& tri, Vec3& HitPoint);
	};

	//--------------------------------------------------------------------------------------
	///	Boneキャラ
	//--------------------------------------------------------------------------------------
	class BoneChara : public GameObject {
		Vec3 m_StartPos;
	public:
		//構築と破棄
		BoneChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~BoneChara();
		bool IsHitSegmentTriangles(const Vec3& StartPos, const Vec3& EndPos,TRIANGLE& tri, Vec3& HitPoint);
		bool IsHitSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp, TRIANGLE& tri, Vec3& HitPoint);
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	///	Ballキャラ
	//--------------------------------------------------------------------------------------
	class BallChara : public GameObject {
	public:
		//構築と破棄
		BallChara(const shared_ptr<Stage>& StagePtr);
		virtual ~BallChara();
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

}
//end basecross
