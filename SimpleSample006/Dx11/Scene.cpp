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
		wstring DataDir;
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);

		//四角形の作成
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
		//描画デバイスの取得
		auto Device = App::GetApp()->GetDeviceResources();
		Device->ClearDefaultViews(Col4(0, 0, 0, 1.0));
		
		//デフォルト描画の開始
		Device->StartDefaultDraw();
		
		//四角形の描画
		m_SkySquareSprite->OnDraw();
		m_TileSquareSprite->OnDraw();
		//デフォルト描画の終了
		Device->EndDefaultDraw();
	}
}
//end basecross
