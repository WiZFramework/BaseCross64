/*!
@file CSUpdate.h
@brief CSによる更新処理
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"
#include "CSUpdate.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct CSCollisionManager::Impl;
	//--------------------------------------------------------------------------------------
	struct CSCollisionManager::Impl {
		Impl() {}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	CSによる衝突判定
	//--------------------------------------------------------------------------------------
	CSCollisionManager::CSCollisionManager():
		ObjectInterface(),
		pImpl(new Impl())
	{}
	CSCollisionManager::~CSCollisionManager(){}

	void CSCollisionManager::OnCreate() {

	}

}
//end basecross
