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
		//�l�p�`�I�u�W�F�N�g�̍쐬
		m_SquareSprite = ObjectFactory::Create<SquareSprite>();
	}

	void Scene::OnUpdate() {
		m_SquareSprite->OnUpdate();
	}

	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Device = App::GetApp()->GetDeviceResources();
		Device->ClearDefaultViews(Col4(0, 0, 0, 1.0));
		
		//�f�t�H���g�`��̊J�n
		Device->StartDefaultDraw();
		
		//�l�p�`�̕`��
		m_SquareSprite->OnDraw();
		
		//�f�t�H���g�`��̏I��
		Device->EndDefaultDraw();
	}
}
//end basecross
