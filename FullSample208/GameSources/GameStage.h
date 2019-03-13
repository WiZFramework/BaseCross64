/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	enum class CameraSelect {
		openingCamera,
		myCamera,
		objCamera,
	};

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//OpeningCamera�p�̃r���[
		shared_ptr<SingleView> m_OpeningCameraView;
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
		//�S�[���I�u�W�F�N�g�̍쐬
		void CreateGoalObject();
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
		void ToObjCamera();
		void ToMyCamera();

	};


}
//end basecross

