
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::CreateResourses() {
		wstring dataDir;
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(dataDir);
		//各ゲームは以下のようにデータディレクトリを取得すべき
		//App::GetApp()->GetDataDirectory(dataDir);
		wstring strTexture = dataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = dataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = dataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = dataDir + L"number.png";
		App::GetApp()->RegisterTexture(L"NUMBER_TX", strTexture);
		strTexture = dataDir + L"spark.png";
		App::GetApp()->RegisterTexture(L"SPARK_TX", strTexture);
		strTexture = dataDir + L"fire.png";
		App::GetApp()->RegisterTexture(L"FIRE_TX", strTexture);

		//以下オーディオ
		//サウンド
		wstring CursorWav = dataDir + L"cursor.wav";
		App::GetApp()->RegisterWav(L"cursor", CursorWav);

		//ミュージック
		wstring strMusic = dataDir + L"nanika .wav";
		App::GetApp()->RegisterWav(L"Nanika", strMusic);

	}

	void Scene::OnCreate(){
		try {
			//リソース作成
			CreateResourses();
			//BGMの再生
			auto ptrXA = App::GetApp()->GetXAudio2Manager();
			m_BGM = ptrXA->Start(L"Nanika", XAUDIO2_LOOP_INFINITE, 0.1f);
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
	}

	void Scene::OnDestroy() {
		//親クラスのOnDestroyを呼ぶ
		SceneBase::OnDestroy();

		auto ptrXA = App::GetApp()->GetXAudio2Manager();
		ptrXA->Stop(m_BGM);

	}


}
//end basecross
