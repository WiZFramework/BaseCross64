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
		auto ptrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto ptrMyCamera = ObjectFactory::Create<MyCamera>();
		ptrView->SetCamera(ptrMyCamera);
		ptrMyCamera->SetEye(Vec3(0.0f, 7.0f, -7.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();
	}

	//�Œ�̃{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		//�z��̏�����
		vector< vector<Vec3> > vec = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
			{
				Vec3(5.0f, 0.5f, 5.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(10.0f, 0.25f, 10.0f)
			},
			{
				Vec3(5.0f, 0.5f, 5.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(15.0f, 0.25f, 10.0f)
			},

			{
				Vec3(2.0f, 1.0f, 2.0f),
				Vec3(0, 0, 0),
				Vec3(10.0f, 0.5f, 10.0f)
			},
			{
				Vec3(4.0f, 1.0f, 4.0f),
				Vec3(0, 0, 0),
				Vec3(-10.0f, 0.5f, 10.0f)
			},
			{
				Vec3(10.0f, 0.5f, 10.0f),
				Vec3(-0.5f, 0.0f, -0.5f),
				Vec3(-10.0f, 0.25f, 10.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//�㉺�ړ����Ă���{�b�N�X�̍쐬
	void GameStage::CreateMoveBox() {
		CreateSharedObjectGroup(L"MoveBox");
		AddGameObject<MoveBox>(
			Vec3(5.0f, 0.5f, 5.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 15.0f)
			);
	}


	//�ǂ�������I�u�W�F�N�g�̍쐬
	void GameStage::CreateSeekObject() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto group = CreateSharedObjectGroup(L"SeekGroup");
		//�z��̏�����
		vector<Vec3> vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};

		//�z�u�I�u�W�F�N�g�̍쐬
		for (size_t count = 0; count < vec.size(); count++) {
			auto ptr = AddGameObject<SeekObject>(vec[count]);
			//�i���o�[�X�N�G�A���쐬���Ċ֘A������
			AddGameObject<NumberSquare>(ptr, count);
		}
	}

	//�X�p�[�N�̍쐬
	void GameStage::CreateSpark() {
		auto MultiSparkPtr = AddGameObject<MultiSpark>();
		//���L�I�u�W�F�N�g�ɃX�p�[�N��o�^
		SetSharedGameObject(L"MultiSpark", MultiSparkPtr);
	}
	//���̍쐬
	void GameStage::CreateFire() {
		auto MultiFirePtr = AddGameObject<MultiFire>();
		//���L�I�u�W�F�N�g�ɉ���o�^
		SetSharedGameObject(L"MultiFire", MultiFirePtr);
	}



	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto ptrPlayer = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");
	}




	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�Œ�̃{�b�N�X�̍쐬
			CreateFixedBox();
			//�㉺�ړ����Ă���{�b�N�X�̍쐬
			CreateMoveBox();
			//�ǂ�������I�u�W�F�N�g�̍쐬
			CreateSeekObject();
			//�X�p�[�N�̍쐬
			CreateSpark();
			//���̍쐬
			CreateFire();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross