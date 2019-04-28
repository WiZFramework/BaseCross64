/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	//�@�R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	struct CB
	{
		// paramf[0] == Totaltime
		float paramf[4];
	};
	//�R���X�^���g�o�b�t�@�̃w�b�_��
	DECLARE_DX11_CONSTANT_BUFFER(ConstantBufferCalcbody, CB)

	//--------------------------------------------------------------------------------------
	//�@�R���s���[�g�V�F�[�_�[
	//--------------------------------------------------------------------------------------
	//CS�̃w�b�_��
	DECLARE_DX11_COMPUTE_SHADER(ComputeSaderCalcbody)


	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject {
		// �G�������g�f�[�^�̍\����
		struct Element
		{
			XMFLOAT4 pos;
		};
		//�G�������g�o�b�t�@
		ComPtr<ID3D11Buffer> m_Buffer;
		//�A�N�Z�X�r���[
		ComPtr < ID3D11UnorderedAccessView>  m_UAV;
		//���[�h�o�b�N�o�b�t�@
		ComPtr<ID3D11Buffer> m_ReadBackBuffer;
		//�g�[�^���A�C��
		float m_TotalTime;
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		Vec3 m_Velocity;
	public:
		//�\�z�Ɣj��
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~MoveBox();
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
	};


}
//end basecross
