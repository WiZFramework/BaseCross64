/*!
@file CollisionManager.h
@brief �Փ˔���}�l�[�W�����́i�ȈՕ����v�Z���s���j
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct CollisionManager::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct CollisionManager::Impl {
		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	�Փ˔���Ǘ���
	//--------------------------------------------------------------------------------------
	CollisionManager::CollisionManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_NewIndex(0),
		m_KeepIndex(1),
		pImpl(new Impl())
	{}
	CollisionManager::~CollisionManager() {}

	void CollisionManager::EscapePair(CollisionPair& Pair) {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto SrcSh = Pair.m_Src.lock();
		auto DestSh = Pair.m_Dest.lock();
		auto DestSp = dynamic_pointer_cast<CollisionSphere>(DestSh);
		auto DestCap = dynamic_pointer_cast<CollisionCapsule>(DestSh);
		auto DestObb = dynamic_pointer_cast<CollisionObb>(DestSh);
		if (DestSp) {
			SrcSh->EscapeCollision(DestSp, Pair.m_SrcHitNormal);
		}
		else if (DestCap) {
			SrcSh->EscapeCollision(DestCap, Pair.m_SrcHitNormal);
		}
		else if (DestObb) {
			SrcSh->EscapeCollision(DestObb, Pair.m_SrcHitNormal);
		}
	}


	bool CollisionManager::SimpleCollisionPairSub(const shared_ptr<CollisionSphere>& Src, const shared_ptr<Collision>& Dest) {
		auto ShDestSp = dynamic_pointer_cast<CollisionSphere>(Dest);
		auto ShDestCap = dynamic_pointer_cast<CollisionCapsule>(Dest);
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Dest);
		auto SrcSp = Src->GetSphere();
		if (ShDestSp) {
			auto DestSp = ShDestSp->GetSphere();
			if (HitTest::SPHERE_SPHERE(SrcSp, DestSp)) {
				return true;
			}
		}
		else if (ShDestCap) {
			auto DestCap = ShDestCap->GetCapsule();
			bsm::Vec3 d;
			if (HitTest::SPHERE_CAPSULE(SrcSp, DestCap, d)) {
				return true;
			}
		}
		else if (ShDestObb) {
			auto DestObb = ShDestObb->GetObb();
			bsm::Vec3 d;
			if (HitTest::SPHERE_OBB(SrcSp, DestObb, d)) {
				return true;
			}
		}
		return false;
	}

	bool CollisionManager::SimpleCollisionPairSub(const shared_ptr<CollisionCapsule>& Src, const shared_ptr<Collision>& Dest) {
		auto ShDestSp = dynamic_pointer_cast<CollisionSphere>(Dest);
		auto ShDestCap = dynamic_pointer_cast<CollisionCapsule>(Dest);
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Dest);
		auto SrcCap = Src->GetCapsule();
		if (ShDestSp) {
			bsm::Vec3 d;
			auto DestSp = ShDestSp->GetSphere();
			if (HitTest::SPHERE_CAPSULE(DestSp, SrcCap, d)) {
				return true;
			}
		}
		else if (ShDestCap) {
			auto DestCap = ShDestCap->GetCapsule();
			bsm::Vec3 d1, d2;
			if (HitTest::CAPSULE_CAPSULE(SrcCap, DestCap, d1, d2)) {
				return true;
			}
		}
		else if (ShDestObb) {
			auto DestObb = ShDestObb->GetObb();
			bsm::Vec3 d;
			if (HitTest::CAPSULE_OBB(SrcCap, DestObb, d)) {
				return true;
			}
		}
		return false;
	}
	bool CollisionManager::SimpleCollisionPairSub(const shared_ptr<CollisionObb>& Src, const shared_ptr<Collision>& Dest) {
		auto ShDestSp = dynamic_pointer_cast<CollisionSphere>(Dest);
		auto ShDestCap = dynamic_pointer_cast<CollisionCapsule>(Dest);
		auto ShDestObb = dynamic_pointer_cast<CollisionObb>(Dest);
		auto SrcObb = Src->GetObb();
		if (ShDestSp) {
			bsm::Vec3 d;
			auto DestSp = ShDestSp->GetSphere();
			if (HitTest::SPHERE_OBB(DestSp, SrcObb, d)) {
				return true;
			}
		}
		else if (ShDestCap) {
			auto DestCap = ShDestCap->GetCapsule();
			bsm::Vec3 d;
			if (HitTest::CAPSULE_OBB(DestCap, SrcObb, d)) {
				return true;
			}
		}
		else if (ShDestObb) {
			auto DestObb = ShDestObb->GetObb();
			if (HitTest::OBB_OBB(SrcObb, DestObb)) {
				return true;
			}
		}
		return false;

	}



	bool CollisionManager::SimpleCollisionPair(const CollisionPair& Pair) {
		auto Src = Pair.m_Src.lock();
		auto Dest = Pair.m_Dest.lock();
		if (Src && Dest) {
			auto ShSrcSp = dynamic_pointer_cast<CollisionSphere>(Src);
			auto ShSrcCap = dynamic_pointer_cast<CollisionCapsule>(Src);
			auto ShSrcObb = dynamic_pointer_cast<CollisionObb>(Src);
			if (ShSrcSp) {
				return SimpleCollisionPairSub(ShSrcSp, Dest);
			}
			else if (ShSrcCap) {
				return SimpleCollisionPairSub(ShSrcCap, Dest);
			}
			else if (ShSrcObb) {
				return SimpleCollisionPairSub(ShSrcObb, Dest);
			}
			return false;
		}
		else {
			return false;
		}
	}

	void CollisionManager::SetNewCollision() {
		auto& ObjVec = GetStage()->GetGameObjectVec();
		for (auto& v : ObjVec) {
			if (v->IsUpdateActive()) {
				auto Col = v->GetComponent<Collision>(false);
				if (Col && Col->IsUpdateActive() && !Col->IsFixed()) {
					SetNewCollisionSub(Col);
				}
			}
		}
	}

	void CollisionManager::SetNewCollisionSub(const shared_ptr<Collision>& Src) {
		auto& ObjVec = GetStage()->GetGameObjectVec();
		for (auto& v : ObjVec) {
			if (v->IsUpdateActive()) {
				auto Dest = v->GetComponent<Collision>(false);
				if (Dest && Dest->IsUpdateActive() && Src != Dest) {
					if (!IsInPair(Src, Dest, true)) {
						//�L�[�v����Ă��钆�ɂȂ�������
						//Collision�ɂ��Փ˔���
						Dest->CollisionCall(Src);
					}
				}
			}
		}
	}

	void CollisionManager::InsertNewPair(const CollisionPair& NewPair) {
		m_CollisionPairVec[m_NewIndex].push_back(NewPair);
	}




	void CollisionManager::OnCreate() {

	}


	void CollisionManager::OnUpdate() {
		//keep�̃`�F�b�N
		m_TempVec.clear();
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			if (SimpleCollisionPair(v)) {
				//�܂��Փ˂��Ă���
				m_TempVec.push_back(v);
			}
		}
		//�e���|�����̓��e��keep�y�A�ɃR�s�[
		m_CollisionPairVec[m_KeepIndex].resize(m_TempVec.size());
		m_CollisionPairVec[m_KeepIndex] = m_TempVec;

		//�L�[�v����Ă���y�A��Src�ɂ���Gravity���Z�b�g����Ă�����0�ɂ���
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			auto ShSrc = v.m_Src.lock();
			if (ShSrc) {
				auto Gr = ShSrc->GetGameObject()->GetComponent<Gravity>(false);
				if (Gr) {
					Gr->SetGravityVerocityZero();
				}
			}
		}

		//�V�K�̃y�A�z��̃N���A
		m_CollisionPairVec[m_NewIndex].clear();
		//�V�K�̏Փ˔���
		SetNewCollision();
		//�ǉ����ꂽ�y�A���L�[�v�ɒǉ�
		for (auto& v : m_CollisionPairVec[m_NewIndex]) {
			m_CollisionPairVec[m_KeepIndex].push_back(v);
		}
		////�L�[�v����Ă���y�A�̃G�X�P�[�v
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			EscapePair(v);
		}


	}




}
// end basecross
