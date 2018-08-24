/*!
@file Scene.h
@brief シーンなど
*/
#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneInterface {
		shared_ptr<CubeObject> m_CubeObject1;				///<立方体オブジェクト1
		shared_ptr<CubeObject> m_CubeObject2;				///<立方体オブジェクト2
		shared_ptr<CubeObject> m_CubeObject3;				///<立方体オブジェクト3
		shared_ptr<CubeObject> m_CubeObject4;				///<立方体オブジェクト4
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneInterface() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


}
// end basecross
