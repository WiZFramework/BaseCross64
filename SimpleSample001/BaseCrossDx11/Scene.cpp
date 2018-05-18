/*!
@file Scene.cpp
@brief �V�[���Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate() {
		wstring DataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(DataDir);

		wstring strTexture = DataDir + L"sky.jpg";
		//�����̂̍쐬
		m_CubeObject1 = ObjectFactory::Create<CubeObject>(strTexture, false, Vec3(-1.0f, 0, 2.0f), true);
		m_CubeObject2 = ObjectFactory::Create<CubeObject>(strTexture, false, Vec3(1.0f, 0, 2.0f), false);
		strTexture = DataDir + L"trace.png";
		//�����̂̍쐬
		m_CubeObject3 = ObjectFactory::Create<CubeObject>(strTexture, true, Vec3(-2.0f, 0, 0), true);
		m_CubeObject4 = ObjectFactory::Create<CubeObject>(strTexture, true, Vec3(2.0f, 0, 0), false);
	}

	void Scene::OnUpdate() {
		m_CubeObject1->OnUpdate();
		m_CubeObject2->OnUpdate();
		m_CubeObject3->OnUpdate();
		m_CubeObject4->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		m_CubeObject1->OnDraw();
		m_CubeObject2->OnDraw();
		m_CubeObject3->OnDraw();
		m_CubeObject4->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}

}
//end basecross
