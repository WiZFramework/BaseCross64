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
		m_CubeObject = ObjectFactory::Create<CubeObject>();
	}

	void Scene::OnUpdate() {
		m_CubeObject->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		m_CubeObject->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}

}
//end basecross
