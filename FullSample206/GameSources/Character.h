/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//�\�z�Ɣj��
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};

	//--------------------------------------------------------------------------------------
	///	���˂��鋅��
	//--------------------------------------------------------------------------------------
	class FireSphere : public GameObject {
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		float m_Scale;
		float m_ActiveMaxY;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W
		@param[in]	Emitter	���ˈʒu
		@param[in]	Velocity	���x
		*/
		//--------------------------------------------------------------------------------------
		FireSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
		);
		virtual ~FireSphere();
		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnCollisionEnter(const CollisionPair& Pair) override;
		virtual void OnCollisionExcute(const CollisionPair& Pair) override;
		//���̂����Z�b�g����
		void Reset(const Vec3& Emitter, const Vec3& Velocity);
	};


}
//end basecross
