/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�e�t�@�N�g���[
	//--------------------------------------------------------------------------------------
	class AbstractFactory : public GameObject {
	protected:
		//�\�z�Ɣj��
		AbstractFactory(const shared_ptr<Stage>& StagePtr)
			:GameObject(StagePtr) {}
		virtual ~AbstractFactory() {}
		//�r���[�̍쐬
		void CreateViewLight();
		//�v���C���[�̍쐬
		void CreatePlayer();
	};

	//--------------------------------------------------------------------------------------
	//	�t�@�N�g���[1
	//--------------------------------------------------------------------------------------
	class Factory1 : public AbstractFactory {
	public:
		//�\�z�Ɣj��
		Factory1(const shared_ptr<Stage>& StagePtr) :
			AbstractFactory(StagePtr)
		{}
		virtual ~Factory1() {}
		//������
		virtual void OnCreate() override;
	};

	//--------------------------------------------------------------------------------------
	//	�t�@�N�g���[2
	//--------------------------------------------------------------------------------------
	class Factory2 : public AbstractFactory {
	public:
		//�\�z�Ɣj��
		Factory2(const shared_ptr<Stage>& StagePtr) :
			AbstractFactory(StagePtr)
		{}
		virtual ~Factory2() {}
		//������
		virtual void OnCreate() override;
	};


	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
	};


}
//end basecross

