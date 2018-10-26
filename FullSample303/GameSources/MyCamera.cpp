/*!
@file MyCamera.cpp
@brief �J��������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	MyCamera�J�����i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MyCamera::MyCamera() :
		Camera(),
		m_ToTargetLerp(1.0f),
		m_TargetToAt(0, 0, 0),
		m_RadY(0.5f),
		m_RadXZ(0),
		m_CameraUpDownSpeed(0.5f),
		m_CameraUnderRot(0.1f),
		m_ArmLen(5.0f),
		m_MaxArm(20.0f),
		m_MinArm(2.0f),
		m_RotSpeed(1.0f),
		m_ZoomSpeed(0.1f),
		m_LRBaseMode(true),
		m_UDBaseMode(true)
	{}

	MyCamera::MyCamera(float ArmLen) :
		Camera(),
		m_ToTargetLerp(1.0f),
		m_TargetToAt(0, 0, 0),
		m_RadY(0.5f),
		m_RadXZ(0),
		m_CameraUpDownSpeed(0.5f),
		m_CameraUnderRot(0.1f),
		m_ArmLen(5.0f),
		m_MaxArm(20.0f),
		m_MinArm(2.0f),
		m_RotSpeed(1.0f),
		m_ZoomSpeed(0.1f),
		m_LRBaseMode(true),
		m_UDBaseMode(true)
	{
		m_ArmLen = ArmLen;
		auto Eye = GetEye();
		Eye.y = m_ArmLen;
		SetEye(Eye);
	}

	MyCamera::~MyCamera() {}
	//�A�N�Z�T

	void MyCamera::SetEye(const bsm::Vec3& Eye) {
		Camera::SetEye(Eye);
		UpdateArmLengh();
	}
	void MyCamera::SetEye(float x, float y, float z) {
		Camera::SetEye(x,y,z);
		UpdateArmLengh();
	}


	shared_ptr<GameObject> MyCamera::GetTargetObject() const {
		if (!m_TargetObject.expired()) {
			return m_TargetObject.lock();
		}
		return nullptr;
	}

	void MyCamera::SetTargetObject(const shared_ptr<GameObject>& Obj) {
		m_TargetObject = Obj;
	}

	float MyCamera::GetToTargetLerp() const {
		return m_ToTargetLerp;
	}
	void MyCamera::SetToTargetLerp(float f) {
		m_ToTargetLerp = f;
	}

	float MyCamera::GetArmLengh() const {
		return m_ArmLen;
	}

	void MyCamera::UpdateArmLengh() {
		auto Vec = GetEye() - GetAt();
		m_ArmLen = bsm::length(Vec);
		if (m_ArmLen >= m_MaxArm) {
			//m_MaxArm�ȏ㗣��Ȃ��悤�ɂ���
			m_ArmLen = m_MaxArm;
		}
		if (m_ArmLen <= m_MinArm) {
			//m_MinArm�ȉ��߂Â��Ȃ��悤�ɂ���
			m_ArmLen = m_MinArm;
		}
	}

	float MyCamera::GetMaxArm() const {
		return m_MaxArm;

	}
	void MyCamera::SetMaxArm(float f) {
		m_MaxArm = f;
	}
	float MyCamera::GetMinArm() const {
		return m_MinArm;
	}
	void MyCamera::SetMinArm(float f) {
		m_MinArm = f;
	}

	float MyCamera::GetRotSpeed() const {
		return m_RotSpeed;

	}
	void MyCamera::SetRotSpeed(float f) {
		m_RotSpeed = abs(f);
	}

	bsm::Vec3 MyCamera::GetTargetToAt() const {
		return m_TargetToAt;

	}
	void MyCamera::SetTargetToAt(const bsm::Vec3& v) {
		m_TargetToAt = v;
	}

	bool MyCamera::GetLRBaseMode() const {
		return m_LRBaseMode;

	}
	bool MyCamera::IsLRBaseMode() const {
		return m_LRBaseMode;

	}
	void MyCamera::SetLRBaseMode(bool b) {
		m_LRBaseMode = b;
	}
	bool MyCamera::GetUDBaseMode() const {
		return m_UDBaseMode;

	}
	bool MyCamera::IsUDBaseMode() const {
		return m_UDBaseMode;
	}
	void MyCamera::SetUDBaseMode(bool b) {
		m_UDBaseMode = b;

	}


	void MyCamera::SetAt(const bsm::Vec3& At) {
		Camera::SetAt(At);
		bsm::Vec3 ArmVec = GetEye() - GetAt();
		ArmVec.normalize();
		ArmVec *= m_ArmLen;
		bsm::Vec3 NewEye = GetAt() + ArmVec;
		Camera::SetEye(NewEye);
	}
	void MyCamera::SetAt(float x, float y, float z) {
		Camera::SetAt(x, y, z);
		bsm::Vec3 ArmVec = GetEye() - GetAt();
		ArmVec.normalize();
		ArmVec *= m_ArmLen;
		bsm::Vec3 NewEye = GetAt() + ArmVec;
		Camera::SetEye(NewEye);

	}


	void MyCamera::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyData = App::GetApp()->GetInputDevice().GetKeyState();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		bsm::Vec3 NewEye = GetEye();
		bsm::Vec3 NewAt = GetAt();
		//�v�Z�Ɏg�����߂̘r�p�x�i�x�N�g���j
		bsm::Vec3 ArmVec = NewEye - NewAt;
		//���K�����Ă���
		ArmVec.normalize();
		if (CntlVec[0].bConnected) {
			//�㉺�p�x�̕ύX
			if (CntlVec[0].fThumbRY >= 0.1f || KeyData.m_bPushKeyTbl[VK_UP]) {
				if (IsUDBaseMode()) {
					m_RadY += m_CameraUpDownSpeed * ElapsedTime;
				}
				else {
					m_RadY -= m_CameraUpDownSpeed * ElapsedTime;
				}
			}
			else if (CntlVec[0].fThumbRY <= -0.1f || KeyData.m_bPushKeyTbl[VK_DOWN]) {
				if (IsUDBaseMode()) {
					m_RadY -= m_CameraUpDownSpeed * ElapsedTime;
				}
				else {
					m_RadY += m_CameraUpDownSpeed * ElapsedTime;
				}
			}
			if (m_RadY > XM_PI * 4 / 9.0f) {
				m_RadY = XM_PI * 4 / 9.0f;
			}
			else if (m_RadY <= m_CameraUnderRot) {
				//�J���������E���ɉ��������炻��ȏ㉺����Ȃ�
				m_RadY = m_CameraUnderRot;
			}
			ArmVec.y = sin(m_RadY);
			//������Y����]���쐬
			if (CntlVec[0].fThumbRX != 0 || KeyData.m_bPushKeyTbl[VK_LEFT] || KeyData.m_bPushKeyTbl[VK_RIGHT]) {
				//��]�X�s�[�h�𔽉f
				if (CntlVec[0].fThumbRX != 0) {
					if (IsLRBaseMode()) {
						m_RadXZ += -CntlVec[0].fThumbRX * ElapsedTime * m_RotSpeed;
					}
					else {
						m_RadXZ += CntlVec[0].fThumbRX * ElapsedTime * m_RotSpeed;
					}
				}
				else if (KeyData.m_bPushKeyTbl[VK_LEFT]) {
					if (IsLRBaseMode()) {
						m_RadXZ += ElapsedTime * m_RotSpeed;
					}
					else {
						m_RadXZ -= ElapsedTime * m_RotSpeed;
					}
				}
				else if (KeyData.m_bPushKeyTbl[VK_RIGHT]) {
					if (IsLRBaseMode()) {
						m_RadXZ -= ElapsedTime * m_RotSpeed;
					}
					else {
						m_RadXZ += ElapsedTime * m_RotSpeed;
					}

				}
				if (abs(m_RadXZ) >= XM_2PI) {
					//1�T�������0��]�ɂ���
					m_RadXZ = 0;
				}
			}
			//�N�I�[�^�j�I����Y��]�i�܂�XZ�x�N�g���̒l�j���v�Z
			bsm::Quat QtXZ;
			QtXZ.rotation(m_RadXZ, bsm::Vec3(0, 1.0f, 0));
			QtXZ.normalize();
			//�ړ���s�̍s��v�Z���邱�ƂŁAXZ�̒l���Z�o
			bsm::Mat4x4 Mat;
			Mat.strTransformation(
				bsm::Vec3(1.0f, 1.0f, 1.0f),
				bsm::Vec3(0.0f, 0.0f, -1.0f),
				QtXZ
			);

			bsm::Vec3 PosXZ = Mat.transInMatrix();
			//XZ�̒l���킩�����̂Řr�p�x�ɑ��
			ArmVec.x = PosXZ.x;
			ArmVec.z = PosXZ.z;
			//�r�p�x�𐳋K��
			ArmVec.normalize();

			auto TargetPtr = GetTargetObject();
			if (TargetPtr) {
				//�ڎw�������ꏊ
				bsm::Vec3 ToAt = TargetPtr->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
				ToAt += m_TargetToAt;
				NewAt = Lerp::CalculateLerp(GetAt(), ToAt, 0, 1.0f, 1.0, Lerp::Linear);
			}
			//�A�[���̕ύX
			//D�p�b�h��
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN || KeyData.m_bPushKeyTbl[VK_NEXT]) {
				//�J�����ʒu������
				m_ArmLen += m_ZoomSpeed;
				if (m_ArmLen >= m_MaxArm) {
					//m_MaxArm�ȏ㗣��Ȃ��悤�ɂ���
					m_ArmLen = m_MaxArm;
				}
			}
			//D�p�b�h��
			else if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP || KeyData.m_bPushKeyTbl[VK_PRIOR]) {
				//�J�����ʒu�����
				m_ArmLen -= m_ZoomSpeed;
				if (m_ArmLen <= m_MinArm) {
					//m_MinArm�ȉ��߂Â��Ȃ��悤�ɂ���
					m_ArmLen = m_MinArm;
				}
			}
			////�ڎw�������ꏊ�ɃA�[���̒l�Ƙr�x�N�g����Eye�𒲐�
			bsm::Vec3 ToEye = NewAt + ArmVec * m_ArmLen;
			NewEye = Lerp::CalculateLerp(GetEye(), ToEye, 0, 1.0f, m_ToTargetLerp, Lerp::Linear);
		}
		SetAt(NewAt);
		SetEye(NewEye);
		UpdateArmLengh();
		Camera::OnUpdate();
	}



}
//end basecross
