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
		auto PtrLookAtCamera = ObjectFactory::Create<MyCamera>();
		PtrView->SetCamera(PtrLookAtCamera);
		PtrLookAtCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
	}

	//�{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {
		vector< vector<Vec3> > vec = {
			{
			Vec3(50.0f, 1.0f, 50.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f,0.5f,19.5f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f,0.5f,-19.5f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f,XM_PIDIV2,0.0f),
			Vec3(19.5f,0.5f,0.0f)
			},
			{
			Vec3(40.0f,1.0f,1.0f),
			Vec3(0.0f,XM_PIDIV2,0.0f),
			Vec3(-19.5f,0.5f,0.0f)
			},
		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			AddGameObject<TilingFixedBox>(v[0], v[1], v[2], 1.0f, 1.0f);
		}
	}

	//�X�p�[�N�̍쐬
	void GameStage::CreateSpark() {
		auto MultiSparkPtr = AddGameObject<MultiSpark>();
		//���L�I�u�W�F�N�g�ɃX�p�[�N��o�^
		SetSharedGameObject(L"MultiSpark", MultiSparkPtr);
	}

	//�G�̍쐬
	void GameStage::CreateEnemy() {
		AddGameObject<EnemyBox>(Vec3(1.0), Vec3(0.0f), Vec3(2.5f, 0.5f, 5.0f));
	}

	//�L�����N�^�[�̍쐬
	void GameStage::CreateChara() {
		//�{�[���L�����쐬
		auto ptr = AddGameObject<BoneChara>(Vec3(0, 0, 5.0f));
		//�V�F�A�z��Ƀ{�[���L�����ǉ�
		SetSharedGameObject(L"BoneChara", ptr);
		AddGameObject<BallChara>();
	}

	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
		CreateSharedObjectGroup(L"FireSphereGroup");
	}

	void GameStage::OnCreate() {
		try {
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			////CSV�t�@�C������B�̓ǂݍ���
			m_GameStageCsvB.SetFileName(DataDir + L"GameStageB.csv");
			m_GameStageCsvB.ReadCsv();
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�{�b�N�X�̍쐬
			CreateFixedBox();
			//�X�p�[�N�̍쐬
			CreateSpark();
			//�G�̍쐬
			CreateEnemy();
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
