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
		auto PtrTransform = GetComponent<Transform>();
		//��]�̍X�V
		if (Velocity.length() > 0.0f) {
			bsm::Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quat Qt;
			Qt.rotationRollPitchYawFromVector(bsm::Vec3(0, ToAngle, 0));
			Qt.normalize();
			//���݂̉�]���擾
			Quat NowQt = PtrTransform->GetQuaternion();
			//���݂ƖڕW����
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				//�N�I�[�^�j�I���̕�ԏ���
				NowQt = XMQuaternionSlerp(NowQt, Qt, LerpFact);
			}
			PtrTransform->SetQuaternion(NowQt);
		}
	}


	Vec3 Player::GetMoveVector() const {
		Vec3 Angle(0, 0, 0);
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//���������̃X�s�[�h
				auto PtrTransform = GetComponent<Transform>();
				auto PtrCamera = OnGetDrawCamera();
				//�i�s�����̌������v�Z
				Vec3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
				Front.y = 0;
				Front.normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(Front.z, Front.x);
				//�R���g���[���̌����v�Z
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				Vec2 MoveVec(MoveX, MoveZ);
				float MoveSize = MoveVec.length();
				//�R���g���[���̌�������p�x���v�Z
				float CntlAngle = atan2(-MoveX, MoveZ);
				//�g�[�^���̊p�x���Z�o
				float TotalAngle = FrontAngle + CntlAngle;
				//�p�x����x�N�g�����쐬
				Angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
				//���K������
				Angle.normalize();
				//�ړ��T�C�Y��ݒ�B
				Angle *= MoveSize;
				//Y���͕ω������Ȃ�
				Angle.y = 0;
			}
		}
		return Angle;
	}


	//������
	void Player::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(m_Scale));	//���a25�Z���`�̋���
		PtrTrans->SetRotation(0.0f, 0.0f, 0.0f);
		auto bkCamera = App::GetApp()->GetScene<Scene>()->GetBackupCamera();
		Vec3 FirstPos;
		if (!bkCamera) {
			FirstPos = Vec3(0, m_Scale * 0.5f, 0);
		}
		else {
			FirstPos = App::GetApp()->GetScene<Scene>()->GetBackupPlayerPos();
		}
		PtrTrans->SetPosition(FirstPos);
		//WorldMatrix�����Ƃ�RigidbodySphere�̃p�����[�^���쐬
		PsSphereParam param(PtrTrans->GetWorldMatrix(),1.0f,false, PsMotionType::MotionTypeActive);
		//RigidbodySphere�R���|�[�l���g��ǉ�
		auto PsPtr = AddComponent<RigidbodySphere>(param);
		//�����I��Transform��ݒ肷��t���O�͖���
		PsPtr->SetAutoTransform(false);

		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"TRACE_TX");

		//��������
		SetAlphaActive(true);
		//�J�����𓾂�
		auto PtrCamera = dynamic_pointer_cast<MyCamera>(OnGetDrawCamera());
		if (PtrCamera) {
			//MyCamera�ł���
			//MyCamera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			PtrCamera->SetTargetObject(GetThis<GameObject>());
			PtrCamera->SetTargetToAt(Vec3(0, 0.25f, 0));
		}
	}

	//�X�V
	void Player::OnUpdate() {
		//�R���g���[���`�F�b�N���ē��͂�����΃R�}���h�Ăяo��
		m_InputHandler.PushHandle(GetThis<Player>());
		auto Vec = GetMoveVector();
		auto PtrPs = GetComponent<RigidbodySphere>();
		auto Velo = PtrPs->GetLinearVelocity();
		//x��z�̑��x���C��
		Velo.x = Vec.x * 5.0f;
		Velo.z = Vec.z * 5.0f;
		//���x��ݒ�
		PtrPs->SetLinearVelocity(Velo);
	}

	//��X�V
	void Player::OnUpdate2() {
		//RigidbodySphere����Transform�ւ̃p�����[�^�̐ݒ�
		//�����I�ɐݒ�͂���Ȃ��ݒ�ɂȂ��Ă���̂Ŏ����ōs��
		auto PtrPs = GetComponent<RigidbodySphere>();
		auto Ptr = GetComponent<Transform>();
		//�ʒu���͂��̂܂ܐݒ�
		Ptr->SetPosition(PtrPs->GetPosition());
		//��]�̌v�Z
		Vec3 Angle = GetMoveVector();
		if (Angle.length() > 0.0f) {
			//��ԏ������s����]�B
			RotToHead(Angle, 0.1f);
		}
		//������̕\��
		DrawStrings();
	}

	//A�{�^���n���h��
	void  Player::OnPushA() {
		auto Ptr = GetComponent<Transform>();
		if (Ptr->GetPosition().y > 0.125f) {
			//�n�ʂɂ��ĂȂ���΃W�����v���Ȃ�
			return;
		}
		auto PtrPs = GetComponent<RigidbodySphere>();
		auto Velo = PtrPs->GetLinearVelocity();
		Velo += Vec3(0, 4.0f, 0.0);
		PtrPs->SetLinearVelocity(Velo);
	}

	//B�{�^���n���h��
	void  Player::OnPushB() {
		//�Q�[���X�e�[�W�ēǂݍ���
		App::GetApp()->GetScene<Scene>()->SetBackupCamera(dynamic_pointer_cast<MyCamera>(GetStage()->GetView()->GetTargetCamera()));
		App::GetApp()->GetScene<Scene>()->SetBackupPlayerPos(GetComponent<Transform>()->GetPosition());
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
	}

	//������̕\��
	void Player::DrawStrings() {

		//������\��
		wstring Mess(L"B�{�^���ōēǂݍ���\n");
		//�I�u�W�F�N�g��
		auto ObjCount = GetStage()->GetGameObjectVec().size();
		wstring OBJ_COUNT(L"OBJ_COUNT: ");
		OBJ_COUNT += Util::SizeTToWStr(ObjCount);
		OBJ_COUNT += L"\n";
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

		wstring str = Mess + OBJ_COUNT + FPS + PositionStr;
		//�����������
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}

}
//end basecross

