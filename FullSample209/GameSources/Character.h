/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//�@�^�C�����O����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//�\�z�Ɣj��
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};


	//--------------------------------------------------------------------------------------
	///	�Z���������g�����s���N���X�̃t���O
	//--------------------------------------------------------------------------------------
	enum class CellSearchFlg {
		Failed,
		Seek,
		Arrived
	};

	//--------------------------------------------------------------------------------------
	//�@�G
	//--------------------------------------------------------------------------------------
	class Enemy : public GameObject {
		Vec3 m_StartPosition;
		Vec3 m_Force;
		Vec3 m_Velocity;
		weak_ptr<StageCellMap> m_CelMap;
		vector<CellIndex> m_CellPath;
		//���݂̎����̃Z���C���f�b�N�X
		int m_CellIndex;
		//�߂����i���́j�̃Z���C���f�b�N�X
		int m_NextCellIndex;
		//�^�[�Q�b�g�̃Z���C���f�b�N�X
		int m_TargetCellIndex;
	public:
		//�\�z�Ɣj��
		Enemy(const shared_ptr<Stage>& StagePtr, const shared_ptr<StageCellMap>& CellMap, const Vec3& Position);
		virtual ~Enemy();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�^�[�g�ʒu�𓾂�
		@return	�X�^�[�g�ʒu
		*/
		//--------------------------------------------------------------------------------------
		const Vec3& GetStartPosition()const {
			return m_StartPosition;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ړI�̏ꏊ���T�[�`����
		@param[in]	TargetPos	�ړI�̉ӏ�
		@return	�Z����Ɍ���������true
		*/
		//--------------------------------------------------------------------------------------
		bool Search(const Vec3& TargetPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ړI�̏ꏊ���T�[�`��Seek�s��������
		@param[in]	TargetPos	�ړI�̉ӏ�
		@return	Seek�ł����true
		*/
		//--------------------------------------------------------------------------------------
		CellSearchFlg SeekBehavior(const Vec3& TargetPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�ړI�̏ꏊ��arrive�s�����Ƃ�
		@param[in]	TargetPos	�ړI�̉ӏ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ArriveBehavior(const Vec3& TargetPos);

	};


}
//end basecross
