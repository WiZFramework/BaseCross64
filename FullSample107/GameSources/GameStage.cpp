/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ムービーステージクラス
	//--------------------------------------------------------------------------------------
	//初期化
	void MyMovieStage::OnCreate() {
		MovieStage::OnCreate();
		wstring dataDir;
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(dataDir);
		wstring strMovie = dataDir + L"testMovie.mp4";
		SetMovieFileName(strMovie);
		//再生
		Play();
	}

}
//end basecross
