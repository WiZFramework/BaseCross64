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
		GameObject(StagePtr),
		m_Speed(6.0f)
	{}

	Vec2 Player::GetInputState() const {
		Vec2 ret;
		//�R���g���[���̎擾
		auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		ret.x = 0.0f;
		ret.y = 0.0f;
		WORD wButtons = 0;
		if (cntlVec[0].bConnected) {
			ret.x = cntlVec[0].fThumbLX;
			ret.y = cntlVec[0].fThumbLY;
		}
		//�L�[�{�[�h�̎擾(�L�[�{�[�h�D��)
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPushKeyTbl['W']) {
			//�O
			ret.y = 1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['A']) {
			//��
			ret.x = -1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['S']) {
			//���
			ret.y = -1.0f;
		}
		else if (KeyState.m_bPushKeyTbl['D']) {
			//�E
			ret.x = 1.0f;
		}
		return ret;
	}


	Vec3 Player::GetMoveVector() const {
		Vec3 angle(0, 0, 0);
		//���͂̎擾
		auto inPut = GetInputState();
		float moveX = inPut.x;
		float moveZ = inPut.y;
		if (moveX != 0 || moveZ != 0) {
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
		if (angle.length() > 0.0f) {
			auto pos = GetComponent<Transform>()->GetPosition();
			pos += angle * elapsedTime * m_Speed;
			GetComponent<Transform>()->SetPosition(pos);
		}
		//��]�̌v�Z
		if (angle.length() > 0.0f) {
			auto utilPtr = GetBehavior<UtilBehavior>();
			utilPtr->RotToHead(angle, 1.0f);
		}
	}

	//������
	void Player::OnCreate() {

		//�����ʒu�Ȃǂ̐ݒ�
		auto ptr = AddComponent<Transform>();
		ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		ptr->SetRotation(0.0f, 0.0f, 0.0f);
		ptr->SetPosition(Vec3(0, 0.125f, 0));

		//CollisionSphere�Փ˔����t����
		auto ptrColl = AddComponent<CollisionSphere>();

		//�e�p�t�H�[�}���X�𓾂�
		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();

		GetStage()->SetCollisionPerformanceActive(true);
		GetStage()->SetUpdatePerformanceActive(true);
		GetStage()->SetDrawPerformanceActive(true);

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

	void Player::OnCollisionEnter(shared_ptr<GameObject>& Other) {
		if (Other->FindTag(L"MoveFixedBox")) {
			GetComponent<Transform>()->SetParent(Other);
		}
	}

	void Player::OnCollisionExit(shared_ptr<GameObject>& Other) {
		GetComponent<Transform>()->ClearParent();
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

	//������̕\��
	void Player::DrawStrings() {

		//������\��
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

		wstring updatePerStr(L"UpdatePerformance:\t");
		updatePerStr += Util::FloatToWStr(GetStage()->GetUpdatePerformanceTime());
		updatePerStr += L"\tmillisecond\n";

		wstring drawPerStr(L"DrawPerformance:\t");
		drawPerStr += Util::FloatToWStr(GetStage()->GetDrawPerformanceTime());
		drawPerStr += L"\tmillisecond\n";

		wstring collPerStr(L"CollisionPerform:\t");
		collPerStr += Util::FloatToWStr(GetStage()->GetCollisionPerformanceTime(), 5);
		collPerStr += L"\tmillisecond\n";

		wstring collMiscStr(L"ColMiscPerform:\t");
		collMiscStr += Util::FloatToWStr(GetStage()->GetCollisionManager()->GetMiscPerformanceTime(), 5);
		collMiscStr += L"\tmillisecond\n";

		wstring collTernCountStr(L"CollisionCountOfTern:\t");
		collTernCountStr += Util::UintToWStr(GetStage()->GetCollisionManager()->GetCollisionCountOfTern());
		collTernCountStr += L"\n";
		wstring str = fpsStr + positionStr + gravStr + updatePerStr + drawPerStr + collPerStr + collMiscStr
			+ collTernCountStr;

		//������R���|�[�l���g�̎擾
		auto ptrString = GetComponent<StringSprite>();
		ptrString->SetText(str);
	}

}
//end basecross

