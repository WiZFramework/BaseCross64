/*!
@file RigidMini.cpp
@brief 簡易物理計算コンポーネント
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	 簡易物理計算コンポーネント
	//--------------------------------------------------------------------------------------
	RigidMini::RigidMini(const shared_ptr<GameObject>& GameObjectPtr):
		Component(GameObjectPtr)
	{}
	RigidMini::~RigidMini() {}

	void RigidMini::OnCreate() {
	}

	shared_ptr<RmManager> RigidMini::GetRmManager() const {
		return GetGameObject()->GetStage()->GetRmManager();
	}



}
//end basecross
