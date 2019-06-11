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
		//�����̂̍쐬
		m_CubeObject1 = ObjectFactory::Create<CubeObject>(Vec3(-1.0f, 0, 0), false);
		m_CubeObject2 = ObjectFactory::Create<CubeObject>(Vec3(1.0f, 0, 0), true);
	}

	void Scene::OnUpdate() {
		m_CubeObject1->OnUpdate();
		m_CubeObject2->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		m_CubeObject1->OnDraw();
		m_CubeObject2->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}

}
//end basecross
