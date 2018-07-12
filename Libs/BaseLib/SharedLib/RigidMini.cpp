/*!
@file RigidMini.cpp
@brief �ȈՕ����v�Z�R���|�[�l���g
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	 �ȈՕ����v�Z�R���|�[�l���g
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
