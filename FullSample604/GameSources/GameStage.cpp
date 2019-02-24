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

	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>();
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	void GameStage::OnCreate() {
		try {
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�I�u�W�F�N�g�̃O���[�v���쐬����
			auto Group = CreateSharedObjectGroup(L"SeekGroup");
			//�Q�[���I�u�W�F�N�g�r���_�[
			GameObjecttXMLBuilder Builder;
			//�Q�[���I�u�W�F�N�g�̓o�^
			Builder.Register<TilingFixedBox>(L"TilingFixedBox");
			Builder.Register<SeekObject>(L"SeekObject");
			Builder.Register<MoveBox>(L"MoveBox");
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			//XML����Q�[���I�u�W�F�N�g�̍\�z
			wstring XMLStr = DataDir + L"GameStage";
			auto shScene = App::GetApp()->GetScene<Scene>();
			XMLStr += Util::IntToWStr(shScene->GetStageNum());
			XMLStr += L".xml";
			Builder.Build(GetThis<Stage>(), XMLStr, L"GameStage/GameObject");
			//�v���[���[�̍쐬
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
