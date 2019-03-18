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
		auto ptrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto ptrMyCamera = ObjectFactory::Create<MyCamera>();
		ptrView->SetCamera(ptrMyCamera);
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto ptrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		ptrMultiLight->SetDefaultLighting();
	}

	//セルマップの作成
	void GameStage::CreateStageCellMap() {
		float  PieceSize = 1.0f;
		auto Ptr = AddGameObject<StageCellMap>(Vec3(-12.0f, 0, 1.0f), PieceSize,26, 16);
		//セルマップの区画を表示する場合は以下の設定
		Ptr->SetDrawActive(true);
		//さらにセルのインデックスとコストを表示する場合は以下の設定
		//Ptr->SetCellStringActive(true);
		SetSharedGameObject(L"StageCellMap", Ptr);
		//セルマップへのボックスのコスト設定
		SetCellMapCost();
	}


	//ボックスの作成
	void GameStage::CreateFixedBox() {

		vector< vector<Vec3> > baseVec = {
			{
			Vec3(50.0f, 1.0f, 50.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
			},
			{
			Vec3(50.0f, 1.0f, 50.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f,0.5f,19.5f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f,0.5f,-19.5f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f,XM_PIDIV2,0.0f),
			Vec3(19.5f,0.5f,0.0f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f,XM_PIDIV2,0.0f),
			Vec3(-19.5f,0.5f,0.0f)
			},
		};
		//床と壁オブジェクトの作成
		for (auto v : baseVec) {
			AddGameObject<TilingFixedBox>(v[0], v[1], v[2], 1.0f, 1.0f);
		}


		//障害物ボックスのグループを作成
		auto BoxGroup = CreateSharedObjectGroup(L"FixedBoxes");

		vector< vector<Vec3> > vec = {
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(-8.0f,0.5f,10.5f)
			},
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(0,0.5f,10.5f)
			},
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(8.0f,0.5f,10.5f)
			},
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(-6.0f,0.5f,5.5f)
			},
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(2.0f,0.5f,5.5f)
			},
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(10.0f,0.5f,5.5f)
			},

		};
		//オブジェクトの作成
		for (auto v : vec) {
			auto boxPtr = AddGameObject<TilingFixedBox>(v[0], v[1], v[2], 1.0f, 1.0f);
			//ボックスのグループに追加
			BoxGroup->IntoGroup(boxPtr);
		}
	}

	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//敵の作成
	void  GameStage::CreateEnamy() {
		auto PtrCellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		AddGameObject<Enemy>(PtrCellmap,Vec3(0,0.25f,15.0f));

	}


	//固定のボックスのコストをセルマップに反映
	void GameStage::SetCellMapCost() {
		//セルマップ内にFixedBoxの情報をセット
		auto PtrCellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		auto BoxGroup = GetSharedObjectGroup(L"FixedBoxes");
		//セルマップからセルの配列を取得
		auto& CellVec = PtrCellmap->GetCellVec();
		//ボックスグループからボックスの配列を取得
		auto& BoxVec = BoxGroup->GetGroupVector();
		vector<AABB> ObjectsAABBVec;
		for (auto& v : BoxVec) {
			auto FixedBoxPtr = dynamic_pointer_cast<TilingFixedBox>(v.lock());
			if (FixedBoxPtr) {
				auto ColPtr = FixedBoxPtr->GetComponent<CollisionObb>();
				//ボックスの衝突判定かラッピングするAABBを取得して保存
				ObjectsAABBVec.push_back(ColPtr->GetObb().GetWrappedAABB());
			}
		}
		//セル配列からセルをスキャン
		for (auto& v : CellVec) {
			for (auto& v2 : v) {
				for (auto& vObj : ObjectsAABBVec) {
					if (HitTest::AABB_AABB_NOT_EQUAL(v2.m_PieceRange, vObj)) {
						//ボックスのABBとNOT_EQUALで衝突判定
						v2.m_Cost = -1;
						break;
					}
				}
			}
		}
	}


	void GameStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
			//ボックスの作成
			CreateFixedBox();
			//セルマップの作成
			CreateStageCellMap();
			//プレーヤーの作成
			CreatePlayer();
			//敵の作成
			CreateEnamy();
		}
		catch (...) {
			throw;
		}
	}


}
//end basecross
