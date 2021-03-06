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


	//リソースの作成
	void GameStage::CreateResourses() {
		wstring dataDir;
		App::GetApp()->GetDataDirectory(dataDir);
		wstring strTexture = dataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = dataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
	}



	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrMyCamera = ObjectFactory::Create<MyCamera>(10.0f);
		PtrView->SetCamera(PtrMyCamera);
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}


	//プレートの作成
	void GameStage::CreatePlate() {
		//ステージへのゲームオブジェクトの追加
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(XM_PIDIV2, 0, 0));
		Mat4x4 WorldMat;
		WorldMat.affineTransformation(
			Vec3(200.0f, 200.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Qt,
			Vec3(0.0f, 0.0f, 0.0f)
		);
		PtrTrans->SetScale(200.0f, 200.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		//描画コンポーネントの追加
		auto DrawComp = Ptr->AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//自分に影が映りこむようにする
		DrawComp->SetOwnShadowActive(true);

		//描画コンポーネントテクスチャの設定
		DrawComp->SetTextureResource(L"SKY_TX");
	}

	//FbxMeshContainerの作成
	void GameStage::CreateFbxMeshContainer() {
		auto FbxMeshObjectPtr = AddGameObject<FbxMeshContainer>();
		//シェア配列に追加
		SetSharedGameObject(L"FbxMeshContainer", FbxMeshObjectPtr);

	}

	void GameStage::OnCreate() {
		try {
			//リソースの作成
			CreateResourses();
			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
			//CreateFbxMeshContainerの作成
			CreateFbxMeshContainer();
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::ReadFbxFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale, const Vec3& Position,
		bool IsReadStatic, bool WithTangent, const wstring& NormalFileName, bool TextureWrap) {
		try {
			auto PtrFbxObj = GetSharedGameObject<FbxMeshContainer>(L"FbxMeshContainer");
			PtrFbxObj->ResetFbxMesh(Dir, FileName, MeshIndex, Scale, Position, IsReadStatic, WithTangent, NormalFileName, TextureWrap);
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::SaveStaticBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale) {
		try {
			auto PtrFbxObj = GetSharedGameObject<FbxMeshContainer>(L"FbxMeshContainer");
			PtrFbxObj->SaveStaticBinFile(Dir, FileName, MeshIndex, Scale);
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::SaveSkinBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale,
		UINT FrameParSec, UINT Start, UINT End) {
		try {
			auto PtrFbxObj = GetSharedGameObject<FbxMeshContainer>(L"FbxMeshContainer");
			PtrFbxObj->SaveSkinBinFile(Dir, FileName, MeshIndex, Scale, FrameParSec, Start, End);
		}
		catch (...) {
			throw;
		}
	}



	void GameStage::ClearFbxMesh() {
		auto PtrFbxObj = GetSharedGameObject<FbxMeshContainer>(L"FbxMeshContainer");
		PtrFbxObj->ClearFbxMesh();
	}


	bool GameStage::CheckSkinMesh() {
		auto PtrFbxObj = GetSharedGameObject<FbxMeshContainer>(L"FbxMeshContainer");
		return PtrFbxObj->CheckSkinMesh();
	}

	bool GameStage::CheckMesh() {
		auto PtrFbxObj = GetSharedGameObject<FbxMeshContainer>(L"FbxMeshContainer");
		return PtrFbxObj->CheckMesh();
	}

	void GameStage::MoveFbxFile(UINT FrameRate, UINT StartTime, UINT EndTime, bool IsLoop) {
		auto PtrFbxObj = GetSharedGameObject<FbxMeshContainer>(L"FbxMeshContainer");
		PtrFbxObj->MoveFbxMesh(FrameRate, StartTime, EndTime, IsLoop);
	}

	void GameStage::AnimePoseStart() {
		auto PtrFbxObj = GetSharedGameObject<FbxMeshContainer>(L"FbxMeshContainer");
		PtrFbxObj->AnimePoseStart();
	}




}
//end basecross
