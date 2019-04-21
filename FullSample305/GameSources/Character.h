/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//�@�_�ō\�����ꂽ����
	//-----------------------------------------------ix---------------------------------------
	class PointsBall : public GameObject {
		enum class LocalState {
			Roll,
			Down,
			Fix
		};
		struct LocalData {
			LocalState m_State;
			Vec3 m_LocalPosition;
			Mat4x4 m_FixedMatrix;
			Vec3 m_Velocity;
			LocalData() :
				m_State(LocalState::Roll),
				m_LocalPosition(0.0f),
				m_FixedMatrix(),
				m_Velocity(0.0f)
			{}
		};
		vector<LocalData> m_LocalDataVec;
		vector<Mat4x4> m_MatVec;
		float m_Scale;
		Vec3 m_Position;
		//���b�V��
		shared_ptr<MeshResource> m_MeshRes;
		//�X�e�[�g��Update
		void UpdateState();
		//�e�C���X�^���X��Update
		void UpdateInstances();
	public:
		//�\�z�Ɣj��
		PointsBall(const shared_ptr<Stage>& StagePtr,
			float Scale,
			const Vec3& Position
		);
		virtual ~PointsBall();
		//������
		virtual void OnCreate() override;
		//����
		virtual void OnUpdate() override;
	};

	//--------------------------------------------------------------------------------------
	//�@�^�C�����O����Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//�\�z�Ɣj��
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//������
		virtual void OnCreate() override;
		//����
	};


}
//end basecross
