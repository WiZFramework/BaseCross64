/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {



	//--------------------------------------------------------------------------------------
	///	������
	//--------------------------------------------------------------------------------------
	class CubeObject : public ObjectInterface, public ShapeInterface {
		//���b�V��
		shared_ptr<MeshResource> m_CubeMesh;
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
		Vec3 m_Scale;				///<�X�P�[�����O
		Quat m_Qt;			///<��]
		Vec3 m_Pos;				///<�ʒu
		bool m_Trace;					///<�����������邩�ǂ���
		bool m_Flat;				///<�t���b�g�\�����邩�ǂ���
		void CreateBuffers();
		void UpdateConstantBuffer();

		///���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrv�̃f�X�N�v���^�n���h���̃C���N�������g�T�C�Y
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///�f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU���f�X�N�v���^�̃n���h���̔z��
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

		///�R���X�^���g�o�b�t�@
		struct StaticConstantBuffer
		{
			Mat4x4 World;
			Mat4x4 View;
			Mat4x4 Projection;
			Vec4 LightDir;
			Col4 Emissive;
			Col4 Diffuse;
			StaticConstantBuffer() {
				memset(this, 0, sizeof(StaticConstantBuffer));
			};
		};
		///�R���X�^���g�o�b�t�@�̃I�u�W�F�N�g���ϐ�
		StaticConstantBuffer m_StaticConstantBuffer;
		///�R���X�^���g�o�b�t�@�A�b�v���[�h�q�[�v
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///�R���X�^���g�o�b�t�@��GPU���ϐ�
		void* m_pConstantBuffer{ nullptr };
		///�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_CullBackPipelineState;
		ComPtr<ID3D12PipelineState> m_CullFrontPipelineState;
		///�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;


		///�e�������֐�
		///���[�g�V�O�l�`���쐬
		void CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		void CreateDescriptorHeap();
		///�T���v���[�쐬
		void CreateSampler();
		///�V�F�[�_�[���\�[�X�r���[�쐬
		void CreateShaderResourceView();
		///�R���X�^���g�o�b�t�@�쐬
		void CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		void CreatePipelineState();
		///�R�}���h���X�g�쐬
		void CreateCommandList();

		///�`�揈��
		void DrawObject();

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	Pos	�ʒu
		@param[in]	Flat	�t���b�g�\���ɂ��邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		CubeObject(const wstring& TextureFileName, bool Trace, const Vec3& Pos, bool Flat);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CubeObject();
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
//end basecross
