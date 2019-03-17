/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�v���C���[
	//--------------------------------------------------------------------------------------
	//�\�z
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_Scale(0.25f),
		m_HoledIndex(0)
	{}

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
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(Vec3(m_Scale));	//���a25�Z���`�̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		auto bkCamera = App::GetApp()->GetScene<Scene>()->GetBackupCamera();
		Vec3 FirstPos;
		if (!bkCamera) {
			FirstPos = Vec3(0, m_Scale * 0.5f, -2.0f);
		}
		else {
			FirstPos = App::GetApp()->GetScene<Scene>()->GetBackupPlayerPos();
		}
		Ptr->SetPosition(FirstPos);

		PsSphereParam param;
		//basecross�̃X�P�[�����O�͒��a��Ȃ̂ŁA���a��ɂ���
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//�����e���\���̌v�Z
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		//�v���C���[�Ȃ̂ŃX���[�v���Ȃ�
		param.m_UseSleep = false;
		param.m_Quat.identity();
		param.m_Pos = FirstPos;
		param.m_LinearVelocity = Vec3(0);
		auto PsPtr = AddComponent<RigidbodySphere>(param);
		PsPtr->SetAutoTransform(false);
		PsPtr->SetDrawActive(true);

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
		m_InputHandler.ButtonHandle(GetThis<Player>());

		auto Vec = GetMoveVector();
		auto PtrPs = GetComponent<RigidbodySphere>();
		auto Velo = PtrPs->GetLinearVelocity();
		Velo.x = Vec.x * 5.0f;
		Velo.z = Vec.z * 5.0f;
		PtrPs->SetLinearVelocity(Velo);
	}

	//��X�V
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

	//A�{�^���n���h��
	void  Player::OnPushA() {
		auto Ptr = GetComponent<Transform>();
		if (Ptr->GetPosition().y > 0.125f) {
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


	//Y�{�^���n���h��(�������u��)
	void Player::OnPushY() {
		auto PlayerPos = GetComponent<Transform>()->GetPosition();
		auto RagPtr = GetStage()->GetSharedGameObject<Ragdoll>(L"Ragdoll");
		auto MultiPs = RagPtr->GetComponent<RigidbodyMulti>();
		auto EndPos = MultiPs->GetPosition(m_HoledIndex);
		auto PositionLinePtr = m_PositionLine.lock();
		if (PositionLinePtr) {
			//���C���̐擪�̐ݒ�
			PositionLinePtr->SetStartPos(PlayerPos);
			//���C���̏I���̐ݒ�
			PositionLinePtr->SetEndPos(EndPos);
		}
		else {
			//���C���̍쐬
			PositionLinePtr = GetStage()->AddGameObject<PositionLine>(PlayerPos, EndPos);
			m_PositionLine = PositionLinePtr;
		}
		PositionLinePtr->SetDrawActive(true);

		auto RagMngPtr = GetStage()->GetSharedGameObject<RagdollManager>(L"RagdollManager");
		RagMngPtr->SetHold(true);
		RagMngPtr->SetHoldIndex(m_HoledIndex);

	}


	//Y�{�^���n���h��(��������)
	void Player::OnPressY() {
		auto Ptr = GetComponent<Transform>();
		auto PlayerPos = Ptr->GetPosition();

		auto RagPtr = GetStage()->GetSharedGameObject<Ragdoll>(L"Ragdoll");
		auto MultiPs = RagPtr->GetComponent<RigidbodyMulti>();
		auto PsPos = MultiPs->GetPosition(m_HoledIndex);
		auto PositionLinePtr = m_PositionLine.lock();
		if (PositionLinePtr) {
			PositionLinePtr->SetStartPos(PlayerPos);
			PositionLinePtr->SetEndPos(PsPos);

			float ToY = 5.0f;
			if (PsPos.y > 5.0f) {
				ToY = 0.0f;
			}
			PsPos.y = 0;
			PlayerPos.y = 0;
			Vec3 ToPlayerVec = PlayerPos - PsPos;
			MultiPs->WakeUp(m_HoledIndex);
			MultiPs->SetLinearVelocity(Vec3(ToPlayerVec.x, ToY, ToPlayerVec.z), m_HoledIndex);
		}
	}

	//Y�{�^���n���h��(�������u��)
	void Player::OnReleaseY() {
		auto PositionLinePtr = m_PositionLine.lock();
		if (PositionLinePtr) {
			PositionLinePtr->SetDrawActive(false);
			m_HoledIndex++;
			if (m_HoledIndex > 10) {
				m_HoledIndex = 0;
			}
		}
		auto RagMngPtr = GetStage()->GetSharedGameObject<RagdollManager>(L"RagdollManager");
		RagMngPtr->SetHold(false);
	}

	//������̕\��
	void Player::DrawStrings() {
		//������\��
		wstring Mess(L"B�{�^���ōēǂݍ���\nY�{�^���Ń��C��\n");
		//�I�u�W�F�N�g��
		auto ObjCount = GetStage()->GetGameObjectVec().size();
		wstring OBJ_COUNT(L"OBJ_COUNT: ");
		OBJ_COUNT += Util::UintToWStr((UINT)ObjCount);
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

