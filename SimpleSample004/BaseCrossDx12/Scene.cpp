/*!
@file Scene.cpp
@brief シーンなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate() {
		//四角形スプライトグループの作成
		m_SquareSpriteGroup = ObjectFactory::Create<SquareSpriteGroup>();
	}

	void Scene::OnUpdate() {
		m_SquareSpriteGroup->OnUpdate();
	}

	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Device = App::GetApp()->GetDeviceResources();
		Device->ClearDefaultViews(Col4(0, 0, 0, 1.0));
		
		//デフォルト描画の開始
		Device->StartDefaultDraw();
		
		//四角形スプライトグループの描画(内部で複数のスプライト描画を行っている)
		m_SquareSpriteGroup->OnDraw();

		//デフォルト描画の終了
		Device->EndDefaultDraw();
	}
}
//end basecross
