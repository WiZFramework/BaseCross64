/*!
@file MyCamera.cpp
@brief カメラ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	MyCameraカメラ（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	//構築と破棄
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
	//アクセサ

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
			//m_MaxArm以上離れないようにする
			m_ArmLen = m_MaxArm;
		}
		if (m_ArmLen <= m_MinArm) {
			//m_MinArm以下近づかないようにする
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
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		bsm::Vec3 NewEye = GetEye();
		bsm::Vec3 NewAt = GetAt();
		//計算に使うための腕角度（ベクトル）
		bsm::Vec3 ArmVec = NewEye - NewAt;
		//正規化しておく
		ArmVec.normalize();
		if (CntlVec[0].bConnected) {
			//上下角度の変更
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
				//カメラが限界下に下がったらそれ以上下がらない
				m_RadY = m_CameraUnderRot;
			}
			ArmVec.y = sin(m_RadY);
			//ここでY軸回転を作成
			if (CntlVec[0].fThumbRX != 0 || KeyData.m_bPushKeyTbl[VK_LEFT] || KeyData.m_bPushKeyTbl[VK_RIGHT]) {
				//回転スピードを反映
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
					//1週回ったら0回転にする
					m_RadXZ = 0;
				}
			}
			//クオータニオンでY回転（つまりXZベクトルの値）を計算
			bsm::Quat QtXZ;
			QtXZ.rotation(m_RadXZ, bsm::Vec3(0, 1.0f, 0));
			QtXZ.normalize();
			//移動先行の行列計算することで、XZの値を算出
			bsm::Mat4x4 Mat;
			Mat.strTransformation(
				bsm::Vec3(1.0f, 1.0f, 1.0f),
				bsm::Vec3(0.0f, 0.0f, -1.0f),
				QtXZ
			);

			bsm::Vec3 PosXZ = Mat.transInMatrix();
			//XZの値がわかったので腕角度に代入
			ArmVec.x = PosXZ.x;
			ArmVec.z = PosXZ.z;
			//腕角度を正規化
			ArmVec.normalize();

			auto TargetPtr = GetTargetObject();
			if (TargetPtr) {
				//目指したい場所
				bsm::Vec3 ToAt = TargetPtr->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
				ToAt += m_TargetToAt;
				NewAt = Lerp::CalculateLerp(GetAt(), ToAt, 0, 1.0f, 1.0, Lerp::Linear);
			}
			//アームの変更
			//Dパッド下
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN || KeyData.m_bPushKeyTbl[VK_NEXT]) {
				//カメラ位置を引く
				m_ArmLen += m_ZoomSpeed;
				if (m_ArmLen >= m_MaxArm) {
					//m_MaxArm以上離れないようにする
					m_ArmLen = m_MaxArm;
				}
			}
			//Dパッド上
			else if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP || KeyData.m_bPushKeyTbl[VK_PRIOR]) {
				//カメラ位置を寄る
				m_ArmLen -= m_ZoomSpeed;
				if (m_ArmLen <= m_MinArm) {
					//m_MinArm以下近づかないようにする
					m_ArmLen = m_MinArm;
				}
			}
			////目指したい場所にアームの値と腕ベクトルでEyeを調整
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
