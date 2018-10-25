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
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntlVec[0].bConnected) {
				//A�{�^��
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					Obj->OnPushA();
				}
			}
		}
	};

	//--------------------------------------------------------------------------------------
	///���_��ύX����s���N���X
	//--------------------------------------------------------------------------------------
	class VertexBehavior : public Behavior {
		float m_TotalTime;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	���̃R���|�[�l���g����������Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		VertexBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr),
			m_TotalTime(0)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~VertexBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �L�яk�݂���
		@tparam	T	���_�̌^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename T2>
		void ExpandAndContract() {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += ElapsedTime;
			if (m_TotalTime >= XM_2PI) {
				m_TotalTime = 0;
			}
			auto PtrDraw = GetGameObject()->GetDynamicComponent<T2>();
			const vector<T>& BackupVec = PtrDraw->GetOriginalMeshResource()->GetBackupVerteces<T>();
			vector<T> new_vec;
			for (auto& v : BackupVec) {
				T new_v;
				new_v = v;
				auto Len = (sin(m_TotalTime) * 0.5f) + 1.0f;
				new_v.position.x *= Len;
				new_v.position.z *= Len;
				new_vec.push_back(new_v);
			}
			PtrDraw->UpdateVertices(new_vec);
		}

	};


}

//end basecross

