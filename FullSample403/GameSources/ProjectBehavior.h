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
		void ButtonHandle(const shared_ptr<T>& Obj) {
			//�R���g���[���̎擾
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntlVec[0].bConnected) {
				//A�{�^��
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					Obj->OnPushA();
				}
				//B�{�^��
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
					Obj->OnPushB();
				}
				//X�{�^��
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X) {
					Obj->OnPushX();
				}
				//Y�{�^���������u��
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_Y) {
					Obj->OnPushY();
				}
				//Y�{�^���������ςȂ�
				else if (CntlVec[0].wButtons & XINPUT_GAMEPAD_Y) {
					Obj->OnPressY();
				}
				//Y�{�^������
				else if (CntlVec[0].wReleasedButtons & XINPUT_GAMEPAD_Y) {
					Obj->OnReleaseY();
				}
			}
		}
	};


}

//end basecross

