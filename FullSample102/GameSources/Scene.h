/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	class MyCamera;
	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase {
		shared_ptr<MyCamera> m_BackupCamera;
		Vec3 m_BackupPlayerPos;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���\�[�X�̍쐬
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CreateResourses();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C�x���g�擾
		@return	�Ȃ�
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
