/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	using namespace sce::PhysicsEffects;


	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------

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

	//���O�h�[���̍쐬
	void GameStage::CreateRagdoll() {
		Quat Qt;
		Qt.rotationX(0.3f);
		//���O�h�[��
		auto RagdollPtr = AddGameObject<Ragdoll>(Vec3(0.0f, 3.0f, 5.0f), Qt);
		//�V�F�A�z��Ƀ��O�h�[����ǉ�
		SetSharedGameObject(L"Ragdoll", RagdollPtr);
		auto RagMngPtr = AddGameObject<RagdollManager>(RagdollPtr);
		//�V�F�A�z��Ƀ��O�h�[���}�l�[�W����ǉ�
		SetSharedGameObject(L"RagdollManager", RagMngPtr);
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
			//���̑�
			AddGameObject<FixedPsBox>(Vec3(30.0f, 1.0f, 30.0f), Quat(), Vec3(0.0f, -0.5f, 0.0f));
			//���O�h�[��
			CreateRagdoll();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
