/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	class MyCamera;
	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase {
		shared_ptr<MyCamera> m_BackupCamera;
		Vec3 m_BackupPlayerPos;
		//--------------------------------------------------------------------------------------
		/*!
		@brief リソースの作成
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CreateResourses();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;

		shared_ptr<MyCamera> GetBackupCamera() const {
			return m_BackupCamera;
		}
		void SetBackupCamera(const shared_ptr<MyCamera>& camara) {
			m_BackupCamera = camara;
		}

		Vec3 GetBackupPlayerPos() const {
			return m_BackupPlayerPos;
		}
		void SetBackupPlayerPos(const Vec3& pos) {
			m_BackupPlayerPos = pos;
		}
	};

}

//end basecross
