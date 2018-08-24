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
	//構築と破棄
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Velocity(0)
	{}

	Vec3 Player::GetMoveVector() const {
		Vec3 Angle(0, 0, 0);
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//動いた時のスピード
				auto PtrTransform = GetComponent<Transform>();
				auto PtrCamera = OnGetDrawCamera();
				//進行方向の向きを計算
				Vec3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
				Front.y = 0;
				Front.normalize();
				//進行方向向きからの角度を算出
				float FrontAngle = atan2(Front.z, Front.x);
				//コントローラの向き計算
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				Vec2 MoveVec(MoveX, MoveZ);
				float MoveSize = MoveVec.length();
				//コントローラの向きから角度を計算
				float CntlAngle = atan2(-MoveX, MoveZ);
				//トータルの角度を算出
				float TotalAngle = FrontAngle + CntlAngle;
				//角度からベクトルを作成
				Angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
				//正規化する
				Angle.normalize();
				//移動サイズを設定。
				Angle *= MoveSize;
				//Y軸は変化させない
				Angle.y = 0;
			}
		}
		return Angle;
	}

	void Player::MovePlayer() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vec3 Angle = GetMoveVector();
		if (Angle.length() > 0.0f) {
			auto Pos = GetComponent<Transform>()->GetPosition();
			Pos += Angle * ElapsedTime *6.0f;
			GetComponent<Transform>()->SetPosition(Pos);
		}
		//回転の計算
		if (Angle.length() > 0.0f) {
			auto UtilPtr = GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(Angle, 1.0f);
		}
	}

	//初期化
	void Player::OnCreate() {

		//初期位置などの設定
		auto Ptr = AddComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.125f, 0);

		//CollisionSphere衝突判定を付ける
		auto PtrColl = AddComponent<CollisionSphere>();
		//重力をつける
		auto PtrGra = AddComponent<Gravity>();


		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetFogEnabled(true);
		//描画するテクスチャを設定
		PtrDraw->SetTextureResource(L"TRACE_TX");
		SetAlphaActive(true);

		//カメラを得る
		auto PtrCamera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (PtrCamera) {
			//MyCameraである
			//MyCameraに注目するオブジェクト（プレイヤー）の設定
			PtrCamera->SetTargetObject(GetThis<GameObject>());
			PtrCamera->SetTargetToAt(Vec3(0, 0.25f, 0));
		}
	}

	//更新
	void Player::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		m_InputHandler.PushHandle(GetThis<Player>());
		MovePlayer();
		DrawStrings();
	}

	void Player::OnUpdate2() {
		//文字列の表示
//		DrawStrings();
	}

	//Aボタン
	void Player::OnPushA() {
		auto Grav = GetComponent<Gravity>();
		Grav->StartJump(Vec3(0,4.0f,0));
	}

	//文字列の表示
	void Player::DrawStrings() {

		//文字列表示
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		FPS += Util::FloatToWStr(ElapsedTime);
		FPS += L"\n";

		auto Pos = GetComponent<Transform>()->GetPosition();
		wstring PositionStr(L"Position:\t");
		PositionStr += L"X=" + Util::FloatToWStr(Pos.x, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Y=" + Util::FloatToWStr(Pos.y, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Z=" + Util::FloatToWStr(Pos.z, 6, Util::FloatModify::Fixed) + L"\n";

		auto Velo = GetComponent<Transform>()->GetVelocity();
		wstring VelocityStr(L"Velocity:\t");
		VelocityStr += L"X=" + Util::FloatToWStr(Velo.x, 6, Util::FloatModify::Fixed) + L",\t";
		VelocityStr += L"Y=" + Util::FloatToWStr(Velo.y, 6, Util::FloatModify::Fixed) + L",\t";
		VelocityStr += L"Z=" + Util::FloatToWStr(Velo.z, 6, Util::FloatModify::Fixed) + L"\n";


		wstring GravStr(L"GravityVelocoty:\t");
		auto GravVelocity = GetComponent<Gravity>()->GetGravityVelocity();
		GravStr += L"X=" + Util::FloatToWStr(GravVelocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Y=" + Util::FloatToWStr(GravVelocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Z=" + Util::FloatToWStr(GravVelocity.z, 6, Util::FloatModify::Fixed) + L"\n";
		wstring str = FPS + PositionStr + VelocityStr + GravStr;
		//文字列コンポーネントの取得
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}

}
//end basecross

