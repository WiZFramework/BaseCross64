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
	//	�l�p�`�X�v���C�g
	//--------------------------------------------------------------------------------------
	class SquareSprite : public ObjectInterface, public ShapeInterface {

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		SquareSprite();

		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSprite();

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

	private:
		// ���b�V��
		shared_ptr<MeshResource> m_SquareMesh;

		Vec2 m_Scale;				///<�X�P�[�����O
		float m_Rot;				///<��]�p�x
		Vec2 m_Pos;					///<�ʒu
		Vec2 m_PosSpan;				///<�ʒu�ύX�Ԋu

		//�R���X�^���g�o�b�t�@�X�V
		void UpdateConstantBuffer();

		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;

		//CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };

		//�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;

		//GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

		//�R���X�^���g�o�b�t�@
		struct SpriteConstantBuffer
		{
			Mat4x4 World;
			Col4 Emissive;
			SpriteConstantBuffer() {
				memset(this, 0, sizeof(SpriteConstantBuffer));
			};
		};

		///�R���X�^���g�o�b�t�@�f�[�^
		SpriteConstantBuffer m_SpriteConstantBuffer;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		///�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
	


		//���[�g�V�O�l�`���쐬
		void CreateRootSignature();

		//�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();

		//�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();

		//�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();

		//�R�}���h���X�g�쐬
		void CreateCommandList();

		//�`�揈��
		void DrawObject();
	};
}
//end basecross