/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	
	//--------------------------------------------------------------------------------------
	//	�O�p�`�X�v���C�g
	//--------------------------------------------------------------------------------------
	class TriangleSprite : public ObjectInterface, public ShapeInterface {
		
		//���b�V��
		shared_ptr<MeshResource> m_TriangleMesh;
		Vec3 m_Pos;					//���݂̈ʒu
		Vec3 m_MoveSpan;			//�ʒu�ύX�l

	public:
		//�\�z�Ɣj��
		TriangleSprite();
		virtual ~TriangleSprite();

		//������
		virtual void OnCreate()override;

		void OnUpdate()override;

		void OnDraw()override;
	};

}
//end basecross
