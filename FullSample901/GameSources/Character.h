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
	//	障害物球
	//--------------------------------------------------------------------------------------
	class FixedSphere : public GameObject {
		float m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		FixedSphere(const shared_ptr<Stage>& StagePtr,
			const float Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedSphere();
		//初期化
		virtual void OnCreate() override;
		//操作
	};


	//--------------------------------------------------------------------------------------
	//	配置されるオブジェクトの親
	//--------------------------------------------------------------------------------------
	class BaseChara : public GameObject {
		//ステートマシーン
		unique_ptr< StateMachine<BaseChara> >  m_StateMachine;
		Vec3 m_StartPos;
		float m_StateChangeSize;
		//フォース
		Vec3 m_Force;
		//速度
		Vec3 m_Velocity;
		void ApplyForce();
	protected:
		//構築と破棄
		BaseChara(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~BaseChara();
	public:
		//アクセサ
		const unique_ptr<StateMachine<BaseChara>>& GetStateMachine() {
			return m_StateMachine;
		}
		Vec3 GetStartPos() const {
			return m_StartPos;
		}
		float GetStateChangeSize() const {
			return m_StateChangeSize;
		}
		const Vec3& GetForce()const {
			return m_Force;
		}
		void SetForce(const Vec3& f) {
			m_Force = f;
		}
		void AddForce(const Vec3& f) {
			m_Force += f;
		}
		const Vec3& GetVelocity()const {
			return m_Velocity;
		}
		void SetVelocity(const Vec3& v) {
			m_Velocity = v;
		}
		shared_ptr<GameObject>  GetTarget()const;
		virtual void NearBehavior() = 0;
		virtual void FarBehavior() = 0;
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//	プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	class FarState : public ObjState<BaseChara>
	{
		FarState() {}
	public:
		static shared_ptr<FarState> Instance();
		virtual void Enter(const shared_ptr<BaseChara>& Obj)override;
		virtual void Execute(const shared_ptr<BaseChara>& Obj)override;
		virtual void Exit(const shared_ptr<BaseChara>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	 プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	class NearState : public ObjState<BaseChara>
	{
		NearState() {}
	public:
		static shared_ptr<NearState> Instance();
		virtual void Enter(const shared_ptr<BaseChara>& Obj)override;
		virtual void Execute(const shared_ptr<BaseChara>& Obj)override;
		virtual void Exit(const shared_ptr<BaseChara>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	class SeekObject : public BaseChara {
	public:
		//構築と破棄
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SeekObject();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void NearBehavior() override;
		virtual void FarBehavior() override;
	};


	//--------------------------------------------------------------------------------------
	//	追跡する配置オブジェクト
	//--------------------------------------------------------------------------------------
	class PursuitObject : public BaseChara {
	public:
		//構築と破棄
		PursuitObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~PursuitObject();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void NearBehavior() override;
		virtual void FarBehavior() override;
	};

	//--------------------------------------------------------------------------------------
	//	パスを巡回する配置オブジェクト
	//--------------------------------------------------------------------------------------
	class FollowPathObject : public BaseChara {
	public:
		//構築と破棄
		FollowPathObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~FollowPathObject();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void NearBehavior() override;
		virtual void FarBehavior() override;
	};





}
//end basecross
