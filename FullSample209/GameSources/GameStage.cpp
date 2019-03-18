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
		ptrMyCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		ptrMyCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto ptrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		ptrMultiLight->SetDefaultLighting();
	}

	//�Z���}�b�v�̍쐬
	void GameStage::CreateStageCellMap() {
		float  PieceSize = 1.0f;
		auto Ptr = AddGameObject<StageCellMap>(Vec3(-12.0f, 0, 1.0f), PieceSize,26, 16);
		//�Z���}�b�v�̋���\������ꍇ�͈ȉ��̐ݒ�
		Ptr->SetDrawActive(true);
		//����ɃZ���̃C���f�b�N�X�ƃR�X�g��\������ꍇ�͈ȉ��̐ݒ�
		//Ptr->SetCellStringActive(true);
		SetSharedGameObject(L"StageCellMap", Ptr);
		//�Z���}�b�v�ւ̃{�b�N�X�̃R�X�g�ݒ�
		SetCellMapCost();
	}


	//�{�b�N�X�̍쐬
	void GameStage::CreateFixedBox() {

		vector< vector<Vec3> > baseVec = {
			{
			Vec3(50.0f, 1.0f, 50.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 0.0f)
			},
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
		//���ƕǃI�u�W�F�N�g�̍쐬
		for (auto v : baseVec) {
			AddGameObject<TilingFixedBox>(v[0], v[1], v[2], 1.0f, 1.0f);
		}


		//��Q���{�b�N�X�̃O���[�v���쐬
		auto BoxGroup = CreateSharedObjectGroup(L"FixedBoxes");

		vector< vector<Vec3> > vec = {
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(-8.0f,0.5f,10.5f)
			},
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(0,0.5f,10.5f)
			},
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(8.0f,0.5f,10.5f)
			},
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(-6.0f,0.5f,5.5f)
			},
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(2.0f,0.5f,5.5f)
			},
			{
			Vec3(4.0f,1.0f,1.0f),
			Vec3(0.0f,0.0f,0.0f),
			Vec3(10.0f,0.5f,5.5f)
			},

		};
		//�I�u�W�F�N�g�̍쐬
		for (auto v : vec) {
			auto boxPtr = AddGameObject<TilingFixedBox>(v[0], v[1], v[2], 1.0f, 1.0f);
			//�{�b�N�X�̃O���[�v�ɒǉ�
			BoxGroup->IntoGroup(boxPtr);
		}
	}

	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//�G�̍쐬
	void  GameStage::CreateEnamy() {
		auto PtrCellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		AddGameObject<Enemy>(PtrCellmap,Vec3(0,0.25f,15.0f));

	}


	//�Œ�̃{�b�N�X�̃R�X�g���Z���}�b�v�ɔ��f
	void GameStage::SetCellMapCost() {
		//�Z���}�b�v����FixedBox�̏����Z�b�g
		auto PtrCellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		auto BoxGroup = GetSharedObjectGroup(L"FixedBoxes");
		//�Z���}�b�v����Z���̔z����擾
		auto& CellVec = PtrCellmap->GetCellVec();
		//�{�b�N�X�O���[�v����{�b�N�X�̔z����擾
		auto& BoxVec = BoxGroup->GetGroupVector();
		vector<AABB> ObjectsAABBVec;
		for (auto& v : BoxVec) {
			auto FixedBoxPtr = dynamic_pointer_cast<TilingFixedBox>(v.lock());
			if (FixedBoxPtr) {
				auto ColPtr = FixedBoxPtr->GetComponent<CollisionObb>();
				//�{�b�N�X�̏Փ˔��肩���b�s���O����AABB���擾���ĕۑ�
				ObjectsAABBVec.push_back(ColPtr->GetObb().GetWrappedAABB());
			}
		}
		//�Z���z�񂩂�Z�����X�L����
		for (auto& v : CellVec) {
			for (auto& v2 : v) {
				for (auto& vObj : ObjectsAABBVec) {
					if (HitTest::AABB_AABB_NOT_EQUAL(v2.m_PieceRange, vObj)) {
						//�{�b�N�X��ABB��NOT_EQUAL�ŏՓ˔���
						v2.m_Cost = -1;
						break;
					}
				}
			}
		}
	}


	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�{�b�N�X�̍쐬
			CreateFixedBox();
			//�Z���}�b�v�̍쐬
			CreateStageCellMap();
			//�v���[���[�̍쐬
			CreatePlayer();
			//�G�̍쐬
			CreateEnamy();
		}
		catch (...) {
			throw;
		}
	}


}
//end basecross
