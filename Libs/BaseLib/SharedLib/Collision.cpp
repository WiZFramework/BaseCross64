/*!
@file Collision.cpp
@brief �Փ˔���R���|�[�l���g����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	struct Collision::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Collision::Impl {
		bool m_Fixed;		//�Î~�I�u�W�F�N�g���ǂ���
		weak_ptr<MeshResource> m_MeshResource;	//���b�V�����\�[�X
		weak_ptr<GameObjectGroup> m_ExcludeCollisionGroup;	//���肩�珜�O����O���[�v
		//���肩�珜�O����^�O
		set<wstring> m_ExcludeCollisionTags;
		//�Փˌ�̏���
		AfterCollision m_AfterCollision;
		Impl() :
			m_Fixed(false),
			m_AfterCollision(AfterCollision::Auto)
		{
		}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class Collision : public Component ;
	//	�p�r: �Փ˔���R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Collision::Collision(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Collision::~Collision() {}

	bool Collision::GetFixed() const {
		return pImpl->m_Fixed;
	}
	bool Collision::IsFixed() const {
		return pImpl->m_Fixed;
	}
	void Collision::SetFixed(bool b) {
		pImpl->m_Fixed = b;
	}

	AfterCollision Collision::GetAfterCollision() const {
		return pImpl->m_AfterCollision;
	}
	void Collision::SetAfterCollision(AfterCollision a) {
		pImpl->m_AfterCollision = a;
	}

	shared_ptr<GameObjectGroup> Collision::GetExcludeCollisionGroup() const {
		auto shptr = pImpl->m_ExcludeCollisionGroup.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}

	void Collision::SetExcludeCollisionGroup(const shared_ptr<GameObjectGroup>& Group) {
		pImpl->m_ExcludeCollisionGroup = Group;
	}

	void Collision::SetExcludeCollisionGroup(const wstring& GroupStr) {
		pImpl->m_ExcludeCollisionGroup = GetGameObject()->GetStage()->GetSharedObjectGroup(GroupStr);
	}



	bool Collision::FindExcludeCollisionTag(const wstring& tagstr) const {
		if (pImpl->m_ExcludeCollisionTags.find(tagstr) == pImpl->m_ExcludeCollisionTags.end()) {
			return false;
		}
		return true;
	}
	void  Collision::AddExcludeCollisionTag(const wstring& tagstr) {
		if (tagstr == L"") {
			//�󔒂Ȃ��O
			throw BaseException(
				L"�ݒ肷��^�O����ł�",
				L"if (tagstr == L"")",
				L"Collision::AddExcludeCollisionTag()"
			);
		}
		pImpl->m_ExcludeCollisionTags.insert(tagstr);

	}
	void  Collision::RemoveExcludeCollisionTag(const wstring& tagstr) {
		pImpl->m_ExcludeCollisionTags.erase(tagstr);
	}



	bool Collision::IsExcludeCollisionObject(const shared_ptr<GameObject>& Obj) const {
		auto& tagset = Obj->GetTagSet();
		auto it = tagset.begin();
		while (it != tagset.end()) {
			if (FindExcludeCollisionTag(*it)) {
				return true;
			}
			it++;
		}
		auto shptr = pImpl->m_ExcludeCollisionGroup.lock();
		if (shptr) {
			auto Vec = shptr->GetGroupVector();
			for (auto& v : Vec) {
				auto shobj = v.lock();
				if (shobj && (shobj == Obj)) {
					return true;
				}
			}
		}
		return false;
	}

	//����
	shared_ptr<CollisionManager> Collision::GetCollisionManager() const {
		return GetGameObject()->GetStage()->GetCollisionManager();
	}

	void Collision::OnUpdate() {
	}

	//--------------------------------------------------------------------------------------
	//	struct CollisionSphere::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct CollisionSphere::Impl {
		float m_MakedDiameter;					//�쐬���̒��a
		//�z��{�����[���ƏՓˎ��ɏՓ˂����z�����肷��C���f�b�N�X
		size_t m_IsHitVolumeIndex;
		CalcScaling m_CalcScaling;
		Impl() :
			m_MakedDiameter(1.0f),
			m_IsHitVolumeIndex(0),
			m_CalcScaling(CalcScaling::YScale)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionSphere : public Collision ;
	//	�p�r: ���Փ˔���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	CollisionSphere::CollisionSphere(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionSphere::~CollisionSphere() {}

	void CollisionSphere::OnCreate() {
		SetDrawActive(false);
	}

	//�A�N�Z�T
	float CollisionSphere::GetMakedDiameter() const {
		return pImpl->m_MakedDiameter;
	}
	void CollisionSphere::SetMakedDiameter(float f) {
		pImpl->m_MakedDiameter = f;
	}
	float CollisionSphere::GetMakedRadius() const {
		return pImpl->m_MakedDiameter * 0.5f;
	}
	void CollisionSphere::SetMakedRadius(float f) {
		pImpl->m_MakedDiameter = f * 2.0f;
	}

	CalcScaling CollisionSphere::GetCalcScaling() const {
		return pImpl->m_CalcScaling;

	}
	void CollisionSphere::SetCalcScaling(CalcScaling s) {
		pImpl->m_CalcScaling = s;
	}


	SPHERE CollisionSphere::GetSphere() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Mat4x4 MatBase;
		MatBase.scale(bsm::Vec3(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter));
		MatBase *= TransPtr->GetWorldMatrix();
		//���̃I�u�W�F�N�g��SPHERE���쐬
		SPHERE Ret(MatBase.transInMatrix(), MatBase.scaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::XScale:
			Ret.m_Radius = MatBase.scaleInMatrix().x * 0.5f;
			break;
		case CalcScaling::YScale:
			Ret.m_Radius = MatBase.scaleInMatrix().y * 0.5f;
			break;
		case CalcScaling::ZScale:
			Ret.m_Radius = MatBase.scaleInMatrix().z * 0.5f;
			break;
		default:
			break;
		}
		return Ret;
	}


	SPHERE CollisionSphere::GetBeforeSphere() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		bsm::Mat4x4 MatBase;
		MatBase.scale(bsm::Vec3(pImpl->m_MakedDiameter, pImpl->m_MakedDiameter, pImpl->m_MakedDiameter));
		MatBase *= TransPtr->GetBeforeWorldMatrix();
		//���̃I�u�W�F�N�g��SPHERE���쐬
		SPHERE Ret(MatBase.transInMatrix(), MatBase.scaleInMatrix().x * 0.5f);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::XScale:
			Ret.m_Radius = MatBase.scaleInMatrix().x * 0.5f;
			break;
		case CalcScaling::YScale:
			Ret.m_Radius = MatBase.scaleInMatrix().y * 0.5f;
			break;
		case CalcScaling::ZScale:
			Ret.m_Radius = MatBase.scaleInMatrix().z * 0.5f;
			break;
		default:
			break;
		}
		return Ret;
	}


	void CollisionSphere::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionSphere>());
	}


	void CollisionSphere::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		//Sphere���m��CillisionWrappedSphere()�͌Ăяo���Ȃ�
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//���̏ꍇ�́A���ׂĈړ��ȊO�ω��Ȃ��Ƃ���
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//�����CollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereSphere(SrcBeforSphere, SpanVelocity, DestBeforeSphere, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			SPHERE SrcChkSphere = SrcBeforSphere;
			SrcChkSphere.m_Center += SrcVelocity * HitTime;
			SPHERE DestChkSphere = DestBeforeSphere;
			DestChkSphere.m_Center += DestVelocity * HitTime;
			pair.m_SrcHitNormal = SrcChkSphere.m_Center - DestChkSphere.m_Center;
			pair.m_SrcHitNormal.normalize();
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	void CollisionSphere::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphere�̏d�Ȃ肪�Ȃ��Ȃ�I��
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�ړ��ȊO�ω��Ȃ�
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//����
		CAPSULE DestCap = DestColl->GetCapsule();
		CAPSULE DestBeforeCap = DestColl->GetBeforeCapsule();

		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(SrcBeforSphere, SpanVelocity, DestBeforeCap, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			SPHERE SrcChkSphere = SrcBeforSphere;
			SrcChkSphere.m_Center += SrcVelocity * HitTime;
			CAPSULE DestChkCapsule = DestBeforeCap;
			DestChkCapsule.SetCenter(DestChkCapsule.GetCenter() + DestVelocity * HitTime);
			bsm::Vec3 ret;
			HitTest::SPHERE_CAPSULE(SrcChkSphere, DestChkCapsule, ret);
			//�Փ˂����u�ԂŖ@�����v�Z
			pair.m_SrcHitNormal = SrcChkSphere.m_Center - ret;
			pair.m_SrcHitNormal.normalize();
			GetCollisionManager()->InsertNewPair(pair);
		}
	}


	void CollisionSphere::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphere�̏d�Ȃ肪�Ȃ��Ȃ�I��
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�ړ��ȊO�ω��Ȃ�
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();
		//����
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(SrcBeforSphere, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			SPHERE SrcChkSphere = SrcBeforSphere;
			SrcChkSphere.m_Center += SrcVelocity * HitTime;
			OBB DestChkObb = DestBeforeObb;
			DestChkObb.m_Center += DestVelocity * HitTime;
			bsm::Vec3 ret;
			HitTest::SPHERE_OBB(SrcChkSphere, DestChkObb, ret);
			//�Փ˂����u�ԂŖ@�����v�Z
			pair.m_SrcHitNormal = SrcChkSphere.m_Center - ret;
			pair.m_SrcHitNormal.normalize();
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	SPHERE  CollisionSphere::GetEnclosingSphere()const {
		SPHERE SrcSphere = GetSphere();
		SPHERE SrcBeforSphere = GetBeforeSphere();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcSphere, SrcBeforSphere);
		return Src;
	}

	bsm::Vec3 CollisionSphere::GetCenterPosition()const {
		SPHERE SrcSphere = GetSphere();
		return SrcSphere.m_Center;
	}

	void CollisionSphere::EscapeCollisionPair(CollisionPair& Pair) {
		if (GetAfterCollision() == AfterCollision::None || Pair.m_Dest.lock()->GetAfterCollision() == AfterCollision::None) {
			return;
		}
		SPHERE SrcSphere = GetSphere();
		auto ShDestSphere = dynamic_pointer_cast<CollisionSphere>(Pair.m_Dest.lock());
		auto ShDestCapsule = dynamic_pointer_cast<CollisionCapsule>(Pair.m_Dest.lock());
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Pair.m_Dest.lock());
		if (ShDestSphere) {
			SPHERE DestSphere = ShDestSphere->GetSphere();
			bool IsHit = HitTest::SPHERE_SPHERE(SrcSphere, DestSphere);
			if (IsHit) {
				bsm::Vec3 Normal = SrcSphere.m_Center - DestSphere.m_Center;
				Normal.normalize();
				float Span = SrcSphere.m_Radius + DestSphere.m_Radius;
				Normal *= Span;
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				bsm::Vec3 Pos = DestSphere.m_Center + Normal;
				//�G�X�P�[�v�̓��Z�b�g
				PtrTransform->ResetWorldPosition(Pos);
			}
		}
		else if (ShDestCapsule) {
			CAPSULE DestCap = ShDestCapsule->GetCapsule();
			bsm::Vec3 Ret;
			bool Ishit = HitTest::SPHERE_CAPSULE(SrcSphere, DestCap, Ret);
			if (Ishit) {
				bsm::Vec3 span = SrcSphere.m_Center - Ret;
				span.normalize();
				span *= 0.001f;
				int count = 0;
				auto Center = SrcSphere.m_Center;
				while (1) {
					Center += span;
					SrcSphere.m_Center = Center;
					if (!HitTest::SPHERE_CAPSULE(SrcSphere, DestCap, Ret)) {
						break;
					}
					count++;
					if (count > 100) {
						break;
					}
				}
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//�G�X�P�[�v�̓��Z�b�g
				PtrTransform->ResetWorldPosition(Center);
			}
		}
		else if (ShDestObb) {
			OBB DestObb = ShDestObb->GetObb();
			bsm::Vec3 Ret;
			bool Ishit = HitTest::SPHERE_OBB(SrcSphere, DestObb, Ret);
			if (Ishit) {
				bsm::Vec3 span = Pair.m_SrcHitNormal;
				span.normalize();
				int count = 0;
				while (HitTest::SPHERE_OBB(SrcSphere, DestObb, Ret)) {
					SrcSphere.m_Center += span * 0.01f;
					count++;
					if (count > 20) {
						break;
					}
				}
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//�G�X�P�[�v�̓��Z�b�g
				PtrTransform->ResetWorldPosition(SrcSphere.m_Center);
			}
		}
	}



	void CollisionSphere::OnDraw() {
		GenericDraw Draw;
		bsm::Mat4x4 MeshToTransformMatrix;

		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto Scale = PtrTransform->GetScale();
		bsm::Vec3 CollScale(Scale.x, Scale.x, Scale.x);
		switch (pImpl->m_CalcScaling) {
		case CalcScaling::YScale:
			CollScale = bsm::Vec3(Scale.y, Scale.y, Scale.y);
			break;
		case CalcScaling::ZScale:
			CollScale = bsm::Vec3(Scale.z, Scale.z, Scale.z);
			break;
		default:
			break;
		}
		bsm::Vec3 ColcScale(CollScale.x / Scale.x, CollScale.y / Scale.y, CollScale.z / Scale.z);
		bsm::Mat4x4 mat;
		mat.scale(ColcScale);

		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_SPHERE"),
			mat);
	}


	//--------------------------------------------------------------------------------------
	//	struct CollisionCapsule::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct CollisionCapsule::Impl {
		float m_MakedDiameter;			//�쐬���̒��a
		float m_MakedHeight;			//�쐬������
		//�z��{�����[���ƏՓˎ��ɏՓ˂����z�����肷��C���f�b�N�X
		size_t m_IsHitVolumeIndex;

		bsm::Mat4x4 m_BeforeWorldMatrix;
		bsm::Mat4x4 m_WorldMatrix;
		CAPSULE m_BeforeWorldCapsule;
		CAPSULE m_WorldCapsule;

		bool m_FirstBeforeCalc;
		bool m_FirstCalc;


		Impl() :
			m_MakedDiameter(1.0f),
			m_MakedHeight(1.0f),
			m_IsHitVolumeIndex(0),
			m_BeforeWorldMatrix(),
			m_WorldMatrix(),
			m_BeforeWorldCapsule(),
			m_WorldCapsule(),
			m_FirstBeforeCalc(true),
			m_FirstCalc(true)

		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionCapsule : public Collision ;
	//	�p�r: �J�v�Z���Փ˔���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	CollisionCapsule::CollisionCapsule(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionCapsule::~CollisionCapsule() {}

	//������
	void CollisionCapsule::OnCreate() {
		SetDrawActive(false);
	}

	//�A�N�Z�T
	float CollisionCapsule::GetMakedDiameter() const {
		return pImpl->m_MakedDiameter;
	}
	void CollisionCapsule::SetMakedDiameter(float f) {
		pImpl->m_MakedDiameter = f;
	}
	float CollisionCapsule::GetMakedRadius() const {
		return pImpl->m_MakedDiameter * 0.5f;
	}
	void CollisionCapsule::SetMakedRadius(float f) {
		pImpl->m_MakedDiameter = f * 2.0f;
	}

	float CollisionCapsule::GetMakedHeight() const {
		return pImpl->m_MakedHeight;
	}
	void CollisionCapsule::SetMakedHeight(float f) {
		pImpl->m_MakedHeight = f;
	}

	CAPSULE CollisionCapsule::GetCapsule() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		//		if (pImpl->m_FirstCalc) {
		pImpl->m_WorldMatrix = TransPtr->GetWorldMatrix();
		pImpl->m_WorldCapsule = CAPSULE(
			pImpl->m_MakedDiameter * 0.5f,
			bsm::Vec3(0, pImpl->m_MakedHeight * -0.5f, 0),
			bsm::Vec3(0, pImpl->m_MakedHeight * 0.5f, 0),
			pImpl->m_WorldMatrix);
		pImpl->m_FirstCalc = false;
		//		}
		return pImpl->m_WorldCapsule;
	}


	CAPSULE CollisionCapsule::GetBeforeCapsule() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
		//		if (pImpl->m_FirstBeforeCalc) {
		pImpl->m_BeforeWorldMatrix = TransPtr->GetBeforeWorldMatrix();
		pImpl->m_BeforeWorldCapsule = CAPSULE(
			pImpl->m_MakedDiameter * 0.5f,
			bsm::Vec3(0, pImpl->m_MakedHeight * -0.5f, 0),
			bsm::Vec3(0, pImpl->m_MakedHeight * 0.5f, 0),
			pImpl->m_BeforeWorldMatrix);
		pImpl->m_FirstBeforeCalc = false;
		//		}
		return pImpl->m_BeforeWorldCapsule;
	}


	void CollisionCapsule::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionCapsule>());
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphere�̏d�Ȃ肪�Ȃ��Ȃ�I��
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�ړ��ȊO�ω��Ȃ�
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//�����CollisionSphere
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		//SPHERE��CAPSULE�̈������t�ɂȂ�
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereCapsule(DestBeforeSphere, SpanVelocity, SrcBeforCapsule, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			CAPSULE SrcChkCapsule = SrcBeforCapsule;
			SrcChkCapsule.SetCenter(SrcChkCapsule.GetCenter() + SrcVelocity * HitTime);
			SPHERE DestChkSphere = DestBeforeSphere;
			DestChkSphere.m_Center += DestVelocity * HitTime;
			bsm::Vec3 ret;
			HitTest::SPHERE_CAPSULE(DestChkSphere, SrcChkCapsule, ret);
			//�Փ˂����u�ԂŖ@�����v�Z
			pair.m_SrcHitNormal = ret - DestChkSphere.m_Center;
			pair.m_SrcHitNormal.normalize();
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphere�̏d�Ȃ肪�Ȃ��Ȃ�I��
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�ړ��ȊO�ω��Ȃ�
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//�����CollisionCapsule
		CAPSULE DestCapsule = DestColl->GetCapsule();
		CAPSULE DestBeforeCapsule = DestColl->GetBeforeCapsule();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleCapsule(SrcBeforCapsule, SpanVelocity, DestBeforeCapsule, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;

			CAPSULE SrcChkCapsule = SrcBeforCapsule;
			SrcChkCapsule.SetCenter(SrcChkCapsule.GetCenter() + SrcVelocity * HitTime);
			CAPSULE DestChkCapsule = DestBeforeCapsule;
			DestChkCapsule.SetCenter(DestChkCapsule.GetCenter() + DestVelocity * HitTime);

			bsm::Vec3 ret1, ret2;
			HitTest::CAPSULE_CAPSULE(SrcChkCapsule, DestChkCapsule, ret1, ret2);
			//�ړ_�ւ̃x�N�g��
			//DestCap�̐�����RetVec1�̐�����̍ŋߐړ_��RetVec1�̖@��
			bsm::Vec3 Start = DestChkCapsule.m_PointBottom;
			bsm::Vec3 End = DestChkCapsule.m_PointTop;
			float t;
			bsm::Vec3 RetVec;
			HitTest::ClosetPtPointSegment(ret1, Start, End, t, RetVec);
			pair.m_SrcHitNormal = ret1 - RetVec;
			pair.m_SrcHitNormal.normalize();
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	void CollisionCapsule::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphere�̏d�Ȃ肪�Ȃ��Ȃ�I��
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();

		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�ړ��ȊO�ω��Ȃ�
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();
		//�����CollisionObb
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(SrcBeforCapsule, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			CAPSULE SrcChkCapsule = SrcBeforCapsule;
			SrcChkCapsule.SetCenter(SrcChkCapsule.GetCenter() + SrcVelocity * HitTime);
			OBB DestChkObb = DestBeforeObb;
			DestChkObb.m_Center += DestVelocity * HitTime;
			bsm::Vec3 RetVec;
			HitTest::CAPSULE_OBB(SrcChkCapsule, DestChkObb, RetVec);
			//�ړ_�ւ̃x�N�g��
			float t;
			bsm::Vec3 SegPoint;
			HitTest::ClosetPtPointSegment(RetVec, SrcChkCapsule.m_PointBottom, SrcChkCapsule.m_PointTop, t, SegPoint);
			pair.m_SrcHitNormal = SegPoint - RetVec;
			pair.m_SrcHitNormal.normalize();
			pair.m_SrcHitLength = bsm::length(SrcChkCapsule.GetCenter() - RetVec);
			if (pair.m_SrcHitLength <= SrcChkCapsule.m_Radius) {
				pair.m_SrcHitLength = SrcChkCapsule.m_Radius;
			}
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	SPHERE  CollisionCapsule::GetEnclosingSphere()const {
		CAPSULE SrcCapsule = GetCapsule();
		CAPSULE SrcBeforCapsule = GetBeforeCapsule();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcCapsule.GetWrappedSPHERE(), SrcBeforCapsule.GetWrappedSPHERE());
		return Src;
	}

	bsm::Vec3 CollisionCapsule::GetCenterPosition()const {
		CAPSULE SrcCapsule = GetCapsule();
		return SrcCapsule.GetCenter();
	}

	void CollisionCapsule::EscapeCollisionPair(CollisionPair& Pair) {
		if (GetAfterCollision() == AfterCollision::None || Pair.m_Dest.lock()->GetAfterCollision() == AfterCollision::None) {
			return;
		}
		CAPSULE SrcCap = GetCapsule();
		auto ShDestSphere = dynamic_pointer_cast<CollisionSphere>(Pair.m_Dest.lock());
		auto ShDestCapsule = dynamic_pointer_cast<CollisionCapsule>(Pair.m_Dest.lock());
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Pair.m_Dest.lock());
		if (ShDestSphere) {
			auto Center = SrcCap.GetCenter();
			SPHERE DestSphere = ShDestSphere->GetSphere();
			bsm::Vec3 Ret;
			if (HitTest::SPHERE_CAPSULE(DestSphere, SrcCap, Ret)) {
				int count = 0;
				bsm::Vec3 span = DestSphere.m_Center - Ret;
				span.normalize();
				span *= 0.001f;
				while (1) {
					Center += span;
					SrcCap.SetCenter(Center);
					if (!HitTest::SPHERE_CAPSULE(DestSphere, SrcCap, Ret)) {
						break;
					}
					count++;
					if (count > 100) {
						break;
					}
				}
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//�G�X�P�[�v�̓��Z�b�g
				PtrTransform->ResetWorldPosition(Center);
			}
		}
		else if (ShDestCapsule) {
			auto Center = SrcCap.GetCenter();
			CAPSULE DestCap = ShDestCapsule->GetCapsule();
			bsm::Vec3 Ret1, Ret2;
			if (HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, Ret1, Ret2)) {
				int count = 0;
				//�ړ_�ւ̃x�N�g��
				float t;
				bsm::Vec3 SegPoint;
				HitTest::ClosetPtPointSegment(Ret1, SrcCap.m_PointBottom, SrcCap.m_PointTop, t, SegPoint);
				bsm::Vec3 span = SegPoint - Ret1;
				span.normalize();
				span *= 0.001f;
				while (1) {
					Center += span;
					SrcCap.SetCenter(Center);
					if (!HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, Ret1, Ret2)) {
						break;
					}
					count++;
					if (count > 100) {
						break;
					}
				}
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//�G�X�P�[�v�̓��Z�b�g
				PtrTransform->ResetWorldPosition(Center);
			}
		}
		else if (ShDestObb) {
			auto Center = SrcCap.GetCenter();
			OBB DestObb = ShDestObb->GetObb();
			bsm::Vec3 Ret;
			bool Ishit = HitTest::CAPSULE_OBB(SrcCap, DestObb, Ret);
			if (Ishit) {
				bsm::Vec3 span = SrcCap.GetCenter() - Ret;
				bsm::Vec3 spanChk = span;
				spanChk.normalize();
				if (span.length() <= 0.0f || abs(bsm::angleBetweenNormals(spanChk, Pair.m_SrcHitNormal)) > 1.0f) {
					span = Pair.m_SrcHitNormal;
				}
				span.normalize();
				span *= 0.005f;
				auto Center = SrcCap.GetCenter();
				int count = 0;
				while (1) {
					Center += span;
					SrcCap.SetCenter(Center);
					if (!HitTest::CAPSULE_OBB(SrcCap, DestObb, Ret)) {
						break;
					}
					count++;
					if (count > 100) {
						break;
					}
				}
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//�G�X�P�[�v�̓��Z�b�g
				PtrTransform->ResetWorldPosition(Center);
			}
		}

	}


	void CollisionCapsule::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_CAPSULE"));
	}




	//--------------------------------------------------------------------------------------
	//	struct CollisionObb::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct CollisionObb::Impl {
		float m_Size;					//�쐬���̃T�C�Y
		float m_ChkOnUnderLaySize;

		bsm::Mat4x4 m_BeforeWorldMatrix;
		bsm::Mat4x4 m_WorldMatrix;
		OBB m_BeforeWorldObb;
		OBB m_WorldObb;

		bool m_FirstBeforeCalc;
		bool m_FirstCalc;


		Impl() :
			m_Size(1.0f),
			m_ChkOnUnderLaySize(0.1f),
			m_BeforeWorldMatrix(),
			m_WorldMatrix(),
			m_BeforeWorldObb(),
			m_WorldObb(),
			m_FirstBeforeCalc(true),
			m_FirstCalc(true)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionObb : public Collision ;
	//	�p�r: Obb�Փ˔���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	CollisionObb::CollisionObb(const shared_ptr<GameObject>& GameObjectPtr) :
		Collision(GameObjectPtr),
		pImpl(new Impl())
	{}
	CollisionObb::~CollisionObb() {}

	//������
	void CollisionObb::OnCreate() {
		SetDrawActive(false);
	}


	//�A�N�Z�T
	float CollisionObb::GetMakedSize() const {
		return pImpl->m_Size;
	}
	void CollisionObb::SetMakedSize(float f) {
		pImpl->m_Size = f;
	}

	OBB CollisionObb::GetObb() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
//		if (pImpl->m_FirstCalc) {
			pImpl->m_WorldMatrix = TransPtr->GetWorldMatrix();
			bsm::Mat4x4 MatBase;
			MatBase.scale(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size));
			MatBase *= pImpl->m_WorldMatrix;
			pImpl->m_WorldObb = OBB(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
			pImpl->m_FirstCalc = false;
//		}
		return pImpl->m_WorldObb;
	}


	OBB CollisionObb::GetBeforeObb() const {
		auto TransPtr = GetGameObject()->GetComponent<Transform>();
//		if (pImpl->m_FirstBeforeCalc) {
			pImpl->m_BeforeWorldMatrix = TransPtr->GetBeforeWorldMatrix();
			bsm::Mat4x4 MatBase;
			MatBase.scale(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size));
			MatBase *= pImpl->m_BeforeWorldMatrix;
			pImpl->m_BeforeWorldObb = OBB(bsm::Vec3(pImpl->m_Size, pImpl->m_Size, pImpl->m_Size), MatBase);
			pImpl->m_FirstBeforeCalc = false;
//		}
		return pImpl->m_BeforeWorldObb;
	}


	void CollisionObb::CollisionCall(const shared_ptr<Collision>& Src) {
		Src->CollisionTest(GetThis<CollisionObb>());
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphere�̏d�Ȃ肪�Ȃ��Ȃ�I��
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�ړ��ȊO�ω��Ȃ�
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();
		//����
		SPHERE DestSphere = DestColl->GetSphere();
		SPHERE DestBeforeSphere = DestColl->GetBeforeSphere();
		//SPHERE��OBB�̈������t�ɂȂ�
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestSphereObb(DestBeforeSphere, SpanVelocity, SrcBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			OBB SrcChkObb = SrcBeforeObb;
			SrcChkObb.m_Center += SrcVelocity * HitTime;
			SPHERE DestChkSphere = DestBeforeSphere;
			DestChkSphere.m_Center += DestVelocity * HitTime;
			bsm::Vec3 ret;
			HitTest::SPHERE_OBB(DestChkSphere, SrcChkObb, ret);
			//�Փ˂����u�ԂŖ@�����v�Z
			pair.m_SrcHitNormal = ret - DestChkSphere.m_Center;
			pair.m_SrcHitNormal.normalize();
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	void CollisionObb::CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphere�̏d�Ȃ肪�Ȃ��Ȃ�I��
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�ړ��ȊO�ω��Ȃ�
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();
		//����
		CAPSULE DestCapsule = DestColl->GetCapsule();
		CAPSULE DestBeforeCapsule = DestColl->GetBeforeCapsule();
		//SPHERE��OBB�̈������t�ɂȂ�
		bsm::Vec3 SpanVelocity = DestVelocity - SrcVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestCapsuleObb(DestBeforeCapsule, SpanVelocity, SrcBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			OBB SrcChkObb = SrcBeforeObb;
			SrcChkObb.m_Center += SrcVelocity * HitTime;
			CAPSULE DestChkCapsule = DestBeforeCapsule;
			DestChkCapsule.SetCenter(DestChkCapsule.GetCenter() + DestVelocity * HitTime);
			bsm::Vec3 RetVec;
			HitTest::CAPSULE_OBB(DestChkCapsule, SrcChkObb, RetVec);
			//�ړ_�ւ̃x�N�g��
			float t;
			bsm::Vec3 SegPoint;
			HitTest::ClosetPtPointSegment(RetVec, DestChkCapsule.m_PointBottom, DestChkCapsule.m_PointTop, t, SegPoint);
			pair.m_SrcHitNormal = RetVec - SegPoint;
			pair.m_SrcHitNormal.normalize();
			pair.m_SrcHitLength = bsm::length(SrcChkObb.m_Center - RetVec);
			GetCollisionManager()->InsertNewPair(pair);
		}
	}


	void CollisionObb::CollisionTest(const shared_ptr<CollisionObb>& DestColl) {
		if (!CillisionWrappedSphere(DestColl)) {
			//Sphere�̏d�Ȃ肪�Ȃ��Ȃ�I��
			return;
		}
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		auto PtrDestTransform = DestColl->GetGameObject()->GetComponent<Transform>();
		bsm::Vec3 SrcVelocity = PtrTransform->GetVelocity();
		bsm::Vec3 DestVelocity = PtrDestTransform->GetVelocity();
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		//�ړ��ȊO�ω��Ȃ�
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();
		//����
		OBB DestObb = DestColl->GetObb();
		OBB DestBeforeObb = DestColl->GetBeforeObb();
		bsm::Vec3 SpanVelocity = SrcVelocity - DestVelocity;
		float HitTime = 0;
		if (HitTest::CollisionTestObbObb(SrcBeforeObb, SpanVelocity, DestBeforeObb, 0, ElapsedTime, HitTime)) {
			CollisionPair pair;
			pair.m_Src = GetThis<Collision>();
			pair.m_Dest = DestColl;
			OBB SrcChkObb = SrcBeforeObb;
			SrcChkObb.m_Center += SrcVelocity * HitTime;
			OBB DestChkObb = DestBeforeObb;
			DestChkObb.m_Center += DestVelocity * HitTime;
			bsm::Vec3 RetVec;
			//Src��OBB��Dest�̍ŋߐړ_�𓾂�
			HitTest::ClosestPtPointOBB(SrcChkObb.m_Center, DestChkObb, RetVec);
			//�ړ_�ւ̃x�N�g��
			//�Փ˂����u�ԂŖ@�����v�Z
			pair.m_SrcHitNormal = SrcChkObb.m_Center - RetVec;
			pair.m_SrcHitNormal.normalize();
			GetCollisionManager()->InsertNewPair(pair);
		}
	}

	SPHERE  CollisionObb::GetEnclosingSphere()const {
		OBB SrcObb = GetObb();
		OBB SrcBeforeObb = GetBeforeObb();

		SPHERE Src = HitTest::SphereEnclosingSphere(SrcObb.GetWrappedSPHERE(), SrcBeforeObb.GetWrappedSPHERE());
		return Src;
	}

	bsm::Vec3 CollisionObb::GetCenterPosition()const {
		OBB SrcObb = GetObb();
		return SrcObb.m_Center;
	}

	void CollisionObb::EscapeCollisionPair(CollisionPair& Pair) {
		if (GetAfterCollision() == AfterCollision::None || Pair.m_Dest.lock()->GetAfterCollision() == AfterCollision::None) {
			return;
		}
		OBB SrcObb = GetObb();
		auto ShDestSphere = dynamic_pointer_cast<CollisionSphere>(Pair.m_Dest.lock());
		auto ShDestCapsule = dynamic_pointer_cast<CollisionCapsule>(Pair.m_Dest.lock());
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Pair.m_Dest.lock());
		if (ShDestSphere) {
			SPHERE DestSphere = ShDestSphere->GetSphere();
			bsm::Vec3 Ret;
			bool Ishit = HitTest::SPHERE_OBB(DestSphere, SrcObb, Ret);
			if (Ishit) {
				float CenterToRetLen = bsm::length(Ret - SrcObb.m_Center);
				CenterToRetLen += DestSphere.m_Radius;
				bsm::Vec3 MoveSpan = SrcObb.m_Center - DestSphere.m_Center;
				MoveSpan.normalize();
				MoveSpan *= CenterToRetLen;
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				auto Pos = DestSphere.m_Center;
				Pos += MoveSpan;
				//�G�X�P�[�v�̓��Z�b�g
				PtrTransform->ResetWorldPosition(Pos);
			}
		}
		else if (ShDestCapsule) {
			auto Center = SrcObb.m_Center;
			CAPSULE DestCapsule = ShDestCapsule->GetCapsule();
			bsm::Vec3 Ret;
			bool Ishit = HitTest::CAPSULE_OBB(DestCapsule, SrcObb, Ret);
			if (Ishit) {
				bsm::Vec3 span = SrcObb.m_Center - Ret;
				span.normalize();
				span *= 0.005f;
				auto Center = SrcObb.m_Center;
				int count = 0;
				while (1) {
					Center += span;
					SrcObb.m_Center = Center;
					if (!HitTest::CAPSULE_OBB(DestCapsule, SrcObb, Ret)) {
						break;
					}
					count++;
					if (count > 100) {
						break;
					}
				}
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//�G�X�P�[�v�̓��Z�b�g
				PtrTransform->ResetWorldPosition(Center);
			}
		}
		else if (ShDestObb) {
			OBB DestObb = ShDestObb->GetObb();
			bool Ishit = HitTest::OBB_OBB(SrcObb, DestObb);
			if (Ishit) {
				bsm::Vec3 Ret;
				//Src��OBB��Dest�̍ŋߐړ_�𓾂�
				HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, Ret);
				bsm::Vec3 span = Pair.m_SrcHitNormal;
				span.normalize();
				span *= 0.001f;
				auto Center = SrcObb.m_Center;
				int count = 0;
				while (1) {
					Center += span;
					SrcObb.m_Center = Center;
					if (!HitTest::OBB_OBB(SrcObb, DestObb)) {
						break;
					}
					count++;
					if (count > 500) {
						break;
					}
				}
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				//�G�X�P�[�v�̓��Z�b�g
				PtrTransform->ResetWorldPosition(Center);
			}
		}
	}


	void CollisionObb::OnDraw() {
		GenericDraw Draw;
		Draw.DrawWireFrame(GetGameObject(), App::GetApp()->GetResource<MeshResource>(L"DEFAULT_PC_CUBE"));

	}


}
//end basecross

