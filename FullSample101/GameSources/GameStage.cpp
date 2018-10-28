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
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	void GameStage::CreateFixedSphere() {
		struct SpStr {
			float Scale;
			Vec3 Rot;
			Vec3 Pos;
		};
		//�z��̏�����
		vector< SpStr > Data = {
			{
				2.0f,
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 1.0f, 15.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Data) {
			AddGameObject<FixedSphere>(v.Scale, v.Rot, v.Pos);
		}

	}


	void GameStage::CreateFixedCapsule() {
		//�z��̏�����
		vector< vector<Vec3> > Vec = {
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 1.0f, 15.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<FixedCapsule>(v[0], v[1], v[2]);
		}

	}


	//�ړ�����{�b�N�X�̍쐬
	void GameStage::CreateMoveBox() {
		//�z��̏�����
		vector< vector<Vec3> > Vec = {
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 1.0f, 5.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : Vec) {
			AddGameObject<MoveBox>(v[0], v[1], v[2]);
		}

	}

	//�ǂ�������I�u�W�F�N�g�̍쐬
	void GameStage::CreateSeekObject() {
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto Group = CreateSharedObjectGroup(L"SeekGroup");
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
			//�ړ�����{�b�N�X�̍쐬
			CreateMoveBox();
			//�Œ�̋��̂̍쐬
			CreateFixedSphere();
			//�Œ�̃J�v�Z���̍쐬
			CreateFixedCapsule();
			//�ǂ�������I�u�W�F�N�g�̍쐬
			CreateSeekObject();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
