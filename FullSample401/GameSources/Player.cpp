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
	//�\�z
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Scale(0.25f)
	{}

	void Player::RotToHead(const Vec3& Velocity, float LerpFact) {
		if (LerpFact <= 0.0f) {
			//��ԌW����0�ȉ��Ȃ牽�����Ȃ�
			return;
		}
		auto ptrTrans = GetComponent<Transform>();
		//��]�̍X�V
		if (Velocity.length() > 0.0f) {
			Vec3 temp = Velocity;
			temp.normalize();
			float toAngle = atan2(temp.x, temp.z);
			Quat qt;
			qt.rotationRollPitchYawFromVector(Vec3(0, toAngle, 0));
			qt.normalize();
			//���݂̉�]���擾
			Quat nowQt = ptrTrans->GetQuaternion();
			//���݂ƖڕW����
			if (LerpFact >= 1.0f) {
				nowQt = qt;
			}
			else {
				//�N�I�[�^�j�I���̕�ԏ���
				nowQt = XMQuaternionSlerp(nowQt, qt, LerpFact);
			}
			ptrTrans->SetQuaternion(nowQt);
		}
	}


	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);
		//�R���g���[���̎擾
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (cntlVec[0].bConnected) {
			if (cntlVec[0].fThumbLX != 0 || cntlVec[0].fThumbLY != 0) {
				auto ptrTransform = GetComponent<Transform>();
				auto ptrCamera = OnGetDrawCamera();
				//�i�s�����̌������v�Z
				Vec3 front = ptrTransform->GetPosition() - ptrCamera->GetEye();
				front.y = 0;
				front.normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(front.z, front.x);
				//�R���g���[���̌����v�Z
				float MoveX = cntlVec[0].fThumbLX;
				float MoveZ = cntlVec[0].fThumbLY;
				Vec2 MoveVec(MoveX, MoveZ);
				float MoveSize = MoveVec.length();
				//�R���g���[���̌�������p�x���v�Z
				float CntlAngle = atan2(-MoveX, MoveZ);
				//�g�[�^���̊p�x���Z�o
				float TotalAngle = FrontAngle + CntlAngle;
				//�p�x����x�N�g�����쐬
				angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
				//���K������
				angle.normalize();
				//�ړ��T�C�Y��ݒ�B
				angle *= MoveSize;
				//Y���͕ω������Ȃ�
				angle.y = 0;
			}
		}
		return angle;
	}


	//������
	void Player::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->SetScale(Vec3(m_Scale));	//���a25�Z���`�̋���
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
		//WorldMatrix�����Ƃ�RigidbodySphere�̃p�����[�^���쐬
		PsSphereParam param(ptrTrans->GetWorldMatrix(),1.0f,false, PsMotionType::MotionTypeActive);
		//RigidbodySphere�R���|�[�l���g��ǉ�
		auto psPtr = AddComponent<RigidbodySphere>(param);
		//�����I��Transform��ݒ肷��t���O�͖���
		psPtr->SetAutoTransform(false);

		auto ptrCol = AddComponent<CollisionSphere>();
		ptrCol->SetAfterCollision(AfterCollision::None);

		//�����������
		auto ptrString = AddComponent<StringSprite>();
		ptrString->SetText(L"");
		ptrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ptrShadow = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");
		//�`��R���|�[�l���g�̐ݒ�
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//�`�悷��e�N�X�`����ݒ�
		ptrDraw->SetTextureResource(L"TRACE_TX");

		//��������
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
		m_InputHandler.ButtonHandle(GetThis<Player>());
		auto vec = GetMoveVector();
		auto ptrPs = GetComponent<RigidbodySphere>();
		auto velo = ptrPs->GetLinearVelocity();
		//x��z�̑��x���C��
		velo.x = vec.x * 5.0f;
		velo.z = vec.z * 5.0f;
		//���x��ݒ�
		ptrPs->SetLinearVelocity(velo);
	}

	//��X�V
	void Player::OnUpdate2() {
		//RigidbodySphere����Transform�ւ̃p�����[�^�̐ݒ�
		//�����I�ɐݒ�͂���Ȃ��ݒ�ɂȂ��Ă���̂Ŏ����ōs��
		auto ptrPs = GetComponent<RigidbodySphere>();
		auto ptrTrans = GetComponent<Transform>();
		//�ʒu���͂��̂܂ܐݒ�
		ptrTrans->SetPosition(ptrPs->GetPosition());
		//��]�̌v�Z
		Vec3 angle = GetMoveVector();
		if (angle.length() > 0.0f) {
			//��ԏ������s����]�B
			RotToHead(angle, 0.1f);
		}
		//������̕\��
		DrawStrings();
		vector<shared_ptr<GameObject>> objects;
		ptrPs->GetContactGameObjects(objects);

	}

	//A�{�^���n���h��
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

	//B�{�^���n���h��
	void  Player::OnPushB() {
		//�Q�[���X�e�[�W�ēǂݍ���
		App::GetApp()->GetScene<Scene>()->SetBackupCamera(dynamic_pointer_cast<MyCamera>(GetStage()->GetView()->GetTargetCamera()));
		App::GetApp()->GetScene<Scene>()->SetBackupPlayerPos(GetComponent<Transform>()->GetPosition());
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
	}

	//X�{�^���n���h��
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

	//Y�{�^���n���h��(�������u��)
	void Player::OnPushY() {
		//�z�[���h�����I�u�W�F�N�g���Ȃ���Ή������Ȃ�
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
			//���C���̍쐬
			auto ptrLine = GetStage()->AddGameObject<ActionLine>(GetThis<GameObject>(), ptrHold);
			ptrLine->SetDrawActive(true);
			m_ActionLine = ptrLine;
		}
	}


	//Y�{�^���n���h��(��������)
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

	//Y�{�^���n���h��(�������u��)
	void Player::OnReleaseY() {
		auto ptrActionLine = m_ActionLine.lock();
		if (ptrActionLine) {
			ptrActionLine->SetDrawActive(false);
		}
	}

	//������̕\��
	void Player::DrawStrings() {

		//������\��
		wstring mess(L"B�{�^���ōēǂݍ���\nX�{�^���Ŕ���\n�z�[���h��Y�{�^���ŉ^��\n");
		//�I�u�W�F�N�g��
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
		//�����������
		auto ptrString = GetComponent<StringSprite>();
		ptrString->SetText(str);
	}

}
//end basecross

