/*!
@file ProjectBehavior.h
@brief �v���W�F�N��`�̍s���N���X
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�R���g���[���̃{�^���̃n���h��
	//--------------------------------------------------------------------------------------
	template<typename T>
	struct InputHandler {
		void PushHandle(const shared_ptr<T>& Obj) {
			//�R���g���[���̎擾
			auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (cntlVec[0].bConnected) {
				//A�{�^��
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					Obj->OnPushA();
				}
				//X�{�^��
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X) {
					Obj->OnPushX();
				}
			}
		}
	};

}

//end basecross

