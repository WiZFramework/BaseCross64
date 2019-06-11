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
		//�A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);


		//�l�p�`�I�u�W�F�N�g�̍쐬
		wstring strSkyTexture = DataDir + L"sky.jpg";
		m_SkySquareSprite = ObjectFactory::Create<SquareSprite>(strSkyTexture, false, Vec2(-100.0f, 0.f));

		//�l�p�`�I�u�W�F�N�g�̍쐬
		wstring strTraceTexture = DataDir + L"trace.png";
		m_TraceSquareSprite = ObjectFactory::Create<SquareSprite>(strTraceTexture, true, Vec2(100.0f, 0.f));
	}

	void Scene::OnUpdate() {
		m_SkySquareSprite->OnUpdate();
		m_TraceSquareSprite->OnUpdate();

	}

	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Device = App::GetApp()->GetDeviceResources();
		Device->ClearDefaultViews(Col4(0, 0, 0, 1.0));
		
		//�f�t�H���g�`��̊J�n
		Device->StartDefaultDraw();
		
		//�l�p�`�̕`��
		m_SkySquareSprite->OnDraw();
		m_TraceSquareSprite->OnDraw();
		
		//�f�t�H���g�`��̏I��
		Device->EndDefaultDraw();
	}
}
//end basecross
