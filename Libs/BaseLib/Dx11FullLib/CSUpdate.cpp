/*!
@file CSUpdate.h
@brief CSÇ…ÇÊÇÈçXêVèàóù
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
	///	CSÇ…ÇÊÇÈè’ìÀîªíË
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
