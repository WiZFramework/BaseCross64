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
		bsm::Vec3 m_SrcCalcHitCenter;
		bsm::Vec3 m_DestCalcHitCenter;
		CollisionPair():
			m_SrcHitNormal(0),
			m_CalcHitPoint(0),
			m_SrcCalcHitCenter(0),
			m_DestCalcHitCenter(0)
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
		bool m_PriorityUnderEscapeY;
		//�S�������p�����[�^
		float m_EscapeSpan;
		int m_EscapeMax;
		int m_EscapeFloor;
		bool SimpleCollisionPair(CollisionPair& Pair);
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
		@brief ����Collision���L�[�v�����ǂ����ǂ����𓾂āA�L�[�v���̏ꍇ�y�A�̔z����Z�b�g����
		@param[in]	Coll	���ׂ�R���W����
		@param[out]	PairVec	�y�A�̔z��
		@return	�L�[�v���Ȃ�true�̂���PairVec�ɔz���Ԃ��B
		*/
		//--------------------------------------------------------------------------------------
		bool IsInKeepPair(const shared_ptr<Collision>& Coll, vector<CollisionPair>& PairVec, bool IsKeep) {
			UINT Index = m_NewIndex;
			if (IsKeep) {
				Index = m_KeepIndex;
			}
			PairVec.clear();
			bool ret = false;
			for (auto& v : m_CollisionPairVec[Index]) {
				if (v.m_Src.lock() == Coll || v.m_Dest.lock() == Coll) {
					PairVec.push_back(v);
					ret = true;
				}
			}
			return ret;
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
		@brief �G�X�P�[�v������Y�̉�����s�����ǂ����i�f�t�H���g��true�j
		@return	�G�X�P�[�v������Y�̉�����s���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsPriorityUnderEscapeY() const {
			return m_PriorityUnderEscapeY;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �G�X�P�[�v������Y�̉�����s�����ǂ����ݒ�i�f�t�H���g��false�j
		@param[in]	b	�ݒ�l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPriorityUnderEscapeY(bool b) {
			m_PriorityUnderEscapeY = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�S�������̂��߂̊Ԋu�𓾂�
		@return	�S�������̂��߂̊Ԋu
		*/
		//--------------------------------------------------------------------------------------
		float GetEscapeSpan() const {
			return m_EscapeSpan;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�S�������̂��߂̊Ԋu��ݒ肷��
		@param[in]	f	�ݒ肷��l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetEscapeSpan(float f) {
			m_EscapeSpan = f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�S�������̌J��Ԃ��}�b�N�X�l�𓾂�
		@return	�S�������̌J��Ԃ��}�b�N�X�l
		*/
		//--------------------------------------------------------------------------------------
		int GetEscapeMax() const {
			return m_EscapeMax;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�S�������̌J��Ԃ��}�b�N�X�l��ݒ肷��
		@param[in]	i	�ݒ肷��l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetEscapeMax(int i) {
			m_EscapeMax = i;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�S���������̐؂�̂Ă鏬���_�ȉ��̌����𓾂�
		@return	�S���������̐؂�̂Ă鏬���_�ȉ��̌���
		*/
		//--------------------------------------------------------------------------------------
		int GetEscapeFloor() const {
			return m_EscapeFloor;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�S���������̐؂�̂Ă鏬���_�ȉ��̌�����ݒ肷��
		@param[in]	i	�ݒ肷��l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetEscapeFloor(int i) {
			m_EscapeFloor = i;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�y�A���G�X�P�[�v����
		@param[in]	Pair	�y�A
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void EscapeCollisionPair(CollisionPair& Pair);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�R���W�����̕ύX�������m�F���A�K�v�Ȃ�X���[�v��Ԃɂ��邩�X���[�v��Ԃ���߂�
		@return	 �Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SleepCheckSet();
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
