/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		shared_ptr<SoundItem> m_BGM;
		//�r���[�̍쐬
		void CreateViewLight();
		//�����v�Z�I�u�W�F�N�g�̍쐬
		void CreatePhysicsObjects();
		//�X�p�[�N�̍쐬
		void CreateSpark();
		//�{�b�N�X�̍쐬
		void CreateBox();
		//�v���C���[�̍쐬
		void CreatePlayer();
		//BGM�̍Đ�
		void PlayBGM();
		//�}�E�X�ʒu
		Point2D<int> m_MousePoint;
	public:
		//�\�z�Ɣj��
		GameStage() :Stage(), m_MousePoint(0, 0) {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate()override;
		//�폜��
		virtual void OnDestroy()override;
		//�A�N�Z�T
		Point2D<int> GetMousePoint()const {
			return m_MousePoint;
		}
		//�}�E�X�ƃJ�����̃��C�̎擾
		void GetMouseRay(Vec3& Near, Vec3& Far);
		//�}�E�X�ɂ��I�u�W�F�N�g�I���̃N���A
		void SelectClear();
		//�}�E�X�̍��{�^��(�������u��)
		void OnLButtonEnter();
		//�}�E�X�̍��{�^��(������)
		void OnLButtonUp();
		//�}�E�X�̉E�{�^��(�������u��)
		void OnRButtonEnter();
	};


}
//end basecross

