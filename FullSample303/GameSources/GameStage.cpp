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
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//���̍쐬
	void GameStage::CreateSphere() {
		AddGameObject<PcSphere>(Vec3(-7.0f, 0.5f, 5.0f));
		AddGameObject<PnSphere>(Vec3(-5.0f, 0.5f, 5.0f));
		AddGameObject<PtSphere>(Vec3(-3.0f, 0.5f, 5.0f));
		AddGameObject<PctSphere>(Vec3(-1.0f, 0.5f, 5.0f));
		AddGameObject<PntSphere>(Vec3(1.0f, 0.5f, 5.0f), true);
		AddGameObject<PntSphere>(Vec3(3.0f, 0.5f, 5.0f), false);
		AddGameObject<PntSpSphere>(Vec3(5.0f, 0.5f, 5.0f), true);
		AddGameObject<PntSpSphere>(Vec3(7.0f, 0.5f, 5.0f), false);

	}

	//�L�����N�^�[�̍쐬
	void GameStage::CreateChara() {
		AddGameObject<StaticChara>(Vec3(2.5f, 0.0f, 0.0f), false);
		AddGameObject<StaticChara>(Vec3(5.0f, 0.0f, 0.0f), true);
		AddGameObject<StaticMultiMeshChara>(Vec3(2.5f, 0.0f, -2.0f), false);
		AddGameObject<StaticMultiMeshChara>(Vec3(5.0f, 0.0f, -2.0f), true);
		AddGameObject<BoneChara>(Vec3(-5.0f, 0.0f, 0.0f), false);
		AddGameObject<BoneChara>(Vec3(-2.5f, 0.0f, 0.0f), true);
		AddGameObject<BoneMultiMeshChara>(Vec3(-5.0f, 0.0f, -2.0f), false);
		AddGameObject<BoneMultiMeshChara>(Vec3(-2.5f, 0.0f, -2.0f), true);
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
			//���̍쐬
			CreateSphere();
			//�L�����N�^�[�̍쐬
			CreateChara();
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross