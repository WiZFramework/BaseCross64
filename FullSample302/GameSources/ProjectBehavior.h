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
			float elapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += elapsedTime;
			if (m_TotalTime >= XM_2PI) {
				m_TotalTime = 0;
			}
			auto ptrDraw = GetGameObject()->GetDynamicComponent<T2>();
			const vector<T>& backupVec = ptrDraw->GetOriginalMeshResource()->GetBackupVerteces<T>();
			vector<T> newVec;
			for (auto& v : backupVec) {
				T newV;
				newV = v;
				auto len = (sin(m_TotalTime) * 0.5f) + 1.0f;
				newV.position.x *= len;
				newV.position.z *= len;
				newVec.push_back(newV);
			}
			ptrDraw->UpdateVertices(newVec);
		}

	};


}

//end basecross

