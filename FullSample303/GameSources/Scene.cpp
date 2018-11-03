
/*!
@file Scene.cpp
@brief �V�[������
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	void Scene::CreateResourses() {
		wstring dataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(dataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(dataDir);
		wstring strTexture = dataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = dataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = dataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = dataDir + L"number.png";
		App::GetApp()->RegisterTexture(L"NUMBER_TX", strTexture);

		//�{�[�����f���̒ʏ탊�\�[�X
		auto modelMesh = MeshResource::CreateBoneModelMesh(dataDir, L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"Chara_R_MESH", modelMesh);

		//�{�[�����f���̃^���W�F���g�t�����\�[�X
		modelMesh = MeshResource::CreateBoneModelMeshWithTangent(dataDir, L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"Chara_R_MESH_WITH_TAN", modelMesh);
		//�@���}�b�v
		strTexture = dataDir + L"Chara_R_narmal.png";
		App::GetApp()->RegisterTexture(L"Chara_R_NORMAL_TX", strTexture);

		//�{�[�����f��(�}���`���b�V��)�̒ʏ탊�\�[�X
		auto multiModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(dataDir, L"Object_WalkAnimation.bmf");
		App::GetApp()->RegisterResource(L"Object_WalkAnimation_MESH", multiModelMesh);

		//�{�[�����f��(�}���`���b�V��)�̃^���W�F���g�t�����\�[�X
		multiModelMesh = MultiMeshResource::CreateBoneModelMultiMeshWithTangent(dataDir, L"Object_WalkAnimation.bmf");
		App::GetApp()->RegisterResource(L"Object_WalkAnimation_MESH_WITH_TAN", multiModelMesh);

		//�@���}�b�v
		strTexture = dataDir + L"Tx_Checker_Normal.png";
		App::GetApp()->RegisterTexture(L"OBJECT_NORMAL_TX", strTexture);


		//�X�^�e�B�b�N���f���̒ʏ탊�\�[�X
		auto staticModelMesh = MeshResource::CreateStaticModelMesh(dataDir, L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH", staticModelMesh);
		//�X�^�e�B�b�N���f���̃^���W�F���g�t�����\�[�X
		staticModelMesh = MeshResource::CreateStaticModelMeshWithTangent(dataDir, L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH_WITH_TAN", staticModelMesh);
		//�@���}�b�v
		strTexture = dataDir + L"Character_2_normal.png";
		App::GetApp()->RegisterTexture(L"MODEL_NORMAL_TX", strTexture);

		//�X�^�e�B�b�N���f��(�}���`���b�V��)�̒ʏ탊�\�[�X
		auto staticMultiModelMesh = MultiMeshResource::CreateStaticModelMultiMesh(dataDir, L"ObjectOnly.bmf");
		App::GetApp()->RegisterResource(L"ObjectOnly_MESH", staticMultiModelMesh);

		//�X�^�e�B�b�N���f��(�}���`���b�V��)�̃^���W�F���g�t�����\�[�X
		staticMultiModelMesh = MultiMeshResource::CreateStaticModelMultiMeshWithTangent(dataDir, L"ObjectOnly.bmf");
		App::GetApp()->RegisterResource(L"ObjectOnly_MESH_WITH_TAN", staticMultiModelMesh);


	}

	void Scene::OnCreate(){
		try {
			//���\�[�X�쐬
			CreateResourses();
			//�������g�ɃC�x���g�𑗂�
			//����ɂ��e�X�e�[�W��I�u�W�F�N�g��Create���ɃV�[���ɃA�N�Z�X�ł���
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//�ŏ��̃A�N�e�B�u�X�e�[�W�̐ݒ�
			ResetActiveStage<GameStage>();
		}
	}



}
//end basecross
