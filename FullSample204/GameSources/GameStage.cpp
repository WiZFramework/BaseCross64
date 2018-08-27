/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------

	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PttMyCamera = ObjectFactory::Create<MyCamera>();
		PtrView->SetCamera(PttMyCamera);
		PttMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		PttMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}


	//�Œ�̃{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		//�z��̏�����
		vector< vector<Vec3> > Vec = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//��Q�����̍쐬
	void GameStage::CreateFixedSphere() {
		//�z��̏�����
		vector< vector<Vec3> > Vec = {
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
		for (auto v : Vec) {
			AddGameObject<FixedSphere>(1.0f, v[0], v[1]);
		}

	}



	//�ǂ�������I�u�W�F�N�g�̍쐬
	void GameStage::CreateSeekObject() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"ObjGroup");
		//�z��̏�����
		vector<Vec3> Vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<SeekObject>(Vec[count]);
		}

	}

	//�ǐՂ���I�u�W�F�N�g�̍쐬
	void GameStage::CreatePursuitObject() {
		//�z��̏�����
		vector<Vec3> Vec = {
			{ 10.0f, 0.125f, 10.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<PursuitObject>(Vec[count]);
		}

	}

	//�o�H���񂷂�I�u�W�F�N�g�̍쐬
	void GameStage::CreateFollowPathObject() {
		//�z��̏�����
		vector<Vec3> Vec = {
			{ -10.0f, 0.125f, 10.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<FollowPathObject>(Vec[count]);
		}

	}



	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
		PlayerPtr->AddTag(L"Player");
	}




	void GameStage::OnCreate() {
		try {
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
			//�o�H���񂷂�I�u�W�F�N�g�̍쐬
			CreateFollowPathObject();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross