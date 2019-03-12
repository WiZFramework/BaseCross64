/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	enum class CameraSelect {
		myCamera,
		objCamera,
	};

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//MyCamera�p�̃r���[
		shared_ptr<SingleView> m_MyCameraView;
		//ObjCamera�p�̃r���[
		shared_ptr<SingleView> m_ObjCameraView;
		CameraSelect m_CameraSelect;
		//���̓n���h���[
		InputHandler<GameStage> m_InputHandler;
		//�r���[�̍쐬
		void CreateViewLight();
		//�{�b�N�X�̍쐬
		void CreateFixedBox();
		//�v���C���[�̍쐬
		void CreatePlayer();
		//�J�����}���̍쐬
		void CreateCameraman();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
		//�X�V
		virtual void OnUpdate()override;
		//A�{�^���Ȃɂ����Ȃ�
		void OnPushA() {}
		//B�{�^���J�����̕ύX
		void OnPushB();
		CameraSelect GetCameraSelect() const {
			return m_CameraSelect;
		}

	};


}
//end basecross

