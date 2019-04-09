/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox() {}

	//������
	void FixedBox::OnCreate() {
		auto ptrTransform = GetComponent<Transform>();
		ptrTransform->SetScale(m_Scale);
		ptrTransform->SetRotation(m_Rotation);
		ptrTransform->SetPosition(m_Position);
		//OBB�Փ�j�����t����
		auto ptrColl = AddComponent<CollisionObb>();
		ptrColl->SetFixed(true);
		//�^�O������
		AddTag(L"FixedBox");
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ptrShadow = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ptrShadow->SetMeshResource(L"DEFAULT_CUBE");
		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		ptrDraw->SetTextureResource(L"SKY_TX");
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetOwnShadowActive(true);

	}

	//--------------------------------------------------------------------------------------
	///	���˂��鋅��
	//--------------------------------------------------------------------------------------
	FireSphere::FireSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity
	) :
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity),
		m_Scale(0.25f),
		m_ActiveMaxY(-5.0f)
	{}
	FireSphere::~FireSphere() {}

	void FireSphere::OnCreate() {
		auto ptrTrans = GetComponent<Transform>();

		ptrTrans->SetScale(Vec3(m_Scale));
		ptrTrans->SetQuaternion(Quat());
		ptrTrans->SetPosition(m_Emitter);
		//�R���W������t����
		auto ptrColl = AddComponent<CollisionSphere>();
		ptrColl->SetSleepActive(true);
		//�d�͂�����
		auto ptrGra = AddComponent<Gravity>();
		//�e������
		auto ptrShadow = AddComponent<Shadowmap>();
		ptrShadow->SetMeshResource(L"DEFAULT_SPHERE");

		auto ptrDraw = AddComponent<BcPNTStaticDraw>();
		ptrDraw->SetFogEnabled(true);
		ptrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		ptrDraw->SetTextureResource(L"BROWN_TX");

		auto group = GetStage()->GetSharedObjectGroup(L"FireSphereGroup");
		group->IntoGroup(GetThis<GameObject>());

	}

	void FireSphere::OnUpdate() {
		auto ptrTrans = GetComponent<Transform>();

		auto camera = OnGetDrawCamera();
		auto norm = camera->GetAt() - camera->GetEye();
		auto test = ptrTrans->GetPosition() - camera->GetEye();
		norm.normalize();
		auto dot = bsm::dot(norm, test);
		if (dot < 0.0f) {
			SetDrawActive(false);
		}
		else {
			SetDrawActive(true);
		}

		if (ptrTrans->GetPosition().y > m_ActiveMaxY) {
			float elapsedTime = App::GetApp()->GetElapsedTime();
			auto Pos = ptrTrans->GetPosition();
			Pos += m_Velocity * elapsedTime;
			ptrTrans->SetPosition(Pos);
			auto ptrColl = GetComponent<CollisionSphere>();
			auto ptrDraw = GetComponent<BcPNTStaticDraw>();
			if (ptrColl->IsSleep()) {
				ptrDraw->SetDiffuse(Col4(0.0f, 0.0f, 1.0f, 1.0f));
			}
			else {
				ptrDraw->SetDiffuse(Col4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
		else {
			SetUpdateActive(false);
			SetDrawActive(false);
			return;
		}
	}

	void FireSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		auto ptrTrans = GetComponent<Transform>();
		ptrTrans->ResetPosition(Emitter);
		m_Velocity = Velocity;
		auto ptrGrav = GetComponent<Gravity>();
		ptrGrav->SetGravityVerocityZero();
		SetUpdateActive(true);
		SetDrawActive(true);
	}

	void FireSphere::OnCollisionEnter(const CollisionPair& Pair) {
		auto ptrTrans = GetComponent<Transform>();
		auto shDest = Pair.m_Dest.lock();
		m_Velocity -= shDest->GetVelocity();
		m_Velocity.reflect(Pair.m_SrcHitNormal);
		if (m_Velocity.length() > 20.0f) {
			m_Velocity.normalize();
			m_Velocity *= 20.0f;
		}
	}

	void FireSphere::OnCollisionExcute(const CollisionPair& Pair) {
		auto shDest = Pair.m_Dest.lock();
		if (shDest->IsFixed()) {
			//����
			m_Velocity *= 0.95f;
			if (m_Velocity.length() < 0.05f) {
				m_Velocity = Vec3(0);
			}
		}
	}


}
//end basecross
