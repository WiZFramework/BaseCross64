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
		m_RecursiveCount(0),
		pImpl(new Impl())
	{}
	CollisionManager::~CollisionManager() {}

	void CollisionManager::EscapePair(CollisionPair& Pair) {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto SrcSh = Pair.m_Src.lock();
		auto DestSh = Pair.m_Dest.lock();
		if (SrcSh && DestSh) {
			if (!SrcSh->IsFixed()) {
				SrcSh->EscapeCollisionPair(Pair);
			}
		}
	}

	void CollisionManager::EscapeFromDest(CollisionPair& Pair) {
		if (m_RecursiveCount > 5) {
			//�ċA���[���Ȃ����獡��̃^�[���ł́A����ȏ�s��Ȃ�
			return;
		}
		m_RecursiveCount++;
		EscapePair(Pair);
		auto SrcSh = Pair.m_Src.lock();
		auto DestSh = Pair.m_Dest.lock();
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			auto TgtSrcSh = v.m_Src.lock();
			auto TgtDestSh = v.m_Dest.lock();
			if ((SrcSh == TgtSrcSh && DestSh == TgtDestSh) || (SrcSh == TgtDestSh && DestSh == TgtSrcSh)) {
				continue;
			}
			if (SrcSh == TgtDestSh) {
				EscapeFromDest(v);
				break;
			}
		}
		m_RecursiveCount--;
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


	bool CollisionManager::SimpleCollisionPair(CollisionPair& Pair) {
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
		}
		return false;
	}

	bool CollisionManager::EnableedCollisionPair(const shared_ptr<GameObject>& Src, const shared_ptr<GameObject>& Dest) {
		if (Src == Dest) {
			return false;
		}
		if (!Src->IsUpdateActive() || !Dest->IsUpdateActive()) {
			return false;
		}
		auto SrcColl = Src->GetComponent<Collision>(false);
		auto DestColl = Dest->GetComponent<Collision>(false);
		if (!SrcColl || !DestColl) {
			return false;
		}
		if (SrcColl->IsFixed() || !SrcColl->IsUpdateActive() || !DestColl->IsUpdateActive()) {
			return false;
		}
		if (SrcColl->IsExcludeCollisionObject(Dest) || DestColl->IsExcludeCollisionObject(Src)) {
			return false;
		}
		return true;
	}


	void CollisionManager::SetNewCollision() {
		auto& ObjVec = GetStage()->GetGameObjectVec();
		for (auto& v : ObjVec) {
			SetNewCollisionSub(v);
		}
	}

	void CollisionManager::SetNewCollisionSub(const shared_ptr<GameObject>& Src) {
		auto& ObjVec = GetStage()->GetGameObjectVec();
		for (auto& v : ObjVec) {
			if (EnableedCollisionPair(Src,v)) {
				auto SrcColl = Src->GetComponent<Collision>();
				auto DestColl = v->GetComponent<Collision>();
				if (!IsInPair(SrcColl, DestColl, true)) {
					//�L�[�v����Ă��钆�ɂȂ�������
					//Collision�ɂ��Փ˔���
					DestColl->CollisionCall(SrcColl);
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
		m_TempKeepVec.clear();
		m_TempExitVec.clear();
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			if (SimpleCollisionPair(v)) {
				//�܂��Փ˂��Ă���
				m_TempKeepVec.push_back(v);
			}
			else {
				m_TempExitVec.push_back(v);
			}
		}
		//�e���|�����̓��e��keep�y�A�ɃR�s�[
		m_CollisionPairVec[m_KeepIndex].resize(m_TempKeepVec.size());
		m_CollisionPairVec[m_KeepIndex] = m_TempKeepVec;

		//�L�[�v����Ă���y�A��Src�ɂ���Gravity���Z�b�g����Ă�����0�ɂ���
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			auto ShSrc = v.m_Src.lock();
			auto ShDest = v.m_Dest.lock();

			if (ShSrc) {
				auto Gr = ShSrc->GetGameObject()->GetComponent<Gravity>(false);
				if (Gr) {
					auto f = bsm::angleBetweenNormals(v.m_SrcHitNormal, Vec3(0, 1, 0));
					if (abs(f) < XM_PIDIV4) {
						Gr->SetGravityVerocityZero();
					}
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

		//�L�[�v�z��̃\�[�g
		//--------------------------------------------------------
		auto func = [](CollisionPair& Left, CollisionPair& Right)->bool {
			auto ShLeftDest = Left.m_Dest.lock();
			auto ShRightDest = Right.m_Dest.lock();
			if (!ShLeftDest->IsFixed() && !ShRightDest->IsFixed()) {
				auto LeftMove = bsm::length(ShLeftDest->GetGameObject()->GetComponent<Transform>()->GetVelocity());
				auto RightMove = bsm::length(ShRightDest->GetGameObject()->GetComponent<Transform>()->GetVelocity());
				if (LeftMove > RightMove) {
					return true;
				}
			}
			return false;
		};
		//�����̂��镨�D��Ƀ\�[�g
		std::sort(m_CollisionPairVec[m_KeepIndex].begin(), m_CollisionPairVec[m_KeepIndex].end(), func);

		///Fixed�Ɋ֌W�Ȃ��I�u�W�F�N�g�̃G�X�P�[�v
		for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
			auto TgtSrcSh = v.m_Src.lock();
			auto TgtDestSh = v.m_Dest.lock();
			if (!TgtSrcSh->IsFixed() && !TgtDestSh->IsFixed()) {
				EscapePair(v);
			}
		}

		m_RecursiveCount = 0;
		//Fixed�ɏՓ˂��Ă���
		//�t���Ɍ���
		for (auto it = m_CollisionPairVec[m_KeepIndex].rbegin(); it != m_CollisionPairVec[m_KeepIndex].rend(); it++) {
			auto TgtSrcSh = it->m_Src.lock();
			auto TgtDestSh = it->m_Dest.lock();
			if (TgtDestSh->IsFixed()) {
				//�ċA�Ăяo��
				EscapeFromDest(*it);
			}
		}

		//�Փ˃��b�Z�[�W�̑��M
		//Exit
		for (auto& v : m_TempExitVec) {
			auto ShSrc = v.m_Src.lock();
			auto ShDest = v.m_Dest.lock();
			if (ShSrc && ShDest) {
				auto ShSrcObj = ShSrc->GetGameObject();
				auto ShDestObj = ShDest->GetGameObject();
				if (ShSrcObj && ShDestObj) {
					ShSrcObj->OnCollisionExit(ShDestObj);
				}
			}
		}
		//�L�[�v
		for (auto& v : m_TempKeepVec) {
			auto ShSrc = v.m_Src.lock();
			auto ShDest = v.m_Dest.lock();
			if (ShSrc && ShDest) {
				auto ShSrcObj = ShSrc->GetGameObject();
				auto ShDestObj = ShDest->GetGameObject();
				if (ShSrcObj && ShDestObj) {
					ShSrcObj->OnCollisionExcute(ShDestObj);
				}
			}
		}
		//�V�K
		for (auto& v : m_CollisionPairVec[m_NewIndex]) {
			auto ShSrc = v.m_Src.lock();
			auto ShDest = v.m_Dest.lock();
			if (ShSrc && ShDest) {
				auto ShSrcObj = ShSrc->GetGameObject();
				auto ShDestObj = ShDest->GetGameObject();
				if (ShSrcObj && ShDestObj) {
					ShSrcObj->OnCollisionEnter(ShDestObj);
				}
			}
		}
	}



}
// end basecross
