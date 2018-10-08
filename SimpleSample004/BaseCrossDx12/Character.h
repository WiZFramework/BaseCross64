/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
@copyright shike.y
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	//--------------------------------------------------------------------------------------
	//	�l�p�`�X�v���C�g(�\����)
	//--------------------------------------------------------------------------------------
	struct SquareSprite
	{
		Vec2	m_LocalScale;			//���[�J���X�P�[��
		float	m_LocalRot;				//���[�J����]�p�x
		float	m_LocalRotVelocity;		//���[�J����]���x
		Vec2	m_LocalPos;				//���[�J���ʒu
		Vec2	m_LocalVelocity;		//���[�J�����x
		Vec2	m_LocalGravityVelocity;	//���[�J�������x
		Col4	m_RandomVertexColor;			//���[�J�����_�J���[

		//CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };

		//�f�X�N���v�^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;

		//GPU���f�X�N���v�^�̃n���h���z��
		vector<D3D12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

		//�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;

		//�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };

		//�R���X�^���g�o�b�t�@
		struct SpriteConstantBuffer
		{
			Mat4x4 World;
			Col4 Emissive;
			SpriteConstantBuffer()
			{
				memset(this, 0, sizeof(SpriteConstantBuffer));
			};
		};

		//�R���X�^���g�o�b�t�@�f�[�^
		SpriteConstantBuffer m_SpriteConstantBuffer;
		SquareSprite() :
			m_LocalScale(64.f, 64.f),
			m_LocalRot(0.f),
			m_LocalRotVelocity(0.f),
			m_LocalPos(0.f, 0.f),
			m_LocalVelocity(0.f, 0.f),
			m_LocalGravityVelocity(0.f, 0.f),
			m_RandomVertexColor(0.0f, 0.0f, 0.0f, 0.0f)
		{
		}

		//�f�X�N���v�^�q�[�v�̍쐬
		void CreateDescriptorHeap();

		//�R���X�^���g�o�b�t�@�̍쐬
		void CreateConstantBuffer();

		//�R���X�^���g�o�b�t�@�̍X�V
		void UpdateConstantBuffer();
	};
	

	//--------------------------------------------------------------------------------------
	//	�l�p�`�X�v���C�g�̃I�u�W�F�N�g�O���[�v
	//--------------------------------------------------------------------------------------
	class SquareSpriteGroup : public ObjectInterface, public ShapeInterface
	{

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		SquareSpriteGroup();

		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSpriteGroup();

		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw() override;


	private:
		//���[�g�V�O�l�`���̍쐬
		void CreateRootSignature();

		//�f�X�N���v�^�q�[�v�̍쐬
		void CreateDescriptorHeap();

		//�R���X�^���g�o�b�t�@�̍쐬
		void CreateConstantBuffer();

		//�p�C�v���C���X�e�[�g�̍쐬
		void CreatePipelineState();

		//�R�}���h���X�g�̍쐬
		void CreateCommandList();

		//�`�揈��
		void DrawObject();

		//���b�V��
		shared_ptr<MeshResource> m_SquareSpriteMesh;

		//�I�u�W�F�N�g�Ǘ��R���e�i
		vector<SquareSprite> m_SquareSpriteVec;

		float m_TotalTime;

		//�e�I�u�W�F�N�g�̈ʒu���̕ύX
		void UpdateObjects(float ElapsedTime);

		//�R���X�^���g�o�b�t�@�̍X�V
		void UpdateConstantBuffer();

		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;

		//�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;

		//�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

	
	};


}
//end basecross