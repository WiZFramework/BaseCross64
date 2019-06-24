/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	ムービーステージクラス
	//--------------------------------------------------------------------------------------
	class MyMovieStage : public MovieStage {
	public:
		//構築と破棄
		MyMovieStage() :MovieStage() {}
		virtual ~MyMovieStage() {}
		//初期化
		virtual void OnCreate()override;
	};



}
//end basecross

