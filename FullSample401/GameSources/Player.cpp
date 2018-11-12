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
		auto ptrTrans = GetComponent<Transform>();
		//回転の更新
		if (Velocity.length() > 0.0f) {
			Vec3 temp = Velocity;
			temp.normalize();
			float toAngle = atan2(temp.x, temp.z);
			Quat qt;
			qt.rotationRollPitchYawFromVector(Vec3(0, toAngle, 0));
			qt.normalize();
			//現在の回転を取得
			Quat nowQt = ptrTrans->GetQuaternion();
			//現在と目標を補間
			if (LerpFact >= 1.0f) {
				nowQt = qt;
			}
			else {
				//クオータニオンの補間処理
				nowQt = XMQuaternionSlerp(nowQt, qt, LerpFact);
			}
			ptrTrans->SetQuaternion(nowQt);
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

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetAfterCollision(AfterCollision::None);

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
		m_InputHandler.ButtonHandle(GetThis<Player>());
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
		vector<shared_ptr<GameObject>> objects;
		ptrPs->GetContactGameObjects(objects);

	}

	//Aボタンハンドラ
	void  Player::OnPushA() {
		auto ptrTrans = GetComponent<Transform>();
		if (ptrTrans->GetPosition().y > 0.125f) {
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

	//Xボタンハンドラ
	void Player::OnPushX() {
		auto ptrTrans = GetComponent<Transform>();
		Vec3 pos = ptrTrans->GetPosition();
		pos.y += 0.5f;
		Quat qt = ptrTrans->GetQuaternion();
		Vec3 rot = qt.toRotVec();
		float rotY = rot.y;
		Vec3 velo(sin(rotY), 0.05f, cos(rotY));
		velo.normalize();
		velo *= 20.0f;

		auto ptrSh = GetStage()->GetSharedGameObject<FireSphere>(L"FireSphere", false);
		if (ptrSh) {
			ptrSh->Reset(pos, velo);
		}
		else {
			GetStage()->AddGameObject<FireSphere>(pos, velo);
		}
	}

	//Yボタンハンドラ(押した瞬間)
	void Player::OnPushY() {
		//ホールドしたオブジェクトがなければ何もしない
		auto ptrHold = m_HoldObject.lock();
		if (!ptrHold) {
			return;
		}
		auto ptrActionLine = m_ActionLine.lock();
		if (ptrActionLine) {
			auto Check = ptrActionLine->GetEndObj();
			auto CheckHold = dynamic_pointer_cast<GameObject>(ptrHold);
			if (Check != CheckHold) {
				ptrActionLine->SetEndObj(ptrHold);
			}
			ptrActionLine->SetDrawActive(true);
		}
		else {
			//ラインの作成
			auto ptrLine = GetStage()->AddGameObject<ActionLine>(GetThis<GameObject>(), ptrHold);
			ptrLine->SetDrawActive(true);
			m_ActionLine = ptrLine;
		}
	}


	//Yボタンハンドラ(押し続け)
	void Player::OnPressY() {
		auto ptrTrans = GetComponent<Transform>();
		auto playerPos = ptrTrans->GetPosition();
		auto ptrHold = m_HoldObject.lock();
		if (ptrHold) {
			auto ptrPs = ptrHold->GetDynamicComponent<RigidbodySingle>(false);
			if (ptrPs) {
				auto psPos = ptrPs->GetPosition();
				float toY = 2.0f;
				if (psPos.y > 5.0f) {
					toY = 0.0f;
				}
				psPos.y = 0;
				playerPos.y = 0;
				Vec3 toPlayerVec = playerPos - psPos;
				ptrPs->WakeUp();
				ptrPs->SetLinearVelocity(Vec3(toPlayerVec.x, toY, toPlayerVec.z));
			}
		}
	}

	//Yボタンハンドラ(離した瞬間)
	void Player::OnReleaseY() {
		auto ptrActionLine = m_ActionLine.lock();
		if (ptrActionLine) {
			ptrActionLine->SetDrawActive(false);
		}
	}

	//文字列の表示
	void Player::DrawStrings() {

		//文字列表示
		wstring mess(L"Bボタンで再読み込み\nXボタンで発射\nホールド後Yボタンで運搬\n");
		//オブジェクト数
		auto objCount = GetStage()->GetGameObjectVec().size();
		wstring objCountStr(L"OBJ_COUNT: ");
		objCountStr += Util::SizeTToWStr(objCount);
		objCountStr += L"\n";
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring fpsStr(L"FPS: ");
		fpsStr += Util::UintToWStr(fps);
		fpsStr += L"\nElapsedTime: ";
		float elapsedTime = App::GetApp()->GetElapsedTime();
		fpsStr += Util::FloatToWStr(elapsedTime);
		fpsStr += L"\n";

		auto pos = GetComponent<Transform>()->GetPosition();
		wstring positionStr(L"Position:\t");
		positionStr += L"X=" + Util::FloatToWStr(pos.x, 6, Util::FloatModify::Fixed) + L",\t";
		positionStr += L"Y=" + Util::FloatToWStr(pos.y, 6, Util::FloatModify::Fixed) + L",\t";
		positionStr += L"Z=" + Util::FloatToWStr(pos.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring str = mess + objCountStr + fpsStr + positionStr;
		//文字列をつける
		auto ptrString = GetComponent<StringSprite>();
		ptrString->SetText(str);
	}

}
//end basecross

