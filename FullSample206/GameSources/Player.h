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
		// コントローラから方向ベクトルを得る
		Vec3 GetMoveVector() const;
		//文字列の表示
		void DrawStrings();
		//入力ハンドラー
		InputHandler<Player> m_InputHandler;
		//速度
		Vec3 m_Velocity;
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
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
		//Aボタン
		void OnPushA();
		//Xボタン
		void OnPushX();
		void MovePlayer();

	};






}
//end basecross

