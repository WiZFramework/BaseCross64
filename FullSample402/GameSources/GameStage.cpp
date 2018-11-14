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
		auto ptrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto bkCamera = App::GetApp()->GetScene<Scene>()->GetBackupCamera();
		if (!bkCamera) {
			auto ptrMyCamera = ObjectFactory::Create<MyCamera>(10.0f);
			ptrView->SetCamera(ptrMyCamera);
			ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
			ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		}
		else {
			ptrView->SetCamera(bkCamera);
		}
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();

	}


	//�����v�Z�I�u�W�F�N�g�̍쐬
	void GameStage::CreatePhysicsObjects() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"SeekObjectGroup");
		//���̑�
		AddGameObject<FixedPsBox>(Vec3(30.0f, 1.0f, 30.0f), Quat(), Vec3(0.0f, -0.5f, 0.0f));
		//�ʖ�
		AddGameObject<ActivePsConvex>(Vec3(-10.0f, 1.0f, 0.0f));
		AddGameObject<ActivePsConvex>(Vec3(10.0f, 1.0f, 0.0f));
		//����
		AddGameObject<ActivePsSphere>(1.0f, Vec3(0.0f, 1.0f, -10.f));
		AddGameObject<ActivePsSphere>(1.0f, Vec3(0.0f, 1.0f, 10.f));

		Quat Qt1, Qt2;
		Qt1.rotationZ(2.0f);
		Qt2.rotationX(0.7f);
		AddGameObject<ActivePsCombinedObject>(Qt1, Vec3(-5.0f, 5.0f, 5.0f));
		AddGameObject<ActivePsCombinedObject>(Qt2, Vec3(5.0f, 5.0f, 5.0f));

	}

	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto ptrPlayer = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", ptrPlayer);
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
