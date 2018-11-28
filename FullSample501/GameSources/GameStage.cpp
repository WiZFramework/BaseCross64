/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�e�t�@�N�g���[
	//--------------------------------------------------------------------------------------
	//�r���[�̍쐬
	void AbstractFactory::CreateViewLight() {
		auto ptrStage = GetStage();
		auto ptrView = ptrStage->CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto ptrMyCamera = ObjectFactory::Create<MyCamera>();
		ptrView->SetCamera(ptrMyCamera);
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = ptrStage->CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();
	}

	void AbstractFactory::OnCreate() {
		//�r���[�ƃ��C�g�̍쐬
		CreateViewLight();
		//�Œ�̃{�b�N�X�̍쐬
		CreateFixedBox();
		//��Q�����̍쐬
		CreateFixedSphere();
		//�ǂ�������I�u�W�F�N�g�̍쐬
		CreateSeekObject();
		//�ǐՂ���I�u�W�F�N�g�̍쐬
		CreatePursuitObject();
		//�v���[���[�̍쐬
		CreatePlayer();
	}



	//--------------------------------------------------------------------------------------
	//	�t�@�N�g���[1
	//--------------------------------------------------------------------------------------
	//�Œ�̃{�b�N�X�̍쐬
	void Factory1::CreateFixedBox() {
		//�z��̏�����
		vector< vector<Vec3> > vec = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			GetStage()->AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//��Q�����̍쐬
	void Factory1::CreateFixedSphere() {
		//�z��̏�����
		vector< vector<Vec3> > vec = {
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.0f, 10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-5.0f, 0.0f, 10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.0f, -10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-.0f, 0.0f, -10.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			GetStage()->AddGameObject<FixedSphere>(1.0f, v[0], v[1]);
		}

	}



	//�ǂ�������I�u�W�F�N�g�̍쐬
	void Factory1::CreateSeekObject() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto group = GetStage()->CreateSharedObjectGroup(L"ObjGroup");
		//�z��̏�����
		vector<Vec3> vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < vec.size(); count++) {
			GetStage()->AddGameObject<SeekObject>(vec[count]);
		}

	}

	//�ǐՂ���I�u�W�F�N�g�̍쐬
	void Factory1::CreatePursuitObject() {
		//�z��̏�����
		vector<Vec3> vec = {
			{ 10.0f, 0.125f, 10.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < vec.size(); count++) {
			GetStage()->AddGameObject<PursuitObject>(vec[count]);
		}

	}

	//�v���C���[�̍쐬
	void Factory1::CreatePlayer() {
		//�v���[���[�̍쐬
		auto ptrPlayer = GetStage()->AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		GetStage()->SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");
	}




	//--------------------------------------------------------------------------------------
	//	�t�@�N�g���[2
	//--------------------------------------------------------------------------------------
	//�Œ�̃{�b�N�X�̍쐬
	void Factory2::CreateFixedBox() {
		//�z��̏�����
		vector< vector<Vec3> > vec = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			GetStage()->AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//��Q�����̍쐬
	void Factory2::CreateFixedSphere() {
		//�z��̏�����
		vector< vector<Vec3> > vec = {
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.0f, 10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-5.0f, 0.0f, 10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.0f, -10.0f)
			},
			{
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-.0f, 0.0f, -10.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			GetStage()->AddGameObject<FixedSphere>(1.0f, v[0], v[1]);
		}

	}



	//�ǂ�������I�u�W�F�N�g�̍쐬
	void Factory2::CreateSeekObject() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto group = GetStage()->CreateSharedObjectGroup(L"ObjGroup");
		//�z��̏�����
		vector<Vec3> vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
			{ 10.0f, 0.125f, 10.0f },
			{ 10.0f, 0.125f, -10.0f },
			{ -10.0f, 0.125f, 10.0f },
			{ -10.0f, 0.125f, -10.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < vec.size(); count++) {
			GetStage()->AddGameObject<SeekObject>(vec[count]);
		}

	}

	//�ǐՂ���I�u�W�F�N�g�̍쐬
	void Factory2::CreatePursuitObject() {
		//�z��̏�����
		vector<Vec3> vec = {
			{ 20.0f, 0.125f, 20.0f },
			{ 20.0f, 0.125f, -20.0f },
			{ -20.0f, 0.125f, 20.0f },
			{ -20.0f, 0.125f, -20.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < vec.size(); count++) {
			GetStage()->AddGameObject<PursuitObject>(vec[count]);
		}

	}

	//�v���C���[�̍쐬
	void Factory2::CreatePlayer() {
		//�v���[���[�̍쐬
		auto ptrPlayer = GetStage()->AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		GetStage()->SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");
	}


	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------
	void GameStage::OnCreate() {
		try {
			//2����1�̊m���̗����𔭐�
			if (rand() % 2) {
				AddGameObject<Factory2>();
			}
			else {
				AddGameObject<Factory1>();
			}
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
