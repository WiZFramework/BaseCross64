/*!
@file CollisionManager.h
@brief �Փ˔���}�l�[�W���i�ȈՕ����v�Z���s���j
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Փ˔���}�l�[�W��
	//--------------------------------------------------------------------------------------
	class CollisionManager : public GameObject {
		enum class CollMessType {
			Enter,
			Excute,
			Exit
		};
		void CollisionSub(size_t SrcIndex);
		bool CollisionPair(const shared_ptr<CollisionSphere>& src, const shared_ptr<Collision>& dest,const bsm::Vec3& HitPoint,float& deps);
		bool CollisionPair(const shared_ptr<CollisionCapsule>& src, const shared_ptr<Collision>& dest, const bsm::Vec3& HitPoint, float& deps);
		bool CollisionPair(const shared_ptr<CollisionObb>& src, const shared_ptr<Collision>& dest, const bsm::Vec3& HitPoint, float& deps);
		void CollisionPairbase(vector<CollisionHitPair>& CheckVec, vector<CollisionHitPair>& tempVec);
		void SendCollisionMessageSub(CollMessType messtype);
		void Solver(CollisionHitPair& SrcPair);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionState��Transform����擾
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateCollisionState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Enter����Excute�Ɉڍs����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void EnterToExcutePair();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Excute�y�A���X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateExcutePair();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Enter�y�A���X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateEnterPair();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�\���o�[���X�V����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateSolver();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�Փ˃��b�Z�[�W�𔭍s����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
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
		@brief	�Փ˂�ݒ肷��
		@param[in]	pair	�Փ˃y�A
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetEnterPair(const CollisionHitPair& pair);
	private:
		//Impl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


}
//end basecross
