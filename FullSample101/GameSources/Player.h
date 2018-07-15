/*!
@file Player.h
@brief プレイヤーなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	プレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public GameObject {
		//最高速度
		float m_MaxSpeed;
		//減速率
		float m_Decel;
		//質量
		float m_Mass;
		// コントローラから方向ベクトルを得る
		Vec3 GetMoveVector() const;
		//文字列の表示
		void DrawStrings();
		//ステートマシーン
		unique_ptr<StateMachine<Player>>  m_StateMachine;
		//入力ハンドラー
		InputHandler<Player> m_InputHandler;
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		Player(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Player() {}
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	最高速度を得る
		@return	最高速度
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxSpeed() const {
			return m_MaxSpeed;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	減速値を得る
		@return	減速値
		*/
		//--------------------------------------------------------------------------------------
		float GetDecel() const {
			return m_Decel;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	質量を得る
		@return	質量
		*/
		//--------------------------------------------------------------------------------------
		float GetMass() const {
			return m_Mass;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートマシンを得る
		@return	ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr<StateMachine<Player>>& GetStateMachine() {
			return m_StateMachine;
		}
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
		//Aボタン
		void OnPushA() {}
		void MovePlayer();
		virtual void OnCollisionEnter(vector<shared_ptr<GameObject>>& OtherVec) override;
		virtual void OnCollisionExcute(vector<shared_ptr<GameObject>>& OtherVec) override;
		virtual void OnCollisionExit(vector<shared_ptr<GameObject>>& OtherVec) override;
	};


	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	class PlayerDefaultState : public ObjState<Player>
	{
		PlayerDefaultState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerDefaultState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	///	ジャンプステート
	//--------------------------------------------------------------------------------------
	class PlayerJumpState : public ObjState<Player>
	{
		PlayerJumpState() {}
	public:
		//ステートのインスタンス取得
		DECLARE_SINGLETON_INSTANCE(PlayerJumpState)
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};





}
//end basecross

