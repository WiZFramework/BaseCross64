/*!
@file CollisionManager.h
@brief �Փ˔���}�l�[�W���i�ȈՕ����v�Z���s���j
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Փ˃y�A
	//--------------------------------------------------------------------------------------
	struct CollisionPair {
		weak_ptr<Collision> m_Src;
		weak_ptr<Collision> m_Dest;
	};

	//--------------------------------------------------------------------------------------
	//	�Փ˔���}�l�[�W��
	//--------------------------------------------------------------------------------------
	class CollisionManager : public GameObject {
		enum class CollMessType {
			Enter,
			Excute,
			Exit
		};
		vector<CollisionPair> m_PairVec[2];
		vector<CollisionPair> m_ExitPairVec;
		UINT m_PairSwap;
		UINT m_BeforePairSwap;
		void CollisionKeepCheck();
		bool CollisionCheckSub(const shared_ptr<CollisionSphere>& Src, const shared_ptr<Collision>& Dest);
		bool CollisionCheckSub(const shared_ptr<CollisionCapsule>& Src, const shared_ptr<Collision>& Dest);
		bool CollisionCheckSub(const shared_ptr<CollisionObb>& Src, const shared_ptr<Collision>& Dest);

		bool CollisionCheck(const shared_ptr<Collision>& Src, const shared_ptr<Collision>& Dest);

		bool CheckInPair(const CollisionPair& tgt,UINT swap);


		void CollisionSub(size_t SrcIndex);
		void SendCollisionMessageSub(CollMessType messtype);
		void SendCollisionMessage();
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V�����Փ˃y�A��ݒ肷��B���łɏՓ˂��Ă����ꍇ�͉������Ȃ�
		@param[in]	pair	�V�����y�A
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddNewCollisionPair(const CollisionPair& pair);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̏Փ˃y�A�̔z��𓾂�(const)
		@return	���݂̏Փ˃y�A�̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector<CollisionPair> GetPairVec() const {
			return m_PairVec[m_PairSwap];
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1�O�̏Փ˃y�A�̔z��𓾂�(const)
		@return	1�O�̏Փ˃y�A�̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector<CollisionPair> GetBeforePairVec() const {
			return m_PairVec[m_BeforePairSwap];
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ՓˏI���̃y�A�̔z��𓾂�(const)
		@return	�ՓˏI���̃y�A�̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector<CollisionPair> GetExitPairVec() const {
			return m_ExitPairVec;
		}

	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


}
//end basecross
