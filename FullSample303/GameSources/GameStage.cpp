/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PttMyCamera = ObjectFactory::Create<MyCamera>();
		PtrView->SetCamera(PttMyCamera);
		PttMyCamera->SetEye(Vec3(0.0f, 7.0f, -7.0f));
		PttMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}

	//固定のボックスの作成
	void GameStage::CreateFixedBox() {
		//配列の初期化
		vector< vector<Vec3> > Vec = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//球の作成
	void GameStage::CreateSphere() {
		AddGameObject<PcSphere>(Vec3(-7.0f, 0.5f, 5.0f));
		AddGameObject<PnSphere>(Vec3(-5.0f, 0.5f, 5.0f));
		AddGameObject<PtSphere>(Vec3(-3.0f, 0.5f, 5.0f));
		AddGameObject<PctSphere>(Vec3(-1.0f, 0.5f, 5.0f));
		AddGameObject<PntSphere>(Vec3(1.0f, 0.5f, 5.0f), true);
		AddGameObject<PntSphere>(Vec3(3.0f, 0.5f, 5.0f), false);
		AddGameObject<PntSpSphere>(Vec3(5.0f, 0.5f, 5.0f), true);
		AddGameObject<PntSpSphere>(Vec3(7.0f, 0.5f, 5.0f), false);

	}

	//キャラクターの作成
	void GameStage::CreateChara() {
		AddGameObject<StaticChara>(Vec3(2.5f, 0.0f, 0.0f), false);
		AddGameObject<StaticChara>(Vec3(5.0f, 0.0f, 0.0f), true);
		AddGameObject<StaticMultiMeshChara>(Vec3(2.5f, 0.0f, -2.0f), false);
		AddGameObject<StaticMultiMeshChara>(Vec3(5.0f, 0.0f, -2.0f), true);
		AddGameObject<BoneChara>(Vec3(-5.0f, 0.0f, 0.0f), false);
		AddGameObject<BoneChara>(Vec3(-2.5f, 0.0f, 0.0f), true);
		AddGameObject<BoneMultiMeshChara>(Vec3(-5.0f, 0.0f, -2.0f), false);
		AddGameObject<BoneMultiMeshChara>(Vec3(-2.5f, 0.0f, -2.0f), true);
	}

	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
		PlayerPtr->AddTag(L"Player");
	}




	void GameStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
			//固定のボックスの作成
			CreateFixedBox();
			//球の作成
			CreateSphere();
			//キャラクターの作成
			CreateChara();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
