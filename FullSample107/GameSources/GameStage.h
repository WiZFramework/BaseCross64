/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	���[�r�[�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class MyMovieStage : public MovieStage {
	public:
		//�\�z�Ɣj��
		MyMovieStage() :MovieStage() {}
		virtual ~MyMovieStage() {}
		//������
		virtual void OnCreate()override;
	};



}
//end basecross

