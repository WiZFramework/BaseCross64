/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�ʒu�w��̃��C��
	//--------------------------------------------------------------------------------------
	class PositionLine : public GameObject {
		Vec3 m_StartPos;
		Vec3 m_EndPos;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	StartObj	�X�^�[�g�̃I�u�W�F�N�g
		@param[in]	EndObj	�G���h�̃I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		PositionLine(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos,
			const Vec3& EndPos);
		virtual ~PositionLine() {}
		//������
		virtual void OnCreate() override;
		//�X�V
		virtual void OnUpdate() override;
		//���̃X�^�[�g�ʒu�̎擾
		const Vec3& GetStartPos() const {
			return m_StartPos;
		}
		//���̃X�^�[�g�ʒu�̐ݒ�
		void SetStartPos(const Vec3& Pos) {
			m_StartPos = Pos;
		}
		//���̃G���h�ʒu�̎擾
		const Vec3& GetEndPos() const {
			return m_EndPos;
		}
		//���̃G���h�ʒu�̐ݒ�
		void SetEndPos(const Vec3& Pos) {
			m_EndPos = Pos;
		}
		//���̃X�^�[�g�ƃG���h�I�u�W�F�N�g�ʒu�̐ݒ�
		void ResetObject(const Vec3& StartPos, const Vec3& EndPos) {
			m_StartPos = StartPos;
			m_EndPos = EndPos;
		}
	};




	//--------------------------------------------------------------------------------------
	///	�����v�Z����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class FixedPsBox : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedPsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~FixedPsBox();
		//������
		virtual void OnCreate() override;
	};





	//--------------------------------------------------------------------------------------
	///	���O�h�[��
	//--------------------------------------------------------------------------------------
	class Ragdoll : public GameObject {
		Vec3 m_StartPos;
		Quat m_StartQt;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	StartPos	�����ʒu
		@param[in]	StartQt		�����p��
		*/
		//--------------------------------------------------------------------------------------
		Ragdoll(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Quat& StartQt);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Ragdoll();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};

	//--------------------------------------------------------------------------------------
	///	���O�h�[���}�l�[�W���i�`����Ǘ�����j
	//--------------------------------------------------------------------------------------
	class RagdollManager : public GameObject {
		weak_ptr<Ragdoll> m_Ragdoll;
		vector<weak_ptr<GameObject>> m_RagdollObjVec;
		size_t m_HoldIndex;
		bool m_Holded;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		@param[in]	ragdoll		���O�h�[��
		*/
		//--------------------------------------------------------------------------------------
		RagdollManager(const shared_ptr<Stage>& StagePtr, const shared_ptr<Ragdoll>& ragdoll);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RagdollManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �z�[���h��Ԃ̐ݒ�
		@param[in]	b	�z�[���h��ԂȂ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetHold(bool b) {
			m_Holded = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �z�[���h����Ă�I�u�W�F�N�g�̃C���f�b�N�X�̐ݒ�
		@param[in]	i	�z�[���h����Ă�I�u�W�F�N�g�̃C���f�b�N�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetHoldIndex(size_t i) {
			m_HoldIndex = i;

		}
	};


}
//end basecross
