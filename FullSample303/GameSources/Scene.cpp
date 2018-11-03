
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

		//ボーンモデルの通常リソース
		auto modelMesh = MeshResource::CreateBoneModelMesh(dataDir, L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"Chara_R_MESH", modelMesh);

		//ボーンモデルのタンジェント付きリソース
		modelMesh = MeshResource::CreateBoneModelMeshWithTangent(dataDir, L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"Chara_R_MESH_WITH_TAN", modelMesh);
		//法線マップ
		strTexture = dataDir + L"Chara_R_narmal.png";
		App::GetApp()->RegisterTexture(L"Chara_R_NORMAL_TX", strTexture);

		//ボーンモデル(マルチメッシュ)の通常リソース
		auto multiModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(dataDir, L"Object_WalkAnimation.bmf");
		App::GetApp()->RegisterResource(L"Object_WalkAnimation_MESH", multiModelMesh);

		//ボーンモデル(マルチメッシュ)のタンジェント付きリソース
		multiModelMesh = MultiMeshResource::CreateBoneModelMultiMeshWithTangent(dataDir, L"Object_WalkAnimation.bmf");
		App::GetApp()->RegisterResource(L"Object_WalkAnimation_MESH_WITH_TAN", multiModelMesh);

		//法線マップ
		strTexture = dataDir + L"Tx_Checker_Normal.png";
		App::GetApp()->RegisterTexture(L"OBJECT_NORMAL_TX", strTexture);


		//スタティックモデルの通常リソース
		auto staticModelMesh = MeshResource::CreateStaticModelMesh(dataDir, L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH", staticModelMesh);
		//スタティックモデルのタンジェント付きリソース
		staticModelMesh = MeshResource::CreateStaticModelMeshWithTangent(dataDir, L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH_WITH_TAN", staticModelMesh);
		//法線マップ
		strTexture = dataDir + L"Character_2_normal.png";
		App::GetApp()->RegisterTexture(L"MODEL_NORMAL_TX", strTexture);

		//スタティックモデル(マルチメッシュ)の通常リソース
		auto staticMultiModelMesh = MultiMeshResource::CreateStaticModelMultiMesh(dataDir, L"ObjectOnly.bmf");
		App::GetApp()->RegisterResource(L"ObjectOnly_MESH", staticMultiModelMesh);

		//スタティックモデル(マルチメッシュ)のタンジェント付きリソース
		staticMultiModelMesh = MultiMeshResource::CreateStaticModelMultiMeshWithTangent(dataDir, L"ObjectOnly.bmf");
		App::GetApp()->RegisterResource(L"ObjectOnly_MESH_WITH_TAN", staticMultiModelMesh);


	}

	void Scene::OnCreate(){
		try {
			//リソース作成
			CreateResourses();
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



}
//end basecross
