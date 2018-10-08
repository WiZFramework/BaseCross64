/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
@copyright shike.y
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	�l�p�`�X�v���C�g�̍\����
	//--------------------------------------------------------------------------------------
	struct SquareSprite {
		Vec2 m_LocalScale;				// ���[�J���X�P�[�����O
		float m_LocalRot;				// ���[�J����]�p�x
		float m_LocalRotVelocity;		// ���[�J����]���x
		Vec2 m_LocalPos;				// ���[�J���ʒu
		Vec2 m_LocalVelocity;			// ���[�J�����x
		Vec2 m_LocalGravityVelocity;	// ���[�J���������x

		SquareSprite() :
			m_LocalScale(64.0f, 64.0f),
			m_LocalRot(0),
			m_LocalRotVelocity(0),
			m_LocalPos(0, 0),
			m_LocalVelocity(0, 0),
			m_LocalGravityVelocity(0, 0)
		{}
	};

	//--------------------------------------------------------------------------------------
	//	�l�p�`�X�v���C�g�̃O���[�v
	//--------------------------------------------------------------------------------------
	class SquareSpriteGroup : public ObjectInterface, public ShapeInterface {
		
		//�o�b�N�A�b�v���Ă������_�f�[�^
		vector<VertexPositionColor> m_BackupVertices;
		
		//���b�V��
		shared_ptr<MeshResource> m_SquareSpriteMesh;
		
		vector<SquareSprite> m_SquareSpriteVec;
		
		float m_TotalTime;
		
		//�e�I�u�W�F�N�g�̈ʒu���̕ύX
		void UpdateObjects(float ElapsedTime);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		SquareSpriteGroup();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSpriteGroup();
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
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};
}
//end basecross