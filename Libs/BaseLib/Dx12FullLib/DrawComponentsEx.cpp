/*!
@file DrawComponentsEx.cpp
@brief �`��R���|�[�l���g�g������
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"
#include <pix.h>


namespace basecross {

	//���_�V�F�[�_
	//�e����
	IMPLEMENT_DX12SHADER(BcVSPNTStaticVL, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticVL.cso")
	//�e�t��
	IMPLEMENT_DX12SHADER(BcVSPNTStaticVLShadow, App::GetApp()->GetShadersPath() + L"BcVSPNTStaticVLShadow.cso")

	//�s�N�Z���V�F�[�_
	//�e����
	IMPLEMENT_DX12SHADER(BcPSPNTVL, App::GetApp()->GetShadersPath() + L"BcPSPNTVL.cso")
	//�e�t��
	IMPLEMENT_DX12SHADER(BcPSPNTVLShadow, App::GetApp()->GetShadersPath() + L"BcPSPNTVLShadow.cso")

	//--------------------------------------------------------------------------------------
	///	Bc�`��Ɏg�p����\����(�e�Ή�)
	//--------------------------------------------------------------------------------------
	struct BcDrawObject :public DrawObjectBase {
		//���_�ύX����ꍇ�̃��b�V���i�I���W�i���j
		shared_ptr<MeshResource> m_OriginalMeshResource;
		//�I���W�i�����b�V�����g�����ǂ���
		bool m_UseOriginalMeshResource;
		//�e�N�X�`�����\�[�X
		weak_ptr<TextureResource> m_TextureResource;
		//NormalMap�e�N�X�`��
		weak_ptr<TextureResource> m_NormalMapTextureResource;
		vector<weak_ptr<TextureResource>> m_NormalMapTextureResourceVec;
		//�G�~�b�V�u�F
		bsm::Col4 m_Emissive;
		//�f�t���[�Y�F
		bsm::Col4 m_Diffuse;
		//�����x
		float m_Alpha;
		//�X�y�L�����[�ƃp���[�F
		bsm::Col4 m_SpecularColorAndPower;
		//���C�e�B���O���邩�ǂ���
		bool m_LightingEnabled;
		//�s�N�Z�����C�e�B���O�����邩�ǂ���
		bool m_PreferPerPixelLighting;
		//�@���Ƀo�C�A�X�������邩�ǂ���
		bool m_BiasedNormals;
		//�A���r�G���g
		bsm::Col4 m_AmbientLightColor;
		//���C�g��MAX
		static const int MaxDirectionalLights = 3;
		//���C�g���L�����ǂ���
		bool m_LightEnabled[MaxDirectionalLights];
		//���C�g�̌���
		bsm::Vec3 m_LightDirection[MaxDirectionalLights];
		//���C�g�̃f�t���[�Y�F
		bsm::Col4 m_LightDiffuseColor[MaxDirectionalLights];
		//���C�g�̃X�y�L�����[�F
		bsm::Col4 m_LightSpecularColor[MaxDirectionalLights];
		//�t�H�O���L�����ǂ���
		bool m_FogEnabled;
		//�t�H�O�̊J�n�ʒu
		float m_FogStart;
		//�t�H�O�̏I���ʒu
		float m_FogEnd;
		//�t�H�O�F
		bsm::Col4 m_FogColor;
		//�t�H�O�x�N�g��
		bsm::Vec3 m_FogVector;
		//�e�𓊉e���邩�ǂ���
		bool m_OwnShadowActive;
		//���f���ɓ����Ă���Diffuse���g�����ǂ���
		bool m_ModelDiffusePriority;
		//���f���ɓ����Ă���Emissive���g�����ǂ���
		bool m_ModelEmissivePriority;
		//���f���ɓ����Ă���e�N�X�`�����g�����ǂ���
		bool m_ModelTextureEnabled;
		BcDrawObject() :
			m_UseOriginalMeshResource(false),
			m_Emissive(0.0f, 0.0f, 0.0f, 0.0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_Alpha(1.0f),
			m_SpecularColorAndPower(0.0f, 0.0f, 0.0f, 1.0f),
			m_LightingEnabled(false),
			m_PreferPerPixelLighting(false),
			m_BiasedNormals(false),
			m_AmbientLightColor(0, 0, 0, 0),
			m_FogEnabled(false),
			m_FogStart(-25.0f),
			m_FogEnd(-40.0f),
			m_FogColor(0.8f, 0.8f, 0.8f, 1.0f),
			m_FogVector(0.0, 0.0, 1.0f),
			m_OwnShadowActive(false),
			m_ModelDiffusePriority(true),
			m_ModelEmissivePriority(true),
			m_ModelTextureEnabled(true)
		{
			static const XMVECTORF32 defaultSpecular = { 1, 1, 1, 16 };
			static const bsm::Vec3 defaultLightDirection = { 0, -1, 0 };
			m_SpecularColorAndPower = defaultSpecular;
			for (int i = 0; i < MaxDirectionalLights; i++)
			{
				m_LightDirection[i] = defaultLightDirection;
				m_LightDiffuseColor[i] = g_XMZero;
				m_LightSpecularColor[i] = g_XMZero;
			}
		}
	};

	//--------------------------------------------------------------------------------------
	//	BcBaseDraw::Impl
	//--------------------------------------------------------------------------------------
	struct BcBaseDraw::Impl {
		BcDrawObject m_BcDrawObject;
		Dx12DrawResources<BasicConstants> m_Dx12DrawResources;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�����X�^�e�B�b�N�`��̍쐬
		@tparam[in]	T_Vertex	���_�̌^
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_Vertex,typename T_VShader, typename T_PShader>
		void CreateStaticNotShadow() {
			//���[�g�V�O�l�`��
			m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrvSmpCbv();
			//�f�X�N�v���^�q�[�v
			auto Dev = App::GetApp()->GetDeviceResources();
			{
				m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
					Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
				//�f�X�N�v���^�q�[�v
				m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
				//�T���v���[�f�X�N�v���^�q�[�v
				m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
				//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
				m_Dx12DrawResources.m_GPUDescriptorHandleVec.clear();
				CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
					m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
					0,
					0
				);
				m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SrvHandle);
				CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
					m_Dx12DrawResources.m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
					0,
					0
				);
				m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SamplerHandle);
				CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
					m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
					1,
					m_Dx12DrawResources.m_DescriptorHandleIncrementSize
				);
				m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(CbvHandle);

			}
			//�T���v���[
			{
				auto SamplerDescriptorHandle = m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
				DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
			}
			//�R���X�^���g�o�b�t�@
			{
				//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
				UINT ConstBuffSize = (sizeof(m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;
				//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
				ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
					L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
					L"Dev->GetDevice()->CreateCommittedResource()",
					L"BcPNTStaticDraw::CreateNotShadow()"
				);
				//�R���X�^���g�o�b�t�@�̃r���[���쐬
				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
				cbvDesc.BufferLocation = m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
				cbvDesc.SizeInBytes = ConstBuffSize;
				//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
				//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
				CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
					m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
					1,
					m_Dx12DrawResources.m_DescriptorHandleIncrementSize
				);

				Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
				//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
				CD3DX12_RANGE readRange(0, 0);
				ThrowIfFailed(m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
					reinterpret_cast<void**>(&m_Dx12DrawResources.m_pConstantBuffer)),
					L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
					L"m_ConstantBufferUploadHeap->Map()",
					L"PNTStaticDraw::CreateNotShadow()"
				);

			}
			//�V�F�[�_���\�[�X�r���[�̓e�N�X�`���Z�b�g���ɍ쐬
			//�p�C�v���C���X�e�[�g�̍쐬
			{
				PipelineState::CreateDefault3D<T_Vertex, T_VShader, T_PShader>(m_Dx12DrawResources.m_RootSignature, m_Dx12DrawResources.m_PineLineDesc);
				m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
				m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
				m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(m_Dx12DrawResources.m_PineLineDesc);
				m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
				m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(m_Dx12DrawResources.m_PineLineDesc);
			}
			//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
			{
				m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(m_Dx12DrawResources.m_CullBackPipelineState);
				//�R���X�^���g�o�b�t�@�X�V
				UpdateConstantBuffer();
				CommandList::Close(m_Dx12DrawResources.m_CommandList);
			}
		};


		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�t���X�^�e�B�b�N�`��̍쐬
		@tparam[in]	T_Vertex	���_�̌^
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_Vertex,typename T_VShader, typename T_PShader>
		void CreateStaticWithShadow() {
			//���[�g�V�O�l�`��
			m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrv2Smp2Cbv();
			//�f�X�N�v���^�q�[�v
			auto Dev = App::GetApp()->GetDeviceResources();
			{
				m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
					Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
				m_Dx12DrawResources.m_SamplerDescriptorHandleIncrementSize =
					Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

				//�f�X�N�v���^�q�[�v
				m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 2);
				//�T���v���[�f�X�N�v���^�q�[�v
				m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(2);
				//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
				m_Dx12DrawResources.m_GPUDescriptorHandleVec.clear();
				CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle1(
					m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
					0,
					0
				);
				m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SrvHandle1);
				CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle2(
					m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
					1,
					m_Dx12DrawResources.m_DescriptorHandleIncrementSize
				);
				m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SrvHandle2);

				CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle1(
					m_Dx12DrawResources.m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
					0,
					0
				);
				m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SamplerHandle1);

				CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle2(
					m_Dx12DrawResources.m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
					1,
					m_Dx12DrawResources.m_SamplerDescriptorHandleIncrementSize
				);
				m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SamplerHandle2);

				CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
					m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
					2,
					m_Dx12DrawResources.m_DescriptorHandleIncrementSize
				);
				m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(CbvHandle);
			}
			//�T���v���[
			{
				auto SamplerDescriptorHandleWithShadow1 = m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
				DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandleWithShadow1);

				CD3DX12_CPU_DESCRIPTOR_HANDLE SamplerDescriptorHandleWithShadow2{
					m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
					1,
					m_Dx12DrawResources.m_SamplerDescriptorHandleIncrementSize
				};
				DynamicSampler::CreateSampler(SamplerState::ComparisonLinear, SamplerDescriptorHandleWithShadow2);
			}
			//�R���X�^���g�o�b�t�@
			{
				//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
				UINT ConstBuffSize = (sizeof(m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;
				//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
				ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
					L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
					L"Dev->GetDevice()->CreateCommittedResource()",
					L"PNTStaticDraw::CreateWithShadow()"
				);
				//�R���X�^���g�o�b�t�@�̃r���[���쐬
				D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
				cbvDesc.BufferLocation = m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
				cbvDesc.SizeInBytes = ConstBuffSize;
				//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
				//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
				CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
					m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
					2,
					m_Dx12DrawResources.m_DescriptorHandleIncrementSize
				);

				Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
				//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
				CD3DX12_RANGE readRange(0, 0);
				ThrowIfFailed(m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
					reinterpret_cast<void**>(&m_Dx12DrawResources.m_pConstantBuffer)),
					L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
					L"m_ConstantBufferUploadHeap->Map()",
					L"PNTStaticDraw::CreateNotShadow()"
				);
			}
			//�V�F�[�_���\�[�X�r���[�̓e�N�X�`���Z�b�g���ɍ쐬
			//�p�C�v���C���X�e�[�g�̍쐬
			{
				//�V���h�E�t���p�C�v���C���X�e�[�g�̍쐬
				PipelineState::CreateDefault3D<T_Vertex, T_VShader, T_PShader>(m_Dx12DrawResources.m_RootSignature,
					m_Dx12DrawResources.m_PineLineDesc);
				m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
				m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
				m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(m_Dx12DrawResources.m_PineLineDesc);
				m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
				m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(m_Dx12DrawResources.m_PineLineDesc);
			}
			//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
			{
				m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(m_Dx12DrawResources.m_CullBackPipelineState);
				//�R���X�^���g�o�b�t�@�X�V
				UpdateConstantBuffer();
				CommandList::Close(m_Dx12DrawResources.m_CommandList);
			}
		}


		void UpdateConstantBuffer() {
			memcpy(m_Dx12DrawResources.m_pConstantBuffer, reinterpret_cast<void**>(&m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer),
				sizeof(m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer));
		}

		///�V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j�쐬
		void CreateShaderResourceView(const shared_ptr<TextureResource>& ShPtr,bool IsOwnShadowActive) {
			if (!ShPtr) {
				return;
			}
			auto Dev = App::GetApp()->GetDeviceResources();
			//�e�N�X�`���n���h�����쐬
			CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
				m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
			D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			//�t�H�[�}�b�g
			srvDesc.Format = ShPtr->GetTextureResDesc().Format;
			srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = ShPtr->GetTextureResDesc().MipLevels;
			//�V�F�[�_���\�[�X�r���[
			Dev->GetD3DDevice()->CreateShaderResourceView(
				ShPtr->GetTexture().Get(),
				&srvDesc,
				Handle);
			if (IsOwnShadowActive) {
				auto ShdowRender = Dev->GetShadowMapRenderTarget();
				CD3DX12_CPU_DESCRIPTOR_HANDLE ShadowHandle(
					m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
					1,
					m_Dx12DrawResources.m_DescriptorHandleIncrementSize
				);
				D3D12_SHADER_RESOURCE_VIEW_DESC shadowSrvDesc = {};
				shadowSrvDesc.Format = DXGI_FORMAT_R32_FLOAT;
				shadowSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
				shadowSrvDesc.Texture2D.MipLevels = 1;
				shadowSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
				Dev->GetD3DDevice()->CreateShaderResourceView(ShdowRender->GetDepthStencil().Get(), &shadowSrvDesc, ShadowHandle);
			}
		}

		void RefreshTrace(bool IsAlphaActive) {
			//�u�����h�X�e�[�g�ƃ��X�^���C�U�����ւ�
			if (IsAlphaActive) {
				D3D12_BLEND_DESC blend_desc;
				D3D12_RENDER_TARGET_BLEND_DESC Target;
				ZeroMemory(&blend_desc, sizeof(blend_desc));
				blend_desc.AlphaToCoverageEnable = false;
				blend_desc.IndependentBlendEnable = false;
				ZeroMemory(&Target, sizeof(Target));
				Target.BlendEnable = true;
				Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
				Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
				Target.BlendOp = D3D12_BLEND_OP_ADD;
				Target.SrcBlendAlpha = D3D12_BLEND_ONE;
				Target.DestBlendAlpha = D3D12_BLEND_ZERO;
				Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
				Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
					blend_desc.RenderTarget[i] = Target;
				}
				m_Dx12DrawResources.m_PineLineDesc.BlendState = blend_desc;
			}
			else {
				m_Dx12DrawResources.m_PineLineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			}
			m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(m_Dx12DrawResources.m_PineLineDesc);

			m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(m_Dx12DrawResources.m_PineLineDesc);

		}

		///�`�揈��
		void DrawObject(const shared_ptr<MeshResource>& ShMesh, const shared_ptr<TextureResource>& ShTex, bool IsAlphaActive) {
			if (!ShMesh) {
				return;
			}
			//��������
			RefreshTrace(IsAlphaActive);
			//�R�}���h���X�g�̃��Z�b�g
			if (IsAlphaActive) {
				CommandList::Reset(m_Dx12DrawResources.m_CullFrontPipelineState, m_Dx12DrawResources.m_CommandList);
			}
			else {
				CommandList::Reset(m_Dx12DrawResources.m_CullBackPipelineState, m_Dx12DrawResources.m_CommandList);
			}
			ShMesh->UpdateResources<VertexPositionNormalTexture>(m_Dx12DrawResources.m_CommandList);
			if (ShTex) {
				ShTex->UpdateResources(m_Dx12DrawResources.m_CommandList);
			}
			//�`��
			m_Dx12DrawResources.m_CommandList->SetGraphicsRootSignature(m_Dx12DrawResources.m_RootSignature.Get());
			ID3D12DescriptorHeap* ppHeaps[] = { m_Dx12DrawResources.m_DescriptorHeap.Get(), m_Dx12DrawResources.m_SamplerDescriptorHeap.Get() };
			m_Dx12DrawResources.m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

			for (size_t i = 0; i < m_Dx12DrawResources.m_GPUDescriptorHandleVec.size(); i++) {
				m_Dx12DrawResources.m_CommandList->SetGraphicsRootDescriptorTable((UINT)i, m_Dx12DrawResources.m_GPUDescriptorHandleVec[i]);
			}
			auto Dev = App::GetApp()->GetDeviceResources();
			m_Dx12DrawResources.m_CommandList->RSSetViewports(1, &Dev->GetViewport());
			m_Dx12DrawResources.m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
				Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
				Dev->GetFrameIndex(),
				Dev->GetRtvDescriptorSize());
			CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
				Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
			);
			m_Dx12DrawResources.m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

			m_Dx12DrawResources.m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_Dx12DrawResources.m_CommandList->IASetIndexBuffer(&ShMesh->GetIndexBufferView());
			m_Dx12DrawResources.m_CommandList->IASetVertexBuffers(0, 1, &ShMesh->GetVertexBufferView());


			m_Dx12DrawResources.m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
			if (IsAlphaActive) {
				m_Dx12DrawResources.m_CommandList->SetPipelineState(m_Dx12DrawResources.m_CullBackPipelineState.Get());
				m_Dx12DrawResources.m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
			}

			//�R�}���h���X�g�̃N���[�Y
			CommandList::Close(m_Dx12DrawResources.m_CommandList);
			//�f�o�C�X�ɃR�}���h���X�g�𑗂�
			Dev->InsertDrawCommandLists(m_Dx12DrawResources.m_CommandList.Get());
		}
	};

	//--------------------------------------------------------------------------------------
	///	Bc�`��I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	BcBaseDraw::BcBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g�̂RD
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	BcBaseDraw::~BcBaseDraw() {}
	bool BcBaseDraw::IsOriginalMeshUse() const {
		return pImpl->m_BcDrawObject.m_UseOriginalMeshResource;
	}

	void BcBaseDraw::SetOriginalMeshUse(bool b) {
		pImpl->m_BcDrawObject.m_UseOriginalMeshResource = b;
	}

	shared_ptr<MeshResource> BcBaseDraw::GetOriginalMeshResource() const {
		return pImpl->m_BcDrawObject.m_OriginalMeshResource;
	}
	void BcBaseDraw::SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_BcDrawObject.m_OriginalMeshResource = MeshRes;
	}


	shared_ptr<MeshResource> BcBaseDraw::GetMeshResource() const {
		if (IsOriginalMeshUse()) {
			return pImpl->m_BcDrawObject.m_OriginalMeshResource;
		}
		else {
			auto shptr = pImpl->m_BcDrawObject.m_MeshResource.lock();
			if (shptr) {
				return shptr;
			}
		}
		return nullptr;
	}
	void BcBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_BcDrawObject.m_MeshResource = MeshRes;
	}

	void BcBaseDraw::SetMeshResource(const wstring& MeshKey) {
		this->SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}

	shared_ptr<TextureResource> BcBaseDraw::GetTextureResource() const {
		//�e�N�X�`�����Ȃ����null��Ԃ�
		auto shptr = pImpl->m_BcDrawObject.m_TextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}


	void BcBaseDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_BcDrawObject.m_TextureResource = TextureRes;
	}

	void BcBaseDraw::SetTextureResource(const wstring& TextureKey) {
		this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<MultiMeshResource> BcBaseDraw::GetMultiMeshResource() const {
		//�Ȃ����null��Ԃ�
		auto shptr = pImpl->m_BcDrawObject.m_MultiMeshResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}

	void BcBaseDraw::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr) {
		pImpl->m_BcDrawObject.m_MultiMeshResource = MeshResourcePtr;
	}
	void BcBaseDraw::SetMultiMeshResource(const wstring& ResKey) {
		this->SetMultiMeshResource(App::GetApp()->GetResource<MultiMeshResource>(ResKey));
	}


	bsm::Col4 BcBaseDraw::GetEmissive() const {
		return pImpl->m_BcDrawObject.m_Emissive;
	}
	void BcBaseDraw::SetEmissive(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_Emissive = col;
	}
	bsm::Col4 BcBaseDraw::GetDiffuse() const {
		return pImpl->m_BcDrawObject.m_Diffuse;
	}
	void BcBaseDraw::SetDiffuse(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_Diffuse = col;
	}

	bsm::Col4 BcBaseDraw::GetSpecularColor() const {
		bsm::Col4 ret = pImpl->m_BcDrawObject.m_SpecularColorAndPower;
		ret.w = 0.0f;
		return ret;
	}
	void BcBaseDraw::SetSpecularColor(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_SpecularColorAndPower = XMVectorSelect(pImpl->m_BcDrawObject.m_SpecularColorAndPower, col, g_XMSelect1110);
	}

	void BcBaseDraw::DisableSpecular() {
		pImpl->m_BcDrawObject.m_SpecularColorAndPower = g_XMIdentityR3;
	}

	float BcBaseDraw::GetSpecularPower() const {
		return pImpl->m_BcDrawObject.m_SpecularColorAndPower.w;
	}


	void BcBaseDraw::SetSpecularPower(float pow) {
		pImpl->m_BcDrawObject.m_SpecularColorAndPower.w = pow;
	}



	bsm::Col4 BcBaseDraw::GetSpecularColorAndPower() const {
		return pImpl->m_BcDrawObject.m_SpecularColorAndPower;
	}
	void BcBaseDraw::SetSpecularColorAndPower(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_SpecularColorAndPower = col;
	}

	float BcBaseDraw::GetAlpha() const {
		return pImpl->m_BcDrawObject.m_Alpha;
	}

	void BcBaseDraw::SetAlpha(float a) {
		pImpl->m_BcDrawObject.m_Alpha = a;
	}

	bsm::Col4 BcBaseDraw::GetColorAndAlpha() const {
		bsm::Col4 ret = pImpl->m_BcDrawObject.m_Diffuse;
		ret.w = pImpl->m_BcDrawObject.m_Alpha;
		return ret;
	}

	void BcBaseDraw::SetColorAndAlpha(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_Diffuse = col;
		pImpl->m_BcDrawObject.m_Alpha = col.w;
	}
	bool BcBaseDraw::IsLightingEnabled() const {
		return pImpl->m_BcDrawObject.m_LightingEnabled;
	}
	bool BcBaseDraw::GetLightingEnabled() const {
		return pImpl->m_BcDrawObject.m_LightingEnabled;

	}
	void BcBaseDraw::SetLightingEnabled(bool value) {
		pImpl->m_BcDrawObject.m_LightingEnabled = value;
	}
	bool BcBaseDraw::GetPerPixelLighting() const {
		return pImpl->m_BcDrawObject.m_PreferPerPixelLighting;
	}

	bool BcBaseDraw::IsPerPixelLighting() const {
		return pImpl->m_BcDrawObject.m_PreferPerPixelLighting;

	}
	void BcBaseDraw::SetPerPixelLighting(bool value) {
		pImpl->m_BcDrawObject.m_PreferPerPixelLighting = value;
	}

	void BcBaseDraw::SetLightingParamaters() {
		//���C�g�𓾂�
		auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
		auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
		if (PtrMultiLight) {
			//�}���`���C�g������
			for (int i = 0; i < GetMaxDirectionalLights(); i++) {
				if (IsLightEnabled(i)) {
					SetLightDirection(i, PtrMultiLight->GetLight(i).m_Directional);
					SetLightDiffuseColor(i, PtrMultiLight->GetLight(i).m_DiffuseColor);
					SetLightSpecularColor(i, PtrMultiLight->GetLight(i).m_SpecularColor);
				}
			}
		}
		else {
			//�����ł͂Ȃ�
			auto LightPtr = GetGameObject()->OnGetDrawLight();
			SetLightEnabled(0, true);
			SetLightDirection(0, LightPtr.m_Directional);
			SetLightDiffuseColor(0, LightPtr.m_DiffuseColor);
			SetLightSpecularColor(0, LightPtr.m_SpecularColor);
			for (int i = 1; i < GetMaxDirectionalLights(); i++) {
				SetLightEnabled(i, false);
			}
		}
		SetAmbientLightColor(PtrLightObj->GetAmbientLightColor());
	}


	bool BcBaseDraw::GetBiasedNormals() const {
		return pImpl->m_BcDrawObject.m_BiasedNormals;
	}

	bool BcBaseDraw::IsBiasedNormals() const {
		return pImpl->m_BcDrawObject.m_BiasedNormals;
	}

	void BcBaseDraw::SetBiasedNormals(bool value) {
		pImpl->m_BcDrawObject.m_BiasedNormals = value;
	}

	bsm::Col4 BcBaseDraw::GetAmbientLightColor()const {
		return pImpl->m_BcDrawObject.m_AmbientLightColor;
	}

	void BcBaseDraw::SetAmbientLightColor(const bsm::Col4& value) {
		pImpl->m_BcDrawObject.m_AmbientLightColor = value;
	}
	void BcBaseDraw::ValidateLightIndex(int whichLight)
	{
		if (whichLight < 0 || whichLight >= BcDrawObject::MaxDirectionalLights)
		{
			throw BaseException(
				L"���C�g�̃C���f�b�N�X���͈͊O�ł�",
				L"if (whichLight < 0 || whichLight >= Impl::MaxDirectionalLights)",
				L"Bc3DDraw::ValidateLightIndex()"
			);
		}
	}

	int BcBaseDraw::GetMaxDirectionalLights() {
		return BcDrawObject::MaxDirectionalLights;
	}

	bool BcBaseDraw::GetLightEnabled(int whichLight)const {
		ValidateLightIndex(whichLight);
		return pImpl->m_BcDrawObject.m_LightEnabled[whichLight];
	}

	bool BcBaseDraw::IsLightEnabled(int whichLight)const {
		ValidateLightIndex(whichLight);
		return pImpl->m_BcDrawObject.m_LightEnabled[whichLight];
	}
	void BcBaseDraw::SetLightEnabled(int whichLight, bool value) {
		ValidateLightIndex(whichLight);
		pImpl->m_BcDrawObject.m_LightEnabled[whichLight] = value;
	}

	bsm::Vec3 BcBaseDraw::GetLightDirection(int whichLight) const {
		ValidateLightIndex(whichLight);
		return pImpl->m_BcDrawObject.m_LightDirection[whichLight];
	}

	void BcBaseDraw::SetLightDirection(int whichLight, const bsm::Vec3& value) {
		ValidateLightIndex(whichLight);
		pImpl->m_BcDrawObject.m_LightDirection[whichLight] = value;
	}

	bsm::Col4 BcBaseDraw::GetLightDiffuseColor(int whichLight) const {
		ValidateLightIndex(whichLight);
		return pImpl->m_BcDrawObject.m_LightDiffuseColor[whichLight];
	}


	void BcBaseDraw::SetLightDiffuseColor(int whichLight, const bsm::Col4& value) {
		ValidateLightIndex(whichLight);
		pImpl->m_BcDrawObject.m_LightDiffuseColor[whichLight] = value;
	}

	bsm::Col4 BcBaseDraw::GetLightSpecularColor(int whichLight) const {
		ValidateLightIndex(whichLight);
		return pImpl->m_BcDrawObject.m_LightSpecularColor[whichLight];
	}

	void BcBaseDraw::SetLightSpecularColor(int whichLight, const bsm::Col4& value) {
		ValidateLightIndex(whichLight);
		pImpl->m_BcDrawObject.m_LightSpecularColor[whichLight] = value;
	}


	bool BcBaseDraw::GetFogEnabled() const {
		return pImpl->m_BcDrawObject.m_FogEnabled;

	}
	bool BcBaseDraw::IsFogEnabled() const {
		return pImpl->m_BcDrawObject.m_FogEnabled;
	}
	void BcBaseDraw::SetFogEnabled(bool b) {
		pImpl->m_BcDrawObject.m_FogEnabled = b;
	}

	float BcBaseDraw::GetFogStart() const {
		return pImpl->m_BcDrawObject.m_FogStart;
	}

	void BcBaseDraw::SetFogStart(float value) {
		//��Ƀ}�C�i�X�l
		pImpl->m_BcDrawObject.m_FogStart = -(abs(value));
	}

	float BcBaseDraw::GetFogEnd() const {
		return pImpl->m_BcDrawObject.m_FogEnd;
	}
	void BcBaseDraw::SetFogEnd(float value) {
		//��Ƀ}�C�i�X�l
		pImpl->m_BcDrawObject.m_FogEnd = -(abs(value));
	}



	bsm::Col4 BcBaseDraw::GetFogColor() const {
		return pImpl->m_BcDrawObject.m_FogColor;
	}
	void BcBaseDraw::SetFogColor(const bsm::Col4& col) {
		pImpl->m_BcDrawObject.m_FogColor = col;
	}
	bool BcBaseDraw::GetOwnShadowActive() const {
		return pImpl->m_BcDrawObject.m_OwnShadowActive;
	}
	bool BcBaseDraw::IsOwnShadowActive() const {
		return pImpl->m_BcDrawObject.m_OwnShadowActive;
	}
	void BcBaseDraw::SetOwnShadowActive(bool b) {
		pImpl->m_BcDrawObject.m_OwnShadowActive = b;
	}

	bool BcBaseDraw::GetModelDiffusePriority() const {
		return pImpl->m_BcDrawObject.m_ModelDiffusePriority;

	}
	bool BcBaseDraw::IsModelDiffusePriority() const {
		return pImpl->m_BcDrawObject.m_ModelDiffusePriority;

	}
	void BcBaseDraw::SetModelDiffusePriority(bool b) {
		pImpl->m_BcDrawObject.m_ModelDiffusePriority = b;
	}
	bool BcBaseDraw::GetModelEmissivePriority() const {
		return pImpl->m_BcDrawObject.m_ModelEmissivePriority;
	}
	bool BcBaseDraw::IsModelEmissivePriority() const {
		return pImpl->m_BcDrawObject.m_ModelEmissivePriority;
	}
	void BcBaseDraw::SetModelEmissivePriority(bool b) {
		pImpl->m_BcDrawObject.m_ModelEmissivePriority = b;
	}


	bool BcBaseDraw::GetModelTextureEnabled() const {
		return pImpl->m_BcDrawObject.m_ModelTextureEnabled;

	}
	bool BcBaseDraw::IsModelTextureEnabled() const {
		return pImpl->m_BcDrawObject.m_ModelTextureEnabled;
	}
	void BcBaseDraw::SetModelTextureEnabled(bool b) {
		pImpl->m_BcDrawObject.m_ModelTextureEnabled = b;
	}

	shared_ptr<TextureResource> BcBaseDraw::GetNormalMapTextureResource() const {
		auto shptr = pImpl->m_BcDrawObject.m_NormalMapTextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}
	void BcBaseDraw::SetNormalMapTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_BcDrawObject.m_NormalMapTextureResource = TextureRes;

	}
	void BcBaseDraw::SetNormalMapTextureResource(const wstring& TextureKey) {
		pImpl->m_BcDrawObject.m_NormalMapTextureResource = App::GetApp()->GetResource<TextureResource>(TextureKey);
	}


	const vector<weak_ptr<TextureResource>>& BcBaseDraw::GetNormalMapTextureResourceVec() const {
		return pImpl->m_BcDrawObject.m_NormalMapTextureResourceVec;

	}
	vector<weak_ptr<TextureResource>>& BcBaseDraw::GetNormalMapTextureResourceVec() {
		return pImpl->m_BcDrawObject.m_NormalMapTextureResourceVec;
	}

	void BcBaseDraw::BoneInit() {
		pImpl->m_BcDrawObject.BoneInit();
	}

	void BcBaseDraw::InterpolationMatrix(const bsm::Mat4x4& m1, const bsm::Mat4x4& m2, float t, bsm::Mat4x4& out) {
		pImpl->m_BcDrawObject.InterpolationMatrix(m1, m2, t, out);
	}

	void BcBaseDraw::AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
		float SamplesParSecond) {
		pImpl->m_BcDrawObject.AddAnimation(Name, StartSample, SampleLength, Loop, SamplesParSecond);
	}
	void  BcBaseDraw::ChangeCurrentAnimation(const wstring& AnemationName, float StartTime) {
		pImpl->m_BcDrawObject.ChangeCurrentAnimation(AnemationName, StartTime);
	}
	const wstring& BcBaseDraw::GetCurrentAnimation() const {
		return pImpl->m_BcDrawObject.m_CurrentAnimeName;

	}

	float BcBaseDraw::GetCurrentAnimationTime() const {
		return pImpl->m_BcDrawObject.m_CurrentAnimeTime;
	}

	bool BcBaseDraw::IsTargetAnimeEnd() const {
		auto& AnimData = pImpl->m_BcDrawObject.GetAnimationData();
		return AnimData.m_IsAnimeEnd;
	}


	bool BcBaseDraw::UpdateAnimation(float ElapsedTime) {
		return pImpl->m_BcDrawObject.UpdateAnimation(ElapsedTime);
	}
	const vector< bsm::Mat4x4 >* BcBaseDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_BcDrawObject.m_LocalBonesMatrix;
	}

	vector< bsm::Mat4x4 >& BcBaseDraw::GetVecLocalBones() {
		return pImpl->m_BcDrawObject.m_LocalBonesMatrix;
	}


	const vector< bsm::Mat4x4 >* BcBaseDraw::GetVecMultiLocalBonesPtr(size_t index) const {
		if (pImpl->m_BcDrawObject.m_MultiLocalBonesMatrix.size() > index) {
			return &(pImpl->m_BcDrawObject.m_MultiLocalBonesMatrix[index]);
		}
		return nullptr;
	}

	vector< bsm::Mat4x4 >& BcBaseDraw::GetVecMultiLocalBones(size_t index) {
		if (pImpl->m_BcDrawObject.m_MultiLocalBonesMatrix.size() <= index) {
			throw BaseException(
				L"�C���f�b�N�X������𒴂��Ă܂�",
				L"if (pImpl->m_BcDrawObject.m_MultiLocalBonesMatrix.size() <= index)",
				L"BcBaseDraw::GetVecMultiLocalBones"
			);
		}
		return pImpl->m_BcDrawObject.m_MultiLocalBonesMatrix[index];
	}

	void BcBaseDraw::GetStaticMeshLocalPositions(vector<bsm::Vec3>& vertices) {
/*
		auto ReshRes = GetMeshResource();
		if (!ReshRes) {
			throw BaseException(
				L"���b�V�����\�[�X������܂���",
				L"if (!ReshRes)",
				L"BcBaseDraw::GetStaticMeshLocalPositions()"
			);
		}
		vertices.clear();
		ReshRes->GetLocalPositions(vertices);
*/
	}

	void BcBaseDraw::GetStaticMeshWorldPositions(vector<bsm::Vec3>& vertices) {
		GetStaticMeshLocalPositions(vertices);
		//���[���h�s��̔��f
		auto WorldMat = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		for (auto& v : vertices) {
			v *= WorldMat;
		}
	}

	bool BcBaseDraw::HitTestStaticMeshSegmentTriangles(const bsm::Vec3& StartPos, const bsm::Vec3& EndPos, bsm::Vec3& HitPoint,
		TRIANGLE& RetTri, size_t& RetIndex) {
		GetStaticMeshWorldPositions(pImpl->m_BcDrawObject.m_TempPositions);
		for (size_t i = 0; i < pImpl->m_BcDrawObject.m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_BcDrawObject.m_TempPositions[i];
			tri.m_B = pImpl->m_BcDrawObject.m_TempPositions[i + 1];
			tri.m_C = pImpl->m_BcDrawObject.m_TempPositions[i + 2];
			bsm::Vec3 ret;
			float t;
			if (HitTest::SEGMENT_TRIANGLE(StartPos, EndPos, tri, ret, t)) {
				auto Len = length(EndPos - StartPos);
				Len *= t;
				auto Nomal = EndPos - StartPos;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartPos + Nomal;
				RetTri = tri;
				RetIndex = i / 3;
				return true;
			}
		}
		return false;
	}


	void BcBaseDraw::GetSkinedMeshLocalPositions(vector<bsm::Vec3>& vertices) {
/*
		if (GetVecLocalBones().size() == 0) {
			throw BaseException(
				L"�{�[���s�񂪂���܂���",
				L"if (GetVecLocalBones().size() == 0)",
				L"BcBaseDraw::GetSkinedMeshLocalPositions()"
			);
		}
		auto ReshRes = GetMeshResource();
		if (!ReshRes) {
			throw BaseException(
				L"���b�V�����\�[�X������܂���",
				L"if (!ReshRes)",
				L"BcBaseDraw::GetSkinedMeshLocalPositions()"
			);
		}
		vertices.clear();
		auto& Bones = GetVecLocalBones();
		auto& PosVec = ReshRes->GetVerteces();
		auto& SkinVec = ReshRes->GetSkins();
		for (auto& v : PosVec) {
			vertices.push_back(v.position);
		}
		//�X�L�j���O����
		for (size_t j = 0; j < vertices.size(); j++) {
			bsm::Mat4x4 skinning(0);
			for (size_t i = 0; i < 4; i++)
			{
				skinning += Bones[SkinVec[j].indices[i]] * SkinVec[j].weights[i];
			}
			skinning._14 = 1.0f;
			skinning._24 = 1.0f;
			skinning._34 = 1.0f;
			skinning._44 = 1.0f;
			bsm::Vec4 p(vertices[j]);
			p.w = 1.0f;
			p *= skinning;
			vertices[j] = p;
		}
*/
	}

	void BcBaseDraw::GetSkinedMeshWorldPositions(vector<bsm::Vec3>& vertices) {
		GetSkinedMeshLocalPositions(vertices);
		//���[���h�s��̔��f
		auto WorldMat = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		for (auto& v : vertices) {
			v *= WorldMat;
		}
	}

	bool BcBaseDraw::HitTestSkinedMeshSegmentTriangles(const bsm::Vec3& StartPos, const bsm::Vec3& EndPos,
		bsm::Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex) {
		GetSkinedMeshWorldPositions(pImpl->m_BcDrawObject.m_TempPositions);
		for (size_t i = 0; i < pImpl->m_BcDrawObject.m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_BcDrawObject.m_TempPositions[i];
			tri.m_B = pImpl->m_BcDrawObject.m_TempPositions[i + 1];
			tri.m_C = pImpl->m_BcDrawObject.m_TempPositions[i + 2];
			bsm::Vec3 ret;
			float t;
			if (HitTest::SEGMENT_TRIANGLE(StartPos, EndPos, tri, ret, t)) {
				auto Len = length(EndPos - StartPos);
				Len *= t;
				auto Nomal = EndPos - StartPos;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartPos + Nomal;
				RetTri = tri;
				RetIndex = i / 3;
				return true;
			}
		}
		return false;
	}

	void BcBaseDraw::SetConstants(BasicConstants& BcCb) {
		//�s��̒�`
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//�J�����𓾂�
		auto CameraPtr = GetGameObject()->OnGetDrawCamera();
		//���C�g�𓾂�
		auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
		auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
		if (PtrMultiLight) {
			//�}���`���C�g������
			BcCb.activeFlg.x = 3;
		}
		else {
			//�����ł͂Ȃ�
			BcCb.activeFlg.x = 1;
		}
		auto StageLight = GetGameObject()->OnGetDrawLight();
		//���[���h�s��
		bsm::Mat4x4 world;

//		if (data.m_UseMeshToTransformMatrix) {
//			world = data.m_MeshToTransformMatrix * GetMeshToTransformMatrix();
//			world *= PtrTrans->GetWorldMatrix();
//		}
//		else {
			world = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
//		}
		//�r���[�s��
		bsm::Mat4x4 view = CameraPtr->GetViewMatrix();
		//�ˉe�s��
		bsm::Mat4x4 projection = CameraPtr->GetProjMatrix();

		//�s��̐ݒ�
		auto worldView = world * view;
		BcCb.worldViewProj = XMMatrixTranspose(XMMatrixMultiply(worldView, projection));

		//�t�H�O�̐ݒ�
		if (IsFogEnabled())
		{
			auto start = GetFogStart();
			auto end = GetFogEnd();
			if (start == end)
			{
				// Degenerate case: force everything to 100% fogged if start and end are the same.
				static const XMVECTORF32 fullyFogged = { 0, 0, 0, 1 };
				BcCb.fogVector = fullyFogged;
			}
			else
			{
				XMMATRIX worldViewTrans = worldView;
				// _13, _23, _33, _43
				XMVECTOR worldViewZ = XMVectorMergeXY(XMVectorMergeZW(worldViewTrans.r[0], worldViewTrans.r[2]),
					XMVectorMergeZW(worldViewTrans.r[1], worldViewTrans.r[3]));
				XMVECTOR wOffset = XMVectorSwizzle<1, 2, 3, 0>(XMLoadFloat(&start));
				BcCb.fogVector = (worldViewZ + wOffset) / (start - end);
			}
			BcCb.fogColor = GetFogColor();
		}
		else
		{
			BcCb.fogVector = g_XMZero;
			BcCb.fogColor = g_XMZero;

		}
		//���C�g�̐ݒ�
		if (IsLightingEnabled())
		{
			if (BcCb.activeFlg.x == 1) {
				BcCb.lightDirection[0] = GetLightDirection(0);
				BcCb.lightDiffuseColor[0] = GetLightDiffuseColor(0);
				BcCb.lightSpecularColor[0] = GetLightSpecularColor(0);
			}
			else {
				for (int i = 0; i < GetMaxDirectionalLights(); i++) {
					if (IsLightEnabled(i)) {
						BcCb.lightDirection[i] = GetLightDirection(i);
						BcCb.lightDiffuseColor[i] = GetLightDiffuseColor(i);
						BcCb.lightSpecularColor[i] = GetLightSpecularColor(i);
					}
				}
			}
			BcCb.world = XMMatrixTranspose(world);

			XMMATRIX worldInverse = XMMatrixInverse(nullptr, world);

			BcCb.worldInverseTranspose[0] = worldInverse.r[0];
			BcCb.worldInverseTranspose[1] = worldInverse.r[1];
			BcCb.worldInverseTranspose[2] = worldInverse.r[2];


			XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);

			BcCb.eyePosition = viewInverse.r[3];
		}
		XMVECTOR diffuse = GetDiffuse();
		XMVECTOR alphaVector = XMVectorReplicate(GetAlpha());
		XMVECTOR emissiveColor = GetEmissive();
		XMVECTOR ambientLightColor = GetAmbientLightColor();
		if (IsLightingEnabled())
		{
			// Merge emissive and ambient light contributions.
			BcCb.emissiveColor = (emissiveColor + ambientLightColor * diffuse) * alphaVector;
			BcCb.specularColorAndPower = GetSpecularColorAndPower();
		}
		else
		{
			BcCb.activeFlg.x = 0;
			// Merge diffuse and emissive light contributions.
			diffuse += emissiveColor;
			BcCb.specularColorAndPower = GetSpecularColorAndPower();
		}
		// xyz = diffuse * alpha, w = alpha.
		BcCb.diffuseColor = XMVectorSelect(alphaVector, diffuse * alphaVector, g_XMSelect1110);

		//�e�p
		if (GetOwnShadowActive()) {
			bsm::Vec3 CalcLightDir(StageLight.m_Directional * -1.0);
			bsm::Vec3 LightAt(CameraPtr->GetAt());
			bsm::Vec3 LightEye(CalcLightDir);
			LightEye *= Shadowmap::GetLightHeight();
			LightEye = LightAt + LightEye;
			bsm::Vec4 LightEye4(LightEye, 1.0f);
			LightEye4.w = 1.0f;
			BcCb.lightPos = LightEye4;
			bsm::Vec4 eyePos4(CameraPtr->GetEye(), 1.0f);
			eyePos4.w = 1.0f;
			BcCb.eyePos = eyePos4;
			bsm::Mat4x4 LightView, LightProj;
			//���C�g�̃r���[�Ǝˉe���v�Z
			LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
				Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
			BcCb.lightView = bsm::transpose(LightView);
			BcCb.lightProjection = bsm::transpose(LightProj);
		}
		//�{�[���̐ݒ�
		size_t BoneSz = pImpl->m_BcDrawObject.m_LocalBonesMatrix.size();
		if (BoneSz > 0) {
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				bsm::Mat4x4 mat = pImpl->m_BcDrawObject.m_LocalBonesMatrix[b];
				mat.transpose();
				BcCb.bones[cb_count] = ((XMMATRIX)mat).r[0];
				BcCb.bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				BcCb.bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}
		}
/*
		else if (pImpl->m_BcDrawObject.m_MultiLocalBonesMatrix.size() > data.m_MultiMeshIndex) {
			//�}���`���b�V���̃{�[����������
			//�{�[���̐ݒ�
			BoneSz = pImpl->m_BcDrawObject.m_MultiLocalBonesMatrix[data.m_MultiMeshIndex].size();
			if (BoneSz > 0) {
				UINT cb_count = 0;
				for (size_t b = 0; b < BoneSz; b++) {
					bsm::Mat4x4 mat = pImpl->m_BcDrawObject.m_MultiLocalBonesMatrix[data.m_MultiMeshIndex][b];
					mat.transpose();
					BcCb.bones[cb_count] = ((XMMATRIX)mat).r[0];
					BcCb.bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
					BcCb.bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
					cb_count += 3;
				}
			}
		}
*/

	}

	void BcBaseDraw::CreateBcPNTNotShadow() {
		pImpl->CreateStaticNotShadow<VertexPositionNormalTexture,BcVSPNTStaticVL, BcPSPNTVL>();
	}

	void BcBaseDraw::CreateBcPNTWithShadow() {
		pImpl->CreateStaticWithShadow<VertexPositionNormalTexture, BcVSPNTStaticVLShadow, BcPSPNTVLShadow>();
	}


	void BcBaseDraw::DrawBcPNT() {
		auto PtrStage = GetGameObject()->GetStage();
		auto PtrMeshResource = GetMeshResource();
		if (GetTextureResource()) {
			pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer.activeFlg.y = 1;
			pImpl->CreateShaderResourceView(GetTextureResource(), IsOwnShadowActive());
		}
		else {
			pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer.activeFlg.y = 0;
		}
		//���C�g��ݒ�
		SetLightingParamaters();
		SetConstants(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer);
		//�X�V
		pImpl->UpdateConstantBuffer();
		pImpl->DrawObject(PtrMeshResource, GetTextureResource(), GetGameObject()->IsAlphaActive());
	}

	const Dx12DrawResources<BasicConstants>& BcBaseDraw::GetBasicDrawResources() const {
		return pImpl->m_Dx12DrawResources;
	}
	Dx12DrawResources<BasicConstants>& BcBaseDraw::GetBasicDrawResources() {
		return pImpl->m_Dx12DrawResources;

	}


	//--------------------------------------------------------------------------------------
	///	BcPNTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	BcPNTStaticDraw::BcPNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		BcBaseDraw(GameObjectPtr)
	{
	}

	BcPNTStaticDraw::~BcPNTStaticDraw() {}

	void BcPNTStaticDraw::SetOwnShadowActive(bool b) {
		if (IsOwnShadowActive()) {
			//���͉e�t��
			if (!b) {
				//PNT�e�����ɕύX
				CreateBcPNTNotShadow();
				BcBaseDraw::SetOwnShadowActive(false);
			}
		}
		else {
			//���͉e����
			if (b) {
				//PNT�e�t���ɕύX
				CreateBcPNTWithShadow();
				BcBaseDraw::SetOwnShadowActive(true);
			}
		}
	}


	void BcPNTStaticDraw::OnCreate() {
		SetLightingEnabled(true);
		//�}���`���C�g�̐ݒ�
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
		//PNT�e�����ɏ�����
		CreateBcPNTNotShadow();
	}

	void BcPNTStaticDraw::OnDraw() {
		DrawBcPNT();
	}


}
//end basecross
