/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedPsBox::FixedPsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	FixedPsBox::~FixedPsBox() {}
	//������
	void FixedPsBox::OnCreate() {

		auto PtrTrans = GetComponent<Transform>();

		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
		//�����v�Z�{�b�N�X
		PsBoxParam param(PtrTrans->GetWorldMatrix(), 0.0f, true, PsMotionType::MotionTypeFixed);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȃ{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActivePsBox::ActivePsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsBox::~ActivePsBox() {}
	//������
	void ActivePsBox::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();

		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
		//�����v�Z�{�b�N�X
		PsBoxParam param(PtrTrans->GetWorldMatrix(), 1.0f, true, PsMotionType::MotionTypeActive);
		auto PsPtr = AddComponent<RigidbodyBox>(param);
	}

	//--------------------------------------------------------------------------------------
	///	�����v�Z����A�N�e�B�u�ȋ���
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ActivePsSphere::ActivePsSphere(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsSphere::~ActivePsSphere() {}
	//������
	void ActivePsSphere::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(Vec3(m_Scale));
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//�e������
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//�����v�Z����
		//WorldMatrix�����Ƃ�RigidbodySphere�̃p�����[�^���쐬
		PsSphereParam param(PtrTransform->GetWorldMatrix(), 1.0f, true, PsMotionType::MotionTypeActive);
		auto PsPtr = AddComponent<RigidbodySphere>(param);
	}


}
//end basecross
