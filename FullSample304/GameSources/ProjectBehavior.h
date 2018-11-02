/*!
@file ProjectBehavior.h
@brief プロジェク定義の行動クラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	コントローラのボタンのハンドラ
	//--------------------------------------------------------------------------------------
	template<typename T>
	struct InputHandler {
		void PushHandle(const shared_ptr<T>& Obj) {
			//コントローラの取得
			auto cntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (cntlVec[0].bConnected) {
				//Aボタン
				if (cntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					Obj->OnPushA();
				}
			}
		}
	};

	//--------------------------------------------------------------------------------------
	///頂点を変更する行動クラス
	//--------------------------------------------------------------------------------------
	class VertexBehavior : public Behavior {
		float m_TotalTime;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		VertexBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr),
			m_TotalTime(0)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~VertexBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 伸び縮みする
		@tparam	T	頂点の型
		@return	なし
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

