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

	//�v���C���[�̍쐬
	void AbstractFactory::CreatePlayer() {
		//�v���[���[�̍쐬
		auto ptrPlayer = GetStage()->AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		GetStage()->SetSharedGameObject(L"Player", ptrPlayer);
		ptrPlayer->AddTag(L"Player");
	}


	//--------------------------------------------------------------------------------------
	//	�t�@�N�g���[1
	//--------------------------------------------------------------------------------------
	void Factory1::OnCreate() {
		//�r���[�ƃ��C�g�̍쐬
		CreateViewLight();
		//�z��̏�����
		vector< vector<Vec3> > vecBox = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
		};
		//�{�b�N�X�̍쐬
		for (auto v : vecBox) {
			GetStage()->AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
		//�z��̏�����
		vector< vector<Vec3> > vecSp = {
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
		//��Q�����̍쐬
		for (auto v : vecSp) {
			GetStage()->AddGameObject<FixedSphere>(1.0f, v[0], v[1]);
		}
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto group = GetStage()->CreateSharedObjectGroup(L"ObjGroup");
		//�z��̏�����
		vector<Vec3> vecSeek = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};
		//�ǂ�������I�u�W�F�N�g�̍쐬
		for (auto v : vecSeek) {
			GetStage()->AddGameObject<SeekObject>(v);
		}
		//�z��̏�����
		vector<Vec3> vecPursuit = {
			{ 10.0f, 0.125f, 10.0f },
		};
		//�ǐՂ���I�u�W�F�N�g�̍쐬
		for (auto v : vecPursuit) {
			GetStage()->AddGameObject<PursuitObject>(v);
		}
		//�v���[���[�̍쐬
		CreatePlayer();
	}


	//--------------------------------------------------------------------------------------
	//	�t�@�N�g���[2
	//--------------------------------------------------------------------------------------
	void Factory2::OnCreate() {
		//�r���[�ƃ��C�g�̍쐬
		CreateViewLight();
		//�z��̏�����
		vector< vector<Vec3> > vecBox = {
			{
				Vec3(50.0f, 1.0f, 50.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, -0.5f, 0.0f)
			},
		};
		//�{�b�N�X�̍쐬
		for (auto v : vecBox) {
			GetStage()->AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
		//�z��̏�����
		vector< vector<Vec3> > vecSp = {
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
		//��Q�����̍쐬
		for (auto v : vecSp) {
			GetStage()->AddGameObject<FixedSphere>(1.0f, v[0], v[1]);
		}
		//�I�u�W�F�N�g�̃O���[�v���쐬����
		auto group = GetStage()->CreateSharedObjectGroup(L"ObjGroup");
		//�z��̏�����
		vector<Vec3> vecSeek = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
			{ 10.0f, 0.125f, 10.0f },
			{ 10.0f, 0.125f, -10.0f },
			{ -10.0f, 0.125f, 10.0f },
			{ -10.0f, 0.125f, -10.0f },
		};
		//�ǂ�������I�u�W�F�N�g�̍쐬
		for (auto v : vecSeek) {
			GetStage()->AddGameObject<SeekObject>(v);
		}
		//�z��̏�����
		vector<Vec3> vecPursuit = {
			{ 20.0f, 0.125f, 20.0f },
			{ 20.0f, 0.125f, -20.0f },
			{ -20.0f, 0.125f, 20.0f },
			{ -20.0f, 0.125f, -20.0f },
		};
		//�ǐՂ���I�u�W�F�N�g�̍쐬
		for (auto v : vecPursuit) {
			GetStage()->AddGameObject<PursuitObject>(v);
		}
		//�v���[���[�̍쐬
		CreatePlayer();
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
