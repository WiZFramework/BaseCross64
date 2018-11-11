/*!
@file CollisionManager.h
@brief �Փ˔���}�l�[�W���i�ȈՕ����v�Z���s���j
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	struct CollisionPair {
		weak_ptr<Collision> m_Src;
		weak_ptr<Collision> m_Dest;
		bsm::Vec3 m_SrcHitNormal;
		bsm::Vec3 m_CalcHitPoint;
		CollisionPair():
			m_SrcHitNormal(0),
			m_CalcHitPoint(0)
		{}
	};

	//--------------------------------------------------------------------------------------
	//	�Փ˔���}�l�[�W��
	//--------------------------------------------------------------------------------------
	class CollisionManager : public GameObject {
		vector<CollisionPair> m_CollisionPairVec[2];
		//�v�Z�Ɏg���z��
		vector<CollisionPair> m_TempKeepVec;
		vector<CollisionPair> m_TempExitVec;
		UINT m_NewIndex;
		UINT m_KeepIndex;
		int m_RecursiveCount;
		void EscapePair(CollisionPair& Pair);
		void EscapeFromDest(CollisionPair& Pair);

		bool SimpleCollisionPair(CollisionPair& Pair);
		bool SimpleCollisionPairSub(const shared_ptr<CollisionSphere>& Src, const shared_ptr<Collision>& Dest);
		bool SimpleCollisionPairSub(const shared_ptr<CollisionCapsule>& Src, const shared_ptr<Collision>& Dest);
		bool SimpleCollisionPairSub(const shared_ptr<CollisionObb>& Src, const shared_ptr<Collision>& Dest);
		void SetNewCollision();
		void SetNewCollisionSub(const shared_ptr<GameObject>& Src);
		bool EnableedCollisionPair(const shared_ptr<GameObject>& Src, const shared_ptr<GameObject>& Dest);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		*/
		//--------------------------------------------------------------------------------------
		explicit CollisionManager(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CollisionManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �y�A�����邩�ǂ���
		@param[in]	Src	�\�[�X
		@param[in]	Dest	�f�X�e�l�[�V����
		@param[in]	IsKeep	�L�[�v�y�A���ǂ���
		@return	�y�A�������true
		*/
		//--------------------------------------------------------------------------------------
		bool IsInPair(const shared_ptr<Collision>& Src, const shared_ptr<Collision>& Dest, bool IsKeep) {
			UINT Index = m_NewIndex;
			if (IsKeep) {
				Index = m_KeepIndex;
			}
			for (auto& v : m_CollisionPairVec[Index]) {
				auto ShSrc = v.m_Src.lock();
				auto ShDest = v.m_Dest.lock();
				if (ShSrc == Src && ShDest == Dest) {
					//�y�A���t�͕s��
					return true;
				}
				//if ((ShSrc == Src && ShDest == Dest) || (ShSrc == Dest && ShDest == Src)) {
				//	//�y�A���t�ł���
				//	return true;
				//}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ����Collision���L�[�v�����ǂ����ǂ���
		@param[in]	Coll	���ׂ�R���W����
		@return	�L�[�v���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsInKeep(const shared_ptr<Collision>& Coll) {
			for (auto& v : m_CollisionPairVec[m_KeepIndex]) {
				if (v.m_Src.lock() == Coll || v.m_Dest.lock() == Coll) {
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V�K�̏Փ˃y�A�̐ݒ�
		@param[in]	NewPair	�V�����y�A
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void InsertNewPair(const CollisionPair& NewPair);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


}
//end basecross
