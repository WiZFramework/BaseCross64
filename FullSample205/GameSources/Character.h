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
	//	class ChildSphere : public GameObject;
	//--------------------------------------------------------------------------------------
	class ChildSphere : public GameObject {
		weak_ptr<GameObject> m_Parent;
		Vec3 m_VecToParent;
		//ステートマシーン
		unique_ptr< StateMachine<ChildSphere> >  m_StateMachine;
	public:
		//構築と破棄
		ChildSphere(const shared_ptr<Stage>& StagePtr,
			const shared_ptr<GameObject>& Parent,
			const Vec3& VecToParent
		);
		virtual ~ChildSphere();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
		//親を追いかける処理
		void SeekParent();
		//宙刷りになってるかどうかのチェック
		bool IsHang();
		//アクセサ
		const unique_ptr<StateMachine<ChildSphere>>& GetStateMachine() {
			return m_StateMachine;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<ChildSphere>;
	//--------------------------------------------------------------------------------------
	class DefaultState : public ObjState<ChildSphere>
	{
		DefaultState() {}
	public:
		static shared_ptr<DefaultState> Instance();
		virtual void Enter(const shared_ptr<ChildSphere>& Obj)override;
		virtual void Execute(const shared_ptr<ChildSphere>& Obj)override;
		virtual void Exit(const shared_ptr<ChildSphere>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class GravState : public ObjState<ChildSphere>;
	//--------------------------------------------------------------------------------------
	class GravState : public ObjState<ChildSphere>
	{
		GravState() {}
	public:
		static shared_ptr<GravState> Instance();
		virtual void Enter(const shared_ptr<ChildSphere>& Obj)override;
		virtual void Execute(const shared_ptr<ChildSphere>& Obj)override;
		virtual void Exit(const shared_ptr<ChildSphere>& Obj)override;
	};


}
//end basecross
