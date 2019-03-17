/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto bkCamera = App::GetApp()->GetScene<Scene>()->GetBackupCamera();
		if (!bkCamera) {
			auto ptrMyCamera = ObjectFactory::Create<MyCamera>();
			PtrView->SetCamera(ptrMyCamera);
			ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
			ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		}
		else {
			PtrView->SetCamera(bkCamera);
		}
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();

		//�V���h�E�}�b�v�̃��C�g�̍����𒲐�(�X�e�[�W���L������)
		Shadowmap::SetLightHeight(50.0f);
	}


	//�����v�Z�I�u�W�F�N�g�̍쐬
	void GameStage::CreatePhysicsObjects() {
		//���̑�
		AddGameObject<FixedPsBox>(Vec3(30.0f, 1.0f, 30.0f), Quat(), Vec3(0.0f, -0.5f, 0.0f));
		//�{�[���W���C���g�p
		auto BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(-2.0f, 1.5f, 4.0f), 5.0f, true);
		SetSharedGameObject(L"Brick01", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(-2.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick02", BrickPtr);
		//���֐߃W���C���g�p
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(-1.0f, 1.5f, 4.0f), 5.0f, true);
		SetSharedGameObject(L"Brick03", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(-1.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick04", BrickPtr);
		//���ԃW���C���g�p
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(0.0f, 1.5f, 4.0f), 5.0f, true);
		SetSharedGameObject(L"Brick05", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(0.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick06", BrickPtr);
		//�X���C�_�[�W���C���g�p
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(1.0f, 1.5f, 4.0f), 5.0f, true);
		SetSharedGameObject(L"Brick07", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(1.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick08", BrickPtr);
		//�Œ�W���C���g�p
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(2.0f, 1.5f, 4.0f), 5.0f, true);
		SetSharedGameObject(L"Brick09", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(2.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick10", BrickPtr);
		//���j�o�[�T���W���C���g�p
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(3.0f, 1.5f, 4.0f), 5.0f, true);
		SetSharedGameObject(L"Brick11", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(3.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick12", BrickPtr);

		//�W���C���g�}�l�[�W���̍쐬
		auto MngPtr = AddGameObject<PsJointManager>();
		SetSharedGameObject(L"PsJointManager", MngPtr);
	}

	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}


	void GameStage::OnCreate() {
		try {
			//�����v�Z�L��
			SetPhysicsActive(true);
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�����v�Z�I�u�W�F�N�g�̍쐬
			CreatePhysicsObjects();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
