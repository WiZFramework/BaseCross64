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
		m_TriangleObject = ObjectFactory::Create<TriangleObject>();
	}

	void Scene::OnUpdate() {
		m_TriangleObject->OnUpdate();
	}
	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//�f�t�H���g�`��̊J�n
		Dev->StartDefaultDraw();
		//�O�p�I�u�W�F�N�g�̕`��J�n
		m_TriangleObject->OnDraw();
		//�f�t�H���g�`��̏I��
		Dev->EndDefaultDraw();
	}

}
//end basecross
