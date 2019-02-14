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
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}

	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);
		//�R���g���[���̎擾
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (cntlVec[0].bConnected) {
			if (cntlVec[0].fThumbLX != 0 || cntlVec[0].fThumbLY != 0) {
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
				float moveX = cntlVec[0].fThumbLX;
				float moveZ = cntlVec[0].fThumbLY;
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
		}
		return angle;
	}

	void Player::MovePlayer() {
		float elapsedTime = App::GetApp()->GetElapsedTime();
		auto angle = GetMoveVector();
		if (angle.length() > 0.0f) {
			auto pos = GetComponent<Transform>()->GetPosition();
			pos += angle * elapsedTime * 6.0f;
			GetComponent<Transform>()->SetPosition(pos);
		}
		//��]�̌v�Z
		if (angle.length() > 0.0f) {
			auto utilPtr = GetBehavior<UtilBehavior>();
			utilPtr->RotToHead(angle, 1.0f);
		}
	}

	void Player::ChkModelCollision() {
		auto ptrTrans = GetComponent<Transform>();
		SPHERE startSp(ptrTrans->GetBeforePosition(), 0.125f);
		SPHERE endSp(ptrTrans->GetPosition(), 0.125f);
		auto EnemyPtr = GetStage()->GetSharedGameObject<EnemyBox>(L"EnemyBox");
		Vec3 HitPoint;
		TRIANGLE tri;
		if (EnemyPtr->IsHitSphereTriangles(startSp, endSp, tri, HitPoint)) {
			//�X�p�[�N�̕��o
			auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark");
			PtrSpark->InsertSpark(HitPoint);
		}
		else {
			auto BonePtr = GetStage()->GetSharedGameObject<BoneChara>(L"BoneChara");
			if (BonePtr->IsHitSphereTriangles(startSp, endSp, tri, HitPoint)) {
				//�X�p�[�N�̕��o
				auto PtrSpark = GetStage()->GetSharedGameObject<MultiSpark>(L"MultiSpark");
				PtrSpark->InsertSpark(HitPoint);
			}
		}
	}


	//������
	void Player::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(0, 0.125f, 0);
		//CollisionSphere�Փ˔����t����
		auto ptrColl = AddComponent<CollisionSphere>();
		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();
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
		ChkModelCollision();
		MovePlayer();
	}

	void Player::OnUpdate2() {
		//������̕\��
		DrawStrings();
	}

	//A�{�^��
	void Player::OnPushA() {
		auto grav = GetComponent<Gravity>();
		grav->StartJump(Vec3(0,4.0f,0));
	}

	//X�{�^��
	void Player::OnPushX() {
		auto ptrTrans = GetComponent<Transform>();
		Vec3 pos = ptrTrans->GetPosition();
		pos.y += 0.5f;
		Quat qt = ptrTrans->GetQuaternion();
		Vec3 rot = qt.toRotVec();
		float rotY = rot.y;
		Vec3 velo(sin(rotY), 0.1f, cos(rotY));
		velo.normalize();
		velo *= 20.0f;
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


	//������̕\��
	void Player::DrawStrings() {
		//������\��
		//�I�u�W�F�N�g��
		auto objCount = GetStage()->GetGameObjectVec().size();
		wstring objCountStr(L"OBJ_COUNT: ");
		objCountStr += Util::SizeTToWStr(objCount);
		objCountStr += L"\n";
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring fpsStr(L"FPS: ");
		fpsStr += Util::UintToWStr(fps);
		fpsStr += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		fpsStr += Util::FloatToWStr(ElapsedTime);
		fpsStr += L"\n";

		auto pos = GetComponent<Transform>()->GetPosition();
		wstring positionStr(L"Position:\t");
		positionStr += L"X=" + Util::FloatToWStr(pos.x, 6, Util::FloatModify::Fixed) + L",\t";
		positionStr += L"Y=" + Util::FloatToWStr(pos.y, 6, Util::FloatModify::Fixed) + L",\t";
		positionStr += L"Z=" + Util::FloatToWStr(pos.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring gravStr(L"GravityVelocoty:\t");
		auto gravVelocity = GetComponent<Gravity>()->GetGravityVelocity();
		gravStr += L"X=" + Util::FloatToWStr(gravVelocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		gravStr += L"Y=" + Util::FloatToWStr(gravVelocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		gravStr += L"Z=" + Util::FloatToWStr(gravVelocity.z, 6, Util::FloatModify::Fixed) + L"\n";
		wstring str = objCountStr + fpsStr + positionStr + gravStr;
		//������R���|�[�l���g�̎擾
		auto ptrString = GetComponent<StringSprite>();
		ptrString->SetText(str);
	}

}
//end basecross
