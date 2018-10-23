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

		//�l�p�`�̍쐬
		wstring strTexture = DataDir + L"sky.jpg";
		m_SkySquareSprite = ObjectFactory::Create<SquareSprite>(strTexture, false, Vec2(-100.0f, 0));
		
		strTexture = DataDir + L"trace.png";
		m_TileSquareSprite = ObjectFactory::Create<SquareSprite>(strTexture, true, Vec2(100.0f, 0));

		
	}

	void Scene::OnUpdate() {
		m_SkySquareSprite->OnUpdate();
		m_TileSquareSprite->OnUpdate();
	}

	void Scene::OnDraw() {
		//�`��f�o�C�X�̎擾
		auto Device = App::GetApp()->GetDeviceResources();
		Device->ClearDefaultViews(Col4(0, 0, 0, 1.0));
		
		//�f�t�H���g�`��̊J�n
		Device->StartDefaultDraw();
		
		//�l�p�`�̕`��
		m_SkySquareSprite->OnDraw();
		m_TileSquareSprite->OnDraw();
		//�f�t�H���g�`��̏I��
		Device->EndDefaultDraw();
	}
}
//end basecross
