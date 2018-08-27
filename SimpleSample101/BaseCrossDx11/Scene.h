/*!
@file Scene.h
@brief �V�[���Ȃ�
*/
#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneInterface {
		shared_ptr<CubeObject> m_CubeObject1;				///<�����̃I�u�W�F�N�g1
		shared_ptr<CubeObject> m_CubeObject2;				///<�����̃I�u�W�F�N�g2
		shared_ptr<CubeObject> m_CubeObject3;				///<�����̃I�u�W�F�N�g3
		shared_ptr<CubeObject> m_CubeObject4;				///<�����̃I�u�W�F�N�g4
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneInterface() {}
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
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


}
// end basecross