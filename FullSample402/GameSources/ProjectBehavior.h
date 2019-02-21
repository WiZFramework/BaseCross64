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
			//�L�[�{�[�h�̎擾�i�D��j
			auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
			if (KeyState.m_bPressedKeyTbl[VK_SPACE]) {
				Obj->OnPushA();
				return;
			}
			if (KeyState.m_bPressedKeyTbl['B']) {
				Obj->OnPushB();
				return;
			}
			if (KeyState.m_bPressedKeyTbl['X']) {
				Obj->OnPushX();
				return;
			}
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
			}
		}
	};


}

//end basecross

