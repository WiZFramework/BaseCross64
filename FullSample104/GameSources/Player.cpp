/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Player::Player(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos)
	{}

	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);
		//�R���g���[���̎擾
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		float fThumbLY = 0.0f;
		float fThumbLX = 0.0f;
		WORD wButtons = 0;
		if (cntlVec[0].bConnected) {
			fThumbLY = cntlVec[0].fThumbLY;
			fThumbLX = cntlVec[0].fThumbLX;
			wButtons = cntlVec[0].wButtons;
		}
		//�L�[�{�[�h�̎擾(�L�[�{�[�h�D��)
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPushKeyTbl['W']) {
			//�O
			fThumbLY = 1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['A']) {
			//��
			fThumbLX = -1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['S']) {
			//���
			fThumbLY = -1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['D']) {
			//�E
			fThumbLX = 1.0f;
		}
		if (fThumbLX != 0 || fThumbLY != 0) {
			float moveLength = 0;	//���������̃X�s�[�h
			auto ptrTransform = GetComponent<Transform>();
			auto ptrCamera = OnGetDrawCamera();
			//�i�s�����̌������v�Z
			auto front = ptrTransform->GetPosition() - ptrCamera->GetEye();
			front.y = 0;
			front.normalize();
			//�i�s������������̊p�x���Z�o
			float frontAngle = atan2(front.z, front.x);
			//�R���g���[���̌����v�Z
			float moveX = fThumbLX;
			float moveZ = fThumbLY;
			Vec2 moveVec(moveX, moveZ);
			float moveSize = moveVec.length();
			//�R���g���[���̌�������p�x���v�Z
			float cntlAngle = atan2(-moveX, moveZ);
			//�g�[�^���̊p�x���Z�o
			float totalAngle = frontAngle + cntlAngle;
			//�p�x����x�N�g�����쐬
			angle = Vec3(cos(totalAngle), 0, sin(totalAngle));
			//���K������
			angle.normalize();
			//�ړ��T�C�Y��ݒ�B
			angle *= moveSize;
			//Y���͕ω������Ȃ�
			angle.y = 0;
		}
		return angle;
	}

	void Player::MovePlayer() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		auto angle = GetMoveVector();
		auto Vec = GetMoveVector();
		auto PtrPs = GetComponent<RigidbodySphere>();
		auto Velo = PtrPs->GetLinearVelocity();
		Velo.x = Vec.x * 5.0f;
		Velo.z = Vec.z * 5.0f;
		PtrPs->SetLinearVelocity(Velo);
		//��]�̌v�Z
		if (angle.length() > 0.0f) {
			auto utilPtr = GetBehavior<UtilBehavior>();
			utilPtr->RotToHead(angle, 1.0f);
		}
	}

	//������
	void Player::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptrTrans = AddComponent<Transform>();
		ptrTrans->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		ptrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		ptrTrans->SetPosition(0, 0.125f, 0);
		//CollisionSphere�Փ˔����t����
		auto PtrCol = AddComponent<CollisionSphere>();
		//���肷�邾���Ȃ̂ŃA�N�V������None
		PtrCol->SetAfterCollision(AfterCollision::None);

		//WorldMatrix�����Ƃ�RigidbodySphere�̃p�����[�^���쐬
		PsSphereParam param(ptrTrans->GetWorldMatrix(), 1.0f, false, PsMotionType::MotionTypeActive);
		//RigidbodySphere�R���|�[�l���g��ǉ�
		auto psPtr = AddComponent<RigidbodySphere>(param);
		//�����I��Transform��ݒ肷��t���O�͖���
		psPtr->SetAutoTransform(false);
		psPtr->SetDrawActive(true);
		//�����������
		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");
		ptrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto shadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		shadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//�`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetFogEnabled(true);
		//�`�悷��e�N�X�`����ݒ�
		ptrDraw->SetTextureResource(L"TRACE_TX");
		SetAlphaActive(true);
		//�J�����𓾂�
		auto ptrCamera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (ptrCamera) {
			//MyCamera�ł���
			//MyCamera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			ptrCamera->SetTargetObject(GetThis<GameObject>());
			ptrCamera->SetTargetToAt(Vec3(0, 0.25f, 0));
		}
	}

	//�X�V
	void Player::OnUpdate() {
		//�R���g���[���`�F�b�N���ē��͂�����΃R�}���h�Ăяo��
		m_InputHandler.PushHandle(GetThis<Player>());
		MovePlayer();
	}

	void Player::OnUpdate2() {
		auto PtrPs = GetComponent<RigidbodySphere>();
		auto Ptr = GetComponent<Transform>();
		Ptr->SetPosition(PtrPs->GetPosition());
		//��]�̌v�Z
		Vec3 Angle = GetMoveVector();
		if (Angle.length() > 0.0f) {
			auto UtilPtr = GetBehavior<UtilBehavior>();
			//��ԏ������s��Ȃ���]�B��ԏ�������ɂ͈ȉ�1.0��0.1�Ȃǂɂ���
			UtilPtr->RotToHead(Angle, 1.0f);
		}
		//������̕\��
		DrawStrings();
	}

	//A�{�^��
	void Player::OnPushA() {
		auto Ptr = GetComponent<Transform>();
		auto PtrPs = GetComponent<RigidbodySphere>();
		auto Velo = PtrPs->GetLinearVelocity();
		Velo += Vec3(0, 4.0f, 0.0);
		PtrPs->SetLinearVelocity(Velo);
	}

	//X�{�^��
	void Player::OnPushX() {
		auto ptrTrans = GetComponent<Transform>();
		Vec3 pos = ptrTrans->GetPosition();
		pos.y += 0.3f;
		Quat qt = ptrTrans->GetQuaternion();
		Vec3 rot = qt.toRotVec();
		float rotY = rot.y;
		Vec3 velo(sin(rotY), 0.1f, cos(rotY));
		velo.normalize();
		velo *= 15.0f;
		auto group = GetStage()->GetSharedObjectGroup(L"FireSphereGroup");
		auto& vec = group->GetGroupVector();
		for (auto& v : vec) {
			auto shObj = v.lock();
			if (shObj) {
				if (!shObj->IsUpdateActive()) {
					auto shFire = dynamic_pointer_cast<FireSphere>(shObj);
					if (shFire) {
						shFire->Reset(pos, velo);
						return;
					}
				}
			}
		}
		//�󂪂Ȃ������̂ŐV���ɍ쐬
		GetStage()->AddGameObject<FireSphere>(pos, velo);
	}

	//�C�e�����������s��
	void Player::FirShellHitBehavior(const Vec3& HitVelocity) {
		auto PsPtr = GetComponent<RigidbodySphere>();
		PsPtr->WakeUp();
		Vec3 HitVelo = HitVelocity;
		HitVelo.y = 5.0f;
		PsPtr->SetLinearVelocity(HitVelo);
		//�T�E���h�̍Đ�
		auto ptrXA = App::GetApp()->GetXAudio2Manager();
		ptrXA->Start(L"cursor", 0, 0.5f);
	}

	//������̕\��
	void Player::DrawStrings() {

		//������\��
		wstring Mess(L"�R���g���[��: A�ŃW�����v�AX�{�^���Ŕ���\n");
		Mess += L"�L�[�{�[�h: WASD�ňړ��ASpace�ŃW�����v�AX�Ŕ��ˁB���,PageUP,Down: �J����\n";
		Mess += L"�}�E�X: �N���b�N�ŕ����I�u�W�F�N�g����\n";
		//�I�u�W�F�N�g��
		auto ObjCount = GetStage()->GetGameObjectVec().size();
		wstring OBJ_COUNT(L"OBJ_COUNT: ");
		OBJ_COUNT += Util::UintToWStr((UINT)ObjCount);
		OBJ_COUNT += L"\n";
		//�����I�u�W�F�N�g��
		wstring PS_OBJ_COUNT(L"PS_OBJ_COUNT: ");
		PS_OBJ_COUNT += Util::UintToWStr(GetStage()->GetBasePhysics().GetNumBodies());
		PS_OBJ_COUNT += L"\n";
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

		wstring MousePosStr(L"MousePos:\t");
		auto MousePos = GetTypeStage<GameStage>()->GetMousePoint();
		MousePosStr += L"X=" + Util::IntToWStr(MousePos.x) + L",\t";
		MousePosStr += L"Y=" + Util::IntToWStr(MousePos.y) + L"\n";

		Vec3 Near; Vec3 Far;
		GetTypeStage<GameStage>()->GetMouseRay(Near, Far);

		wstring MouseRayNearStr(L"MouseRayNear:\t");
		MouseRayNearStr += L"X=" + Util::FloatToWStr(Near.x, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayNearStr += L"Y=" + Util::FloatToWStr(Near.y, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayNearStr += L"Z=" + Util::FloatToWStr(Near.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring MouseRayFarStr(L"MouseRayFar:\t");
		MouseRayFarStr += L"X=" + Util::FloatToWStr(Far.x, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayFarStr += L"Y=" + Util::FloatToWStr(Far.y, 6, Util::FloatModify::Fixed) + L",\t";
		MouseRayFarStr += L"Z=" + Util::FloatToWStr(Far.z, 6, Util::FloatModify::Fixed) + L"\n";



		wstring str = Mess + OBJ_COUNT + PS_OBJ_COUNT + FPS + PositionStr + MousePosStr + MouseRayNearStr + MouseRayFarStr;
		//�����������
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}

}
//end basecross

