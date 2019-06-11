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
		//四角形オブジェクトの作成
		m_SquareSprite = ObjectFactory::Create<SquareSprite>();
	}

	void Scene::OnUpdate() {
		m_SquareSprite->OnUpdate();
	}

	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Device = App::GetApp()->GetDeviceResources();
		Device->ClearDefaultViews(Col4(0, 0, 0, 1.0));
		
		//デフォルト描画の開始
		Device->StartDefaultDraw();
		
		//四角形の描画
		m_SquareSprite->OnDraw();
		
		//デフォルト描画の終了
		Device->EndDefaultDraw();
	}
}
//end basecross
