
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
		wstring DataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = DataDir + L"number.png";
		App::GetApp()->RegisterTexture(L"NUMBER_TX", strTexture);

		//�{�[�����f���̒ʏ탊�\�[�X
		auto ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"Chara_R_MESH", ModelMesh);

		//�{�[�����f���̃^���W�F���g�t�����\�[�X
		ModelMesh = MeshResource::CreateBoneModelMeshWithTangent(DataDir, L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"Chara_R_MESH_WITH_TAN", ModelMesh);
		//�@���}�b�v
		strTexture = DataDir + L"Chara_R_narmal.png";
		App::GetApp()->RegisterTexture(L"Chara_R_NORMAL_TX", strTexture);

		//�{�[�����f��(�}���`���b�V��)�̒ʏ탊�\�[�X
		auto MultiModelMesh = MultiMeshResource::CreateBoneModelMultiMesh(DataDir, L"Object_WalkAnimation.bmf");
		App::GetApp()->RegisterResource(L"Object_WalkAnimation_MESH", MultiModelMesh);

		//�{�[�����f��(�}���`���b�V��)�̃^���W�F���g�t�����\�[�X
		MultiModelMesh = MultiMeshResource::CreateBoneModelMultiMeshWithTangent(DataDir, L"Object_WalkAnimation.bmf");
		App::GetApp()->RegisterResource(L"Object_WalkAnimation_MESH_WITH_TAN", MultiModelMesh);

		//�@���}�b�v
		strTexture = DataDir + L"Tx_Checker_Normal.png";
		App::GetApp()->RegisterTexture(L"OBJECT_NORMAL_TX", strTexture);


		//�X�^�e�B�b�N���f���̒ʏ탊�\�[�X
		auto StaticModelMesh = MeshResource::CreateStaticModelMesh(DataDir, L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH", StaticModelMesh);
		//�X�^�e�B�b�N���f���̃^���W�F���g�t�����\�[�X
		StaticModelMesh = MeshResource::CreateStaticModelMeshWithTangent(DataDir, L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH_WITH_TAN", StaticModelMesh);
		//�@���}�b�v
		strTexture = DataDir + L"Character_2_normal.png";
		App::GetApp()->RegisterTexture(L"MODEL_NORMAL_TX", strTexture);

		//�X�^�e�B�b�N���f��(�}���`���b�V��)�̒ʏ탊�\�[�X
		auto StaticMultiModelMesh = MultiMeshResource::CreateStaticModelMultiMesh(DataDir, L"ObjectOnly.bmf");
		App::GetApp()->RegisterResource(L"ObjectOnly_MESH", StaticMultiModelMesh);

		//�X�^�e�B�b�N���f��(�}���`���b�V��)�̃^���W�F���g�t�����\�[�X
		StaticMultiModelMesh = MultiMeshResource::CreateStaticModelMultiMeshWithTangent(DataDir, L"ObjectOnly.bmf");
		App::GetApp()->RegisterResource(L"ObjectOnly_MESH_WITH_TAN", StaticMultiModelMesh);


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
