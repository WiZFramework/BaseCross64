/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	struct CB
	{
		Mat4x4 m_WorldMatrix;
		Vec4 m_PlayerPosition;
		// paramf[0] == elapsedTime
		// paramf[1] == velocityPower
		// paramf[2] == gravity_y
		// paramf[3] == baseY
		float paramf[4];
	};


	DECLARE_DX11_COMPUTE_SHADER(ComputeSaderCalcbody)

	DECLARE_DX11_CONSTANT_BUFFER(ConstantBufferCalcbody, CB)


	//--------------------------------------------------------------------------------------
	//�@�_�ō\�����ꂽ����
	//-----------------------------------------------ix---------------------------------------
	class PointsBall : public GameObject {
		const UINT NUM_ELEMENTS = 64 * 32;
		UINT m_NumVertices;
		struct LocalData {
			Vec4 m_LocalPosition;
			Vec4 m_WorldPosition;
			Vec4 m_Velocity;
			UINT m_State[4];
			LocalData() :
				m_LocalPosition(0.0f),
				m_WorldPosition(0.0f),
				m_Velocity(0.0f)
			{
				for (auto& v : m_State) {
					v = 0;
				}
			}
		};
		//�G�������g�o�b�t�@
		ComPtr<ID3D11Buffer> m_InBuffer;
		ComPtr<ID3D11Buffer> m_OutBuffer;
		//�V�F�[�_�[���\�[�X�r���[
		ComPtr<ID3D11ShaderResourceView> m_SRV;
		//�A�N�Z�X�r���[
		ComPtr < ID3D11UnorderedAccessView>  m_UAV;
		//���[�h�o�b�N�o�b�t�@
		ComPtr<ID3D11Buffer> m_ReadBackBuffer;
		vector<LocalData> m_LocalDataVec;
		vector<LocalData> m_ResultDataVec;
		vector<Mat4x4> m_MatVec;
		float m_Scale;
		Vec3 m_Position;
		//���b�V��
		shared_ptr<MeshResource> m_MeshRes;
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
		virtual void OnUpdate() override;
	};


}
//end basecross
