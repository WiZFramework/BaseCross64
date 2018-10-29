/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	//構築
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Scale(0.25f)
	{}

	void Player::RotToHead(const Vec3& Velocity, float LerpFact) {
		if (LerpFact <= 0.0f) {
			//補間係数が0以下なら何もしない
			return;
		}
		auto ptrTransform = GetComponent<Transform>();
		//回転の更新
		if (Velocity.length() > 0.0f) {
			Vec3 temp = Velocity;
			temp.normalize();
			float toAngle = atan2(temp.x, temp.z);
			Quat qt;
			qt.rotationRollPitchYawFromVector(Vec3(0, toAngle, 0));
			qt.normalize();
			//現在の回転を取得
			Quat nowQt = ptrTransform->GetQuaternion();
			//現在と目標を補間
			if (LerpFact >= 1.0f) {
				nowQt = qt;
			}
			else {
				//クオータニオンの補間処理
				nowQt = XMQuaternionSlerp(nowQt, qt, LerpFact);
			}
			ptrTransform->SetQuaternion(nowQt);
		}
	}


	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);
		//コントローラの取得
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (cntlVec[0].bConnected) {
			if (cntlVec[0].fThumbLX != 0 || cntlVec[0].fThumbLY != 0) {
				auto ptrTransform = GetComponent<Transform>();
				auto ptrCamera = OnGetDrawCamera();
				//進行方向の向きを計算
				Vec3 front = ptrTransform->GetPosition() - ptrCamera->GetEye();
				front.y = 0;
				front.normalize();
				//進行方向向きからの角度を算出
				float FrontAngle = atan2(front.z, front.x);
				//コントローラの向き計算
				float MoveX = cntlVec[0].fThumbLX;
				float MoveZ = cntlVec[0].fThumbLY;
				Vec2 MoveVec(MoveX, MoveZ);
				float MoveSize = MoveVec.length();
				//コントローラの向きから角度を計算
				float CntlAngle = atan2(-MoveX, MoveZ);
				//トータルの角度を算出
				float TotalAngle = FrontAngle + CntlAngle;
				//角度からベクトルを作成
				angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
				//正規化する
				angle.normalize();
				//移動サイズを設定。
				angle *= MoveSize;
				//Y軸は変化させない
				angle.y = 0;
			}
		}
		return angle;
	}


	//初期化
	void Player::OnCreate() {
		//初期位置などの設定
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(m_Scale));	//直径25センチの球体
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		auto bkCamera = App::GetApp()->GetScene<Scene>()->GetBackupCamera();
		Vec3 firstPos;
		if (!bkCamera) {
			firstPos = Vec3(0, m_Scale * 0.5f, 0);
		}
		else {
			firstPos = App::GetApp()->GetScene<Scene>()->GetBackupPlayerPos();
		}
		ptrTrans->SetPosition(firstPos);
		//WorldMatrixをもとにRigidbodySphereのパラメータを作成
		PsSphereParam param(ptrTrans->GetWorldMatrix(),1.0f,false, PsMotionType::MotionTypeActive);
		//RigidbodySphereコンポーネントを追加
		auto psPtr = AddComponent<RigidbodySphere>(param);
		//自動的にTransformを設定するフラグは無し
		psPtr->SetAutoTransform(false);

		//文字列をつける
		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");
		ptrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		//影をつける（シャドウマップを描画する）
		auto ptrShadow = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");
		//描画コンポーネントの設定
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//描画するテクスチャを設定
		ptrDraw->SetTextureResource(L"TRACE_TX");

		//透明処理
		SetAlphaActive(true);
		//カメラを得る
		auto ptrCamera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (ptrCamera) {
			//MyCameraである
			//MyCameraに注目するオブジェクト（プレイヤー）の設定
			ptrCamera->SetTargetObject(GetThis<GameObject>());
			ptrCamera->SetTargetToAt(Vec3(0, 0.25f, 0));
		}
	}

	//更新
	void Player::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		m_InputHandler.PushHandle(GetThis<Player>());
		auto vec = GetMoveVector();
		auto ptrPs = GetComponent<RigidbodySphere>();
		auto velo = ptrPs->GetLinearVelocity();
		//xとzの速度を修正
		velo.x = vec.x * 5.0f;
		velo.z = vec.z * 5.0f;
		//速度を設定
		ptrPs->SetLinearVelocity(velo);
	}

	//後更新
	void Player::OnUpdate2() {
		//RigidbodySphereからTransformへのパラメータの設定
		//自動的に設定はされない設定になっているので自分で行う
		auto ptrPs = GetComponent<RigidbodySphere>();
		auto ptrTrans = GetComponent<Transform>();
		//位置情報はそのまま設定
		ptrTrans->SetPosition(ptrPs->GetPosition());
		//回転の計算
		Vec3 angle = GetMoveVector();
		if (angle.length() > 0.0f) {
			//補間処理を行う回転。
			RotToHead(angle, 0.1f);
		}
		//文字列の表示
		DrawStrings();
	}

	//Aボタンハンドラ
	void  Player::OnPushA() {
		auto ptrTrans = GetComponent<Transform>();
		if (ptrTrans->GetPosition().y > 0.125f) {
			//地面についてなければジャンプしない
			return;
		}
		auto ptrPs = GetComponent<RigidbodySphere>();
		auto velo = ptrPs->GetLinearVelocity();
		velo += Vec3(0, 4.0f, 0.0);
		ptrPs->SetLinearVelocity(velo);
	}

	//Bボタンハンドラ
	void  Player::OnPushB() {
		//ゲームステージ再読み込み
		App::GetApp()->GetScene<Scene>()->SetBackupCamera(dynamic_pointer_cast<MyCamera>(GetStage()->GetView()->GetTargetCamera()));
		App::GetApp()->GetScene<Scene>()->SetBackupPlayerPos(GetComponent<Transform>()->GetPosition());
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
	}

	//文字列の表示
	void Player::DrawStrings() {

		//文字列表示
		wstring strMess(L"Bボタンで再読み込み\n");
		//オブジェクト数
		auto ObjCount = GetStage()->GetGameObjectVec().size();
		wstring  strObjCount(L"OBJ_COUNT: ");
		strObjCount += Util::SizeTToWStr(ObjCount);
		strObjCount += L"\n";
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring strFps(L"FPS: ");
		strFps += Util::UintToWStr(fps);
		strFps += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		strFps += Util::FloatToWStr(ElapsedTime);
		strFps += L"\n";

		auto pos = GetComponent<Transform>()->GetPosition();
		wstring strPos(L"Position:\t");
		strPos += L"X=" + Util::FloatToWStr(pos.x, 6, Util::FloatModify::Fixed) + L",\t";
		strPos += L"Y=" + Util::FloatToWStr(pos.y, 6, Util::FloatModify::Fixed) + L",\t";
		strPos += L"Z=" + Util::FloatToWStr(pos.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring str = strMess + strObjCount + strFps + strPos;
		//文字列をつける
		auto ptrString = GetComponent<StringSprite>();
		ptrString->SetText(str);
	}

}
//end basecross

