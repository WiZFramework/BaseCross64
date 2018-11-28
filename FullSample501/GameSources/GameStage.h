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
		//�Œ�̃{�b�N�X�̍쐬
		virtual void CreateFixedBox() = 0;
		//��Q�����̍쐬
		virtual void CreateFixedSphere() = 0;
		//�ǂ�������I�u�W�F�N�g�̍쐬
		virtual void CreateSeekObject() = 0;
		//�ǐՂ���I�u�W�F�N�g�̍쐬
		virtual void CreatePursuitObject() = 0;
		//�v���C���[�̍쐬
		virtual void CreatePlayer() = 0;
	public:
		//������
		virtual void OnCreate() override;
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
		//�Œ�̃{�b�N�X�̍쐬
		virtual void CreateFixedBox() override;
		//��Q�����̍쐬
		virtual void CreateFixedSphere() override;
		//�ǂ�������I�u�W�F�N�g�̍쐬
		virtual void CreateSeekObject() override;
		//�ǐՂ���I�u�W�F�N�g�̍쐬
		virtual void CreatePursuitObject() override;
		//�v���C���[�̍쐬
		virtual void CreatePlayer() override;

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
		//�Œ�̃{�b�N�X�̍쐬
		virtual void CreateFixedBox() override;
		//��Q�����̍쐬
		virtual void CreateFixedSphere() override;
		//�ǂ�������I�u�W�F�N�g�̍쐬
		virtual void CreateSeekObject() override;
		//�ǐՂ���I�u�W�F�N�g�̍쐬
		virtual void CreatePursuitObject() override;
		//�v���C���[�̍쐬
		virtual void CreatePlayer() override;
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

