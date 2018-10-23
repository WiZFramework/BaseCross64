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
		//アセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);


		//四角形オブジェクトの作成
		wstring strSkyTexture = DataDir + L"sky.jpg";
		m_SkySquareSprite = ObjectFactory::Create<SquareSprite>(strSkyTexture, false, Vec2(-100.0f, 0.f));

		//四角形オブジェクトの作成
		wstring strTraceTexture = DataDir + L"trace.png";
		m_TraceSquareSprite = ObjectFactory::Create<SquareSprite>(strTraceTexture, true, Vec2(100.0f, 0.f));
	}

	void Scene::OnUpdate() {
		m_SkySquareSprite->OnUpdate();
		m_TraceSquareSprite->OnUpdate();

	}

	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Device = App::GetApp()->GetDeviceResources();
		Device->ClearDefaultViews(Col4(0, 0, 0, 1.0));
		
		//デフォルト描画の開始
		Device->StartDefaultDraw();
		
		//四角形の描画
		m_SkySquareSprite->OnDraw();
		m_TraceSquareSprite->OnDraw();
		
		//デフォルト描画の終了
		Device->EndDefaultDraw();
	}
}
//end basecross
