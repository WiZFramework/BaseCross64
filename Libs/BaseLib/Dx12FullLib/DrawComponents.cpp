/*!
@file DrawComponents.cpp
@brief �`��R���|�[�l���g����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"
#include <pix.h>


namespace basecross {

	IMPLEMENT_DX12SHADER(VSShadowmap, App::GetApp()->GetShadersPath() + L"VSShadowmap.cso")

	IMPLEMENT_DX12SHADER(VSPTSprite, App::GetApp()->GetShadersPath() + L"VSPTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPTSprite, App::GetApp()->GetShadersPath() + L"PSPTSprite.cso")


	IMPLEMENT_DX12SHADER(VSPNStatic, App::GetApp()->GetShadersPath() + L"VSPNStatic.cso")
	IMPLEMENT_DX12SHADER(PSPNStatic, App::GetApp()->GetShadersPath() + L"PSPNStatic.cso")

		
	IMPLEMENT_DX12SHADER(VSPNTStatic, App::GetApp()->GetShadersPath() + L"VSPNTStatic.cso")
	IMPLEMENT_DX12SHADER(PSPNTStatic, App::GetApp()->GetShadersPath() + L"PSPNTStatic.cso")

	IMPLEMENT_DX12SHADER(VSPNTStaticShadow, App::GetApp()->GetShadersPath() + L"VSPNTStaticShadow.cso")
	IMPLEMENT_DX12SHADER(PSPNTStaticShadow, App::GetApp()->GetShadersPath() + L"PSPNTStaticShadow.cso")
	IMPLEMENT_DX12SHADER(PSPNTStaticShadow2, App::GetApp()->GetShadersPath() + L"PSPNTStaticShadow2.cso")

	IMPLEMENT_DX12SHADER(VSPCSprite, App::GetApp()->GetShadersPath() + L"VSPCSprite.cso")
	IMPLEMENT_DX12SHADER(PSPCSprite, App::GetApp()->GetShadersPath() + L"PSPCSprite.cso")

	IMPLEMENT_DX12SHADER(VSPCTSprite, App::GetApp()->GetShadersPath() + L"VSPCTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPCTSprite, App::GetApp()->GetShadersPath() + L"PSPCTSprite.cso")

	//--------------------------------------------------------------------------------------
	///	�ėp�`��p
	//--------------------------------------------------------------------------------------
	GenericDraw::GenericDraw() :
		ObjectInterface() {}
	GenericDraw::~GenericDraw() {}
	void GenericDraw::OnCreate() {}

	void GenericDraw::PCWireFrameDraw(const shared_ptr<GameObject>& GameObjectPtr, const shared_ptr<MeshResource>& MeshRes,
		const bsm::Col4& Emissive, const bsm::Col4& Diffuse, const bsm::Mat4x4& MeshToTransformMatrix) {
		auto Dev = App::GetApp()->GetDeviceResources();
	}


	void GenericDraw::DrawWireFrame(const shared_ptr<GameObject>& GameObj,
		const shared_ptr<MeshResource>& Mesh, const bsm::Mat4x4& MeshToTransformMatrix) {

		PCWireFrameDraw(GameObj, Mesh,
			bsm::Col4(0, 0, 0, 0), bsm::Col4(1, 1, 1, 1), MeshToTransformMatrix);
	}


	//--------------------------------------------------------------------------------------
	//	struct DrawComponent::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DrawComponent::Impl {
		BlendState m_BlendState;
		DepthStencilState m_DepthStencilState;
		RasterizerState m_RasterizerState;
		SamplerState m_SamplerState;
		bsm::Mat4x4 m_MeshToTransformMatrix;
		Impl() :
			m_MeshToTransformMatrix()
		{}
	};


	//--------------------------------------------------------------------------------------
	///	�`��R���|�[�l���g�̐e�N���X����
	//--------------------------------------------------------------------------------------
	DrawComponent::DrawComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	DrawComponent::~DrawComponent() {}


	BlendState DrawComponent::GetBlendState() const {
		return pImpl->m_BlendState;
	}

	DepthStencilState DrawComponent::GetDepthStencilState() const {
		return pImpl->m_DepthStencilState;
	}
	RasterizerState DrawComponent::GetRasterizerState() const {
		return pImpl->m_RasterizerState;
	}
	SamplerState DrawComponent::GetSamplerState() const {
		return pImpl->m_SamplerState;
	}


	void DrawComponent::SetBlendState(const BlendState state) {
		pImpl->m_BlendState = state;
	}
	void DrawComponent::SetDeviceBlendState() {
	}


	void DrawComponent::SetDepthStencilState(const DepthStencilState state) {
		pImpl->m_DepthStencilState = state;

	}

	void DrawComponent::SetDeviceDepthStencilState() {
	}

	void DrawComponent::SetRasterizerState(const RasterizerState state) {
		pImpl->m_RasterizerState = state;
	}
	void DrawComponent::SetDeviceRasterizerState() {
	}


	void DrawComponent::SetSamplerState(const SamplerState state) {
		pImpl->m_SamplerState = state;
	}
	void DrawComponent::SetDeviceSamplerState() {
	}



	const bsm::Mat4x4& DrawComponent::GetMeshToTransformMatrix() const {
		return pImpl->m_MeshToTransformMatrix;
	}
	void DrawComponent::SetMeshToTransformMatrix(const bsm::Mat4x4& Mat) {
		pImpl->m_MeshToTransformMatrix = Mat;
	}





	//--------------------------------------------------------------------------------------
	//ParticleDraw�����̂��߂̍\����
	//--------------------------------------------------------------------------------------
	struct DrawParticleSprite {
		//�p�[�e�B�N���̃J�����܂ł̋���
		float m_ToCaneraLength;
		//���[���h�s��
		bsm::Mat4x4 m_WorldMatrix;
		//�e�N�X�`��
		shared_ptr<TextureResource> m_TextureRes;
		//Diffuse�J���[
		bsm::Col4 m_Diffuse;
		DrawParticleSprite() :
			m_ToCaneraLength(0),
			m_Diffuse(1, 1, 1, 1)

		{}
	};



	//--------------------------------------------------------------------------------------
	//	struct PCTParticleDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PCTParticleDraw::Impl {
		//�`��R���e�L�X�g
		shared_ptr<MeshResource> m_MeshResource;	//���b�V�����\�[�X
		vector<DrawParticleSprite> m_DrawParticleSpriteVec;
		const size_t m_MaxInstance;				///<�C���X�^���X�ő�l
//		ComPtr<ID3D11Buffer> m_MatrixBuffer;	///<�s��p�̒��_�o�b�t�@
												//���Z�������邩�ǂ���
		bool m_Addtype;
		Impl(size_t MaxInstance, bool AddType) :
			m_MaxInstance(MaxInstance),
			m_Addtype(AddType)
		{}
		~Impl() {}
		//���_�o�b�t�@�̍쐬
		void CreateParticleBuffers();
	};


	void PCTParticleDraw::Impl::CreateParticleBuffers() {
		try {
			float HelfSize = 0.5f;
			bsm::Vec4 col(1.0f, 1.0f, 1.0f, 1.0f);
			//���_�z��
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, HelfSize, 0),  col,bsm::Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, HelfSize, 0), col, bsm::Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, -HelfSize, 0),  col,bsm::Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, -HelfSize, 0),  col, bsm::Vec2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//���b�V���̍쐬
			m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, false);

			//�C���X�^���X�s��o�b�t�@�̍쐬
			//Max�l�ō쐬����
			vector<bsm::Mat4x4> matrices(m_MaxInstance, bsm::Mat4x4());
//			MeshResource::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);

		}
		catch (...) {
			throw;
		}
	}




	//--------------------------------------------------------------------------------------
	///	PCTParticle�`��R���|�[�l���g(�p�[�e�B�N���`��)
	//--------------------------------------------------------------------------------------
	PCTParticleDraw::PCTParticleDraw(const shared_ptr<GameObject>& GameObjectPtr, size_t MaxInstance, bool AddType) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl(MaxInstance, AddType))
	{}

	PCTParticleDraw::~PCTParticleDraw() {}

	void PCTParticleDraw::AddParticle(float ToCaneraLength, const bsm::Mat4x4& WorldMatrix,
		const shared_ptr<TextureResource>& TextureRes, const bsm::Col4& Diffuse) {
		DrawParticleSprite Item;
		Item.m_ToCaneraLength = ToCaneraLength;
		Item.m_WorldMatrix = WorldMatrix;
		Item.m_TextureRes = TextureRes;
		Item.m_Diffuse = Diffuse;
		pImpl->m_DrawParticleSpriteVec.push_back(Item);
	}

	void PCTParticleDraw::OnCreate() {
		pImpl->CreateParticleBuffers();
	}

	struct InstanceDrawStr {
		size_t Start;
		size_t Count;
		shared_ptr<TextureResource> Tex;
		bsm::Col4 m_Diffuse;
		InstanceDrawStr(size_t s, size_t c, shared_ptr<TextureResource> t,
			const bsm::Col4& Diffuse) :
			Start(s), Count(c), Tex(t), m_Diffuse(Diffuse) {}
	};

	void PCTParticleDraw::OnDraw() {

	}


	//--------------------------------------------------------------------------------------
	//	SpriteBaseDraw::Impl
	//--------------------------------------------------------------------------------------
	struct SpriteBaseDraw::Impl {
		//���b�V��
		shared_ptr<MeshResource> m_SpriteMesh;
		///�e�N�X�`�����\�[�X
		weak_ptr<TextureResource> m_TextureResource;
		//�G�~�b�V�u�F
		bsm::Col4 m_Emissive;
		//�f�t���[�Y�F
		bsm::Col4 m_Diffuse;
		//�X�v���C�g�p��Dx12���\�[�X
		Dx12DrawResources<SpriteConstants> m_Dx12DrawResources;
		Impl() :
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
		{}
	};

	SpriteBaseDraw::SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}

	SpriteBaseDraw::~SpriteBaseDraw() {}

	void SpriteBaseDraw::SetConstants(Dx12Constants<SpriteConstants>& SpCb) {
		//�s��̎擾
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//�s��̒�`
		bsm::Mat4x4 World, Proj;
		World = PtrTrans->Get2DWorldMatrix();
		//�ˉe�s��̌���
		GetGameObject()->OnGet2DDrawProjMatrix(Proj);
		//�s��̍���
		World *= Proj;
		//�G�~�b�V�u
		SpCb.m_ConstantBuffer.Emissive = GetEmissive();
		//�f�t�B�[�Y�͂��ׂĒʂ�
		SpCb.m_ConstantBuffer.Diffuse = GetDiffuse();
		//�s��̐ݒ�
		SpCb.m_ConstantBuffer.World = World;
	}

	shared_ptr<MeshResource> SpriteBaseDraw::GetMeshResource() const {
		return pImpl->m_SpriteMesh;
	}
	void SpriteBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_SpriteMesh = MeshRes;
	}

	void SpriteBaseDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;

	}

	void SpriteBaseDraw::SetTextureResource(const wstring& TextureKey) {
		this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<TextureResource> SpriteBaseDraw::GetTextureResource() const {
		//�e�N�X�`�����Ȃ����null��Ԃ�
		auto shptr = pImpl->m_TextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}



	bsm::Col4 SpriteBaseDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void SpriteBaseDraw::SetEmissive(const bsm::Col4& col) {
		pImpl->m_Emissive = col;
	}

	bsm::Col4 SpriteBaseDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void SpriteBaseDraw::SetDiffuse(const bsm::Col4& col) {
		pImpl->m_Diffuse = col;
	}

	void SpriteBaseDraw::CreatePC()
	{
		//���[�g�V�O�l�`��
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrvSmpCbv();

		//�f�X�N�v���^�q�[�v
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//CbvSrv�f�X�N�v���^�q�[�v
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
			//�T���v���[�f�X�N�v���^�q�[�v
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
			//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.clear();
			CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SrvHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
				pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SamplerHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(CbvHandle);
		}
		//�T���v���[
		{
			auto SamplerDescriptorHandle = pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::SamplerNone, SamplerDescriptorHandle);
		}
		//�R���X�^���g�o�b�t�@
		{
			//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
			UINT ConstBuffSize = (sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;

			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
				L"Device->GetDevice()->CreateCommittedResource()",
				L"SpriteBaseDraw::CreatePC()"
			);

			//�R���X�^���g�o�b�t�@�̃r���[���쐬
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;

			//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N���v�^�q�[�v��̃n���h�����擾
			//�V�F�[�_�[���\�[�X������ꍇ�A�R���X�^���g�o�b�t�@�̓V�F�[�_�[���\�[�X�r���[�̂��Ƃɔz�u����
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				0,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);
			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);

			//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"SpriteBaseDraw::CreatePC()"
			);
		}
		//�p�C�v���C���X�e�[�g
		{
			//�p�C�v���C���X�e�[�g�̒�`
			PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(pImpl->m_Dx12DrawResources.m_RootSignature, pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_CullBackPipelineState);
			//�R���X�^���g�o�b�t�@�X�V
			UpdateConstantBuffer();
			CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		}
	}

	void SpriteBaseDraw::CreatePT() {
		//���[�g�V�O�l�`��
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrvSmpCbv();
		//�f�X�N�v���^�q�[�v
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//�f�X�N�v���^�q�[�v
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
			//�T���v���[�f�X�N�v���^�q�[�v
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
			//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.clear();
			CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SrvHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
				pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SamplerHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(CbvHandle);

		}
		//�T���v���[
		{
			auto SamplerDescriptorHandle = pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle);
		}
		//�R���X�^���g�o�b�t�@
		{
			//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
			UINT ConstBuffSize = (sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;
			//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"SpriteBaseDraw::CreatePT()"
			);
			//�R���X�^���g�o�b�t�@�̃r���[���쐬
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;
			//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
			//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);

			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"SpriteBaseDraw::CreatePT()"
			);

		}
		//�V�F�[�_���\�[�X�r���[�̓e�N�X�`���Z�b�g���ɍ쐬
		//�p�C�v���C���X�e�[�g�̍쐬
		{
			PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(pImpl->m_Dx12DrawResources.m_RootSignature, pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_CullBackPipelineState);
			//�R���X�^���g�o�b�t�@�X�V
			UpdateConstantBuffer();
			CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		}
	}

	void SpriteBaseDraw::CreatePCT() {
		//���[�g�V�O�l�`��
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrvSmpCbv();

		//�f�X�N�v���^�q�[�v
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//�f�X�N�v���^�q�[�v
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
			//�T���v���[�f�X�N�v���^�q�[�v
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
			//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.clear();
			CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SrvHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
				pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SamplerHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(CbvHandle);

		}

		//�T���v���[
		{
			auto SamplerDescriptorHandle = pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle);
		}

		//�R���X�^���g�o�b�t�@
		{
			//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
			UINT ConstBuffSize = (sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;
			//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"SpriteBaseDraw::CreatePT()"
			);
			//�R���X�^���g�o�b�t�@�̃r���[���쐬
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;
			//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
			//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);

			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"SpriteBaseDraw::CreatePT()"
			);

		}

		//�p�C�v���C���X�e�[�g�̍쐬
		{
			PipelineState::CreateDefault2D<VertexPositionColorTexture, VSPCTSprite, PSPCTSprite>(pImpl->m_Dx12DrawResources.m_RootSignature, pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}

		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_CullBackPipelineState);
			//�R���X�^���g�o�b�t�@�X�V
			UpdateConstantBuffer();
			CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		}
	}


	///�V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j�쐬
	void SpriteBaseDraw::CreateShaderResourceView() {
		auto ShPtr = GetTextureResource();
		if (!ShPtr) {
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		//�e�N�X�`���n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
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
	}


	void SpriteBaseDraw::RefreshTrace() {
		//�u�����h�X�e�[�g�ƃ��X�^���C�U�����ւ�
		if (GetGameObject()->IsAlphaActive()) {
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
			pImpl->m_Dx12DrawResources.m_PineLineDesc.BlendState = blend_desc;
		}
		else {
			pImpl->m_Dx12DrawResources.m_PineLineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		}
		pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);

		pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);

	}

	void SpriteBaseDraw::UpdateConstantBuffer() {
		memcpy(pImpl->m_Dx12DrawResources.m_pConstantBuffer, reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer),
			sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer));
	}

	Dx12DrawResources<SpriteConstants>& SpriteBaseDraw::GetDx12DrawResources() {
		return pImpl->m_Dx12DrawResources;
	}

	void SpriteBaseDraw::DrawPC()
	{
		SetConstants(pImpl->m_Dx12DrawResources.m_Dx12Constants);
		//�X�V
		UpdateConstantBuffer();
		DrawObject<VertexPositionColor>();
	}

	void SpriteBaseDraw::DrawPT() {
		auto PtrStage = GetGameObject()->GetStage();
		auto PtrMeshResource = GetMeshResource();
		CreateShaderResourceView();
		SetConstants(pImpl->m_Dx12DrawResources.m_Dx12Constants);
		//�X�V
		UpdateConstantBuffer();
		DrawObject<VertexPositionTexture>();

	}

	void SpriteBaseDraw::DrawPCT() {
		CreateShaderResourceView();
		SetConstants(pImpl->m_Dx12DrawResources.m_Dx12Constants);
		//�X�V
		UpdateConstantBuffer();
		DrawObject<VertexPositionColorTexture>();
	}

	//--------------------------------------------------------------------------------------
	///	PCSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		try {
			CreateMesh(Vertices, indices);
		}
		catch (...) {
			throw;
		}
	}

	PCSpriteDraw::~PCSpriteDraw() {}

	void PCSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//�쐬����ĂȂ�������쐬����
			float HelfSize = 0.5f;
			//���_�z��
			vector<VertexPositionColor> Vertices = {
				{ VertexPositionColor(bsm::Vec3(-HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			{ VertexPositionColor(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			{ VertexPositionColor(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			{ VertexPositionColor(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			CreateMesh(Vertices, indices);
		}
		CreatePC();
	}

	void PCSpriteDraw::OnDraw() {
		DrawPC();
	}

	//--------------------------------------------------------------------------------------
	///	PTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		try {
			CreateMesh(Vertices, indices);
		}
		catch (...) {
			throw;
		}
	}

	PTSpriteDraw::~PTSpriteDraw() {}

	void PTSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//�쐬����ĂȂ�������쐬����
			float HelfSize = 0.5f;
			//���_�z��
			vector<VertexPositionTexture> Vertices = {
				{ VertexPositionTexture(bsm::Vec3(-HelfSize, HelfSize, 0), bsm::Vec2(0.0f, 0.0f)) },
			{ VertexPositionTexture(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Vec2(1.0f, 0.0f)) },
			{ VertexPositionTexture(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Vec2(0.0f, 1.0f)) },
			{ VertexPositionTexture(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Vec2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			CreateMesh(Vertices, indices);
		}
		CreatePT();
	}

	void PTSpriteDraw::OnDraw() {
		DrawPT();
	}

	//--------------------------------------------------------------------------------------
	///	PCTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		try {
			CreateMesh(Vertices, indices);
		}
		catch (...) {
			throw;
		}
	}

	PCTSpriteDraw::~PCTSpriteDraw() {}

	void PCTSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//�쐬����ĂȂ�������쐬����
			float HelfSize = 0.5f;
			//���_�z��
			vector<VertexPositionColorTexture> Vertices = {
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, HelfSize, 0),bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//���b�V���̍쐬�i�ύX�ł���j
			CreateMesh(Vertices, indices);
		}

		CreatePCT();
	}

	void PCTSpriteDraw::OnDraw() {
		DrawPCT();
	}



	//--------------------------------------------------------------------------------------
	//	struct Shadowmap::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct Shadowmap::Impl {
		static float m_LightHeight;	//���C�g�̍����i���������̒l�Ŋ|����j
		static float m_LightNear;	//���C�g��Near
		static float m_LightFar;		//���C�g��Far
		static float m_ViewWidth;
		static float m_ViewHeight;

		//���b�V�����\�[�X
		weak_ptr<MeshResource> m_MeshResource;
		//�}���`���b�V�����\�[�X
		weak_ptr<MultiMeshResource> m_MultiMeshResource;

		Dx12DrawResources<ShadowConstants> m_Dx12DrawResources;

		Impl()
		{}
		~Impl() {}
	};

	float Shadowmap::Impl::m_LightHeight(100.0f);
	float Shadowmap::Impl::m_LightNear(1.0f);
	float Shadowmap::Impl::m_LightFar(200.0f);
	float Shadowmap::Impl::m_ViewWidth(32.0f);
	float Shadowmap::Impl::m_ViewHeight(32.0f);





	//--------------------------------------------------------------------------------------
	//	class Shadowmap : public Component;
	//	�p�r: �V���h�E�}�b�v�R���|�[�l���g�i�O�����p�j
	//--------------------------------------------------------------------------------------
	Shadowmap::Shadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	Shadowmap::~Shadowmap() {}

	float Shadowmap::GetLightHeight() { return Impl::m_LightHeight; }
	float Shadowmap::GetLightNear() { return  Impl::m_LightNear; }
	float Shadowmap::GetLightFar() { return  Impl::m_LightFar; }
	float Shadowmap::GetViewWidth() { return  Impl::m_ViewWidth; }
	float Shadowmap::GetViewHeight() { return  Impl::m_ViewHeight; }

	void Shadowmap::SetLightHeight(float f) { Impl::m_LightHeight = f; }
	void Shadowmap::SetLightNear(float f) { Impl::m_LightNear = f; }
	void Shadowmap::SetLightFar(float f) { Impl::m_LightFar = f; }
	void Shadowmap::SetViewWidth(float f) { Impl::m_ViewWidth = f; }
	void Shadowmap::SetViewHeight(float f) { Impl::m_ViewHeight = f; }
	void Shadowmap::SetViewSize(float f) { Impl::m_ViewWidth = Impl::m_ViewHeight = f; }



	shared_ptr<MeshResource> Shadowmap::GetMeshResource(bool ExceptionActive) const {
		auto shptr = pImpl->m_MeshResource.lock();
		if (shptr) {
			return shptr;
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"���b�V�����\�[�X��������܂���",
					L"if (pImpl->m_MeshResource.expired())",
					L"ShadowmapComp::GetMeshResource()"
				);
			}
		}
		return nullptr;
	}


	void Shadowmap::SetMeshResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"���b�V���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"ShadowmapComp::SetMeshResource()"
				);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...) {
			throw;
		}
	}
	void Shadowmap::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr) {
		pImpl->m_MeshResource = MeshResourcePtr;
	}

	shared_ptr<MultiMeshResource> Shadowmap::GetMultiMeshResource(bool ExceptionActive) const {
		auto shptr = pImpl->m_MultiMeshResource.lock();
		if (shptr) {
			return shptr;
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"���b�V�����\�[�X��������܂���",
					L"if (pImpl->m_MultiMeshResource.expired())",
					L"ShadowmapComp::GetMultiMeshResource()"
				);
			}
		}
		return nullptr;
	}
	void Shadowmap::SetMultiMeshResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"���b�V���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"ShadowmapComp::SetMultiMeshResource()"
				);
			}
			pImpl->m_MultiMeshResource = App::GetApp()->GetResource<MultiMeshResource>(ResKey);
		}
		catch (...) {
			throw;
		}
	}
	void Shadowmap::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr) {
		pImpl->m_MultiMeshResource = MeshResourcePtr;
	}


	void Shadowmap::OnCreate() {
		//�R���X�^���g�o�b�t�@�t���[�g�V�O�l�`��
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateCbv();
		auto Dev = App::GetApp()->GetDeviceResources();
		//�f�X�N�v���^�q�[�v�쐬
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
				= Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//CbvSrv�f�X�N�v���^�q�[�v(�R���X�^���g�o�b�t�@�̂�)
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);
			//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.clear();
			CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(CbvHandle);
		}
		///�R���X�^���g�o�b�t�@�쐬
		{
			//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
			UINT ConstBuffSize = (sizeof(ShadowConstants) + 255) & ~255;
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"Shadowmap::Impl::CreateConstantBuffer()"
			);
			//�R���X�^���g�o�b�t�@�̃r���[���쐬
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
			cbvDesc.SizeInBytes = ConstBuffSize;
			//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
			//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"Shadowmap::CreateNotShadow()"
			);
		}
		///�p�C�v���C���X�e�[�g�쐬
		{
			pImpl->m_Dx12DrawResources.m_PipelineState 
				= PipelineState::CreateShadowmap3D<VertexPositionNormalTexture, VSShadowmap>(pImpl->m_Dx12DrawResources.m_RootSignature, pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}
		///�R�}���h���X�g�쐬
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_PipelineState);
			CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		}
	}

	void Shadowmap::UpdateConstantBuffer() {
		//�X�V
		memcpy(pImpl->m_Dx12DrawResources.m_pConstantBuffer, reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer),
			sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer));
	}

	void Shadowmap::DrawObject() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto ShMesh = GetMeshResource(false);
		if (!ShMesh) {
			return;
		}
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(pImpl->m_Dx12DrawResources.m_PipelineState, pImpl->m_Dx12DrawResources.m_CommandList);
		//���b�V�����X�V����Ă���΃��\�[�X�X�V
		ShMesh->UpdateResources<VertexPositionNormalTexture>(pImpl->m_Dx12DrawResources.m_CommandList);
		//���[�g�V�O�l�`���̃Z�b�g
		pImpl->m_Dx12DrawResources.m_CommandList->SetGraphicsRootSignature(pImpl->m_Dx12DrawResources.m_RootSignature.Get());
		//�f�X�N�v���^�q�[�v�̃Z�b�g
		ID3D12DescriptorHeap* ppHeaps[] = { pImpl->m_Dx12DrawResources.m_DescriptorHeap.Get() };
		pImpl->m_Dx12DrawResources.m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		//GPU�f�X�N�v���^�q�[�v�n���h���̃Z�b�g
		for (size_t i = 0; i < pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.size(); i++) {
			pImpl->m_Dx12DrawResources.m_CommandList->SetGraphicsRootDescriptorTable((UINT)i, pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec[i]);
		}

		auto ShadowMapDimension = Dev->GetShadowMapRenderTarget()->GetShadowMapDimension();


		D3D12_VIEWPORT Viewport = {};
		Viewport.Width = static_cast<float>(ShadowMapDimension);
		Viewport.Height = static_cast<float>(ShadowMapDimension);
		Viewport.MaxDepth = 1.0f;

		D3D12_RECT ScissorRect = {};

		ScissorRect.right = static_cast<LONG>(ShadowMapDimension);
		ScissorRect.bottom = static_cast<LONG>(ShadowMapDimension);


		pImpl->m_Dx12DrawResources.m_CommandList->RSSetViewports(1, &Viewport);
		pImpl->m_Dx12DrawResources.m_CommandList->RSSetScissorRects(1, &ScissorRect);

		//�f�v�X�X�e���V���r���[�̃n���h�����擾
		auto SMRenderTarget = Dev->GetShadowMapRenderTarget();
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = SMRenderTarget->GetDsvHandle();
		//�擾�����n���h�����Z�b�g
		pImpl->m_Dx12DrawResources.m_CommandList->OMSetRenderTargets(0, nullptr, FALSE, &dsvHandle);

		pImpl->m_Dx12DrawResources.m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pImpl->m_Dx12DrawResources.m_CommandList->IASetVertexBuffers(0, 1, &ShMesh->GetVertexBufferView());
		//�C���f�b�N�X�o�b�t�@���Z�b�g
		pImpl->m_Dx12DrawResources.m_CommandList->IASetIndexBuffer(&ShMesh->GetIndexBufferView());
		//�C���f�b�N�X�`��
		pImpl->m_Dx12DrawResources.m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(pImpl->m_Dx12DrawResources.m_CommandList.Get());
	}



	void Shadowmap::OnDraw() {
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage) {
			return;
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();

		//�s��̒�`
		bsm::Mat4x4 World, LightView, LightProj;
		//�s��̒�`
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//���[���h�s��̌���
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//�r���[�s��̌���
		auto StageView = PtrStage->GetView();
		//���C�g�̎擾
		auto StageLight = PtrGameObject->OnGetDrawLight();
		//�ʒu�̎擾
		auto Pos = PtrTrans->GetWorldMatrix().transInMatrix();
		bsm::Vec3 PosSpan = StageLight.m_Directional;
		PosSpan *= 0.1f;
//		Pos += PosSpan;
		//���[���h�s��̌���
		World.affineTransformation(
			PtrTrans->GetScale(),			//�X�P�[�����O
			PtrTrans->GetPivot(),		//��]�̒��S�i�d�S�j
			PtrTrans->GetQuaternion(),				//��]�p�x
			Pos				//�ʒu
		);
		bsm::Mat4x4 RealWorldMatrix;
		//���[���h�s��̌���
//		if (data.m_UseMeshToTransformMatrix) {
//			RealWorldMatrix = data.m_MeshToTransformMatrix * GetMeshToTransformMatrix();
//			RealWorldMatrix *= World;
//		}
//		else {
			RealWorldMatrix = GetMeshToTransformMatrix() * World;
//		}
		//�r���[�s��̌���
//		auto StageView = PtrStage->GetView();
		bsm::Vec3 LightDir = -1.0 * StageLight.m_Directional;
		bsm::Vec3 LightAt = StageView->GetTargetCamera()->GetAt();
		bsm::Vec3 LightEye = LightAt + (LightDir * GetLightHeight());
		//���C�g�̃r���[�Ǝˉe���v�Z
		LightView.lookatLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
		LightProj.orthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());

		pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer.mWorld = bsm::transpose(RealWorldMatrix);
		pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer.mView = bsm::transpose(LightView);
		pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer.mProj = bsm::transpose(LightProj);

		UpdateConstantBuffer();
		DrawObject();

	}


	//static�ϐ��̎���
	vector<bsm::Vec3> DrawObjectBase::m_TempPositions;


	//--------------------------------------------------------------------------------------
	///	Simple�`��Ɏg�p����\����(�e�Ή�)
	//--------------------------------------------------------------------------------------
	struct SmDrawObject :public DrawObjectBase {
		//���_�ύX����ꍇ�̃��b�V���i�I���W�i���j
		shared_ptr<MeshResource> m_OriginalMeshResource;
		//�I���W�i�����b�V�����g�����ǂ���
		bool m_UseOriginalMeshResource;
		///�e�N�X�`�����\�[�X
		weak_ptr<TextureResource> m_TextureResource;
		///�G�~�b�V�u�F
		bsm::Col4 m_Emissive;
		/// �f�t���[�Y�F
		bsm::Col4 m_Diffuse;
		/// �X�y�L�����[�F
		bsm::Col4 m_Specular;
		///�e�𓊉e���邩�ǂ���
		bool m_OwnShadowActive;
		//���f���ɓ����Ă���Diffuse���g�����ǂ���
		bool m_ModelDiffusePriority;
		//���f���ɓ����Ă���Emissive���g�����ǂ���
		bool m_ModelEmissivePriority;
		//���f���ɓ����Ă���e�N�X�`�����g�����ǂ���
		bool m_ModelTextureEnabled;
		///Instance�`��p
		///Instance�ő�l
		size_t m_MaxInstance;
		/// �s��p�̃��b�V��
		shared_ptr<MeshResource> m_InstanceMatrixMesh;
		///�s��̔z��
		vector<bsm::Mat4x4> m_MatrixVec;
		//�s��������N���A���邩�ǂ���
		bool m_AutoClearMatrixVec;
		SmDrawObject() :
			m_UseOriginalMeshResource(false),
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_Specular(0, 0, 0, 0),
			m_OwnShadowActive(false),
			m_ModelDiffusePriority(false),
			m_ModelEmissivePriority(false),
			m_ModelTextureEnabled(true),
			m_MaxInstance(2000),
			m_AutoClearMatrixVec(false)
		{}
	};

	//--------------------------------------------------------------------------------------
	//	SmBaseDraw::Impl
	//--------------------------------------------------------------------------------------
	struct SmBaseDraw::Impl {
		SmDrawObject m_SmDrawObject;
		Dx12DrawResources<SimpleConstants> m_Dx12DrawResources;
	};

	//--------------------------------------------------------------------------------------
	///	Simple�`��I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	SmBaseDraw::SmBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g�̂RD
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	SmBaseDraw::~SmBaseDraw() {}

	void SmBaseDraw::SetConstants(Dx12Constants<SimpleConstants>& SmCb) {
		//�s��̒�`
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//�s��̒�`
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//���[���h�s��̌���
//		if (data.m_UseMeshToTransformMatrix) {
//			World = data.m_MeshToTransformMatrix * GetMeshToTransformMatrix();
//			World *= PtrTrans->GetWorldMatrix();
//		}
//		else {
			World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
//		}
		//�]�u����
		World.transpose();
		//�J�����𓾂�
		auto CameraPtr = GetGameObject()->OnGetDrawCamera();
		//�r���[�Ǝˉe�s��𓾂�
		ViewMat = CameraPtr->GetViewMatrix();
		//�]�u����
		ViewMat.transpose();
		//�]�u����
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		SmCb.m_ConstantBuffer.World = World;
		SmCb.m_ConstantBuffer.View = ViewMat;
		SmCb.m_ConstantBuffer.Projection = ProjMat;
		//�G�~�b�V�u
		SmCb.m_ConstantBuffer.Emissive = GetEmissive();
		//�f�t�B�[�Y
		SmCb.m_ConstantBuffer.Diffuse = GetDiffuse();
		//�X�y�L�����[
		SmCb.m_ConstantBuffer.Specular = GetSpecular();
		//���C�e�B���O
		auto StageLight = GetGameObject()->OnGetDrawLight();
		SmCb.m_ConstantBuffer.LightDir = StageLight.m_Directional;
		SmCb.m_ConstantBuffer.LightDir.w = 1.0f;
		SmCb.m_ConstantBuffer.EyePos = CameraPtr->GetEye();
		SmCb.m_ConstantBuffer.EyePos.w = 1.0f;
		//�e�p
		if (GetOwnShadowActive()) {
			bsm::Vec3 CalcLightDir = -1.0 * StageLight.m_Directional;
			bsm::Vec3 LightAt = CameraPtr->GetAt();
			bsm::Vec3 LightEye = CalcLightDir;
			LightEye *= Shadowmap::GetLightHeight();
			LightEye = LightAt + LightEye;
			SmCb.m_ConstantBuffer.LightPos = LightEye;
			SmCb.m_ConstantBuffer.LightPos.w = 1.0f;
			bsm::Mat4x4 LightView, LightProj;
			//���C�g�̃r���[�Ǝˉe���v�Z
			LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
				Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
			SmCb.m_ConstantBuffer.LightView = bsm::transpose(LightView);
			SmCb.m_ConstantBuffer.LightProjection = bsm::transpose(LightProj);
		}
		//�{�[���̐ݒ�
/*
����`
*/
	}

	//�s��o�b�t�@�̍쐬
	void SmBaseDraw::CreateMatrixBuffer() {
/*
����`
*/
	}

	//�s��o�b�t�@�̃}�b�v
	void SmBaseDraw::MapMatrixBuffer() {
/*
����`
*/
	}

	bool SmBaseDraw::IsOriginalMeshUse() const {
		return pImpl->m_SmDrawObject.m_UseOriginalMeshResource;

	}

	void SmBaseDraw::SetOriginalMeshUse(bool b) {
		pImpl->m_SmDrawObject.m_UseOriginalMeshResource = b;
	}

	shared_ptr<MeshResource> SmBaseDraw::GetOriginalMeshResource() const {
		return pImpl->m_SmDrawObject.m_OriginalMeshResource;

	}

	void SmBaseDraw::SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_SmDrawObject.m_OriginalMeshResource = MeshRes;

	}

	shared_ptr<MeshResource> SmBaseDraw::GetMeshResource() const {
		if (IsOriginalMeshUse()) {
			return pImpl->m_SmDrawObject.m_OriginalMeshResource;
		}
		else {
			auto shptr = pImpl->m_SmDrawObject.m_MeshResource.lock();
			if (shptr) {
				return shptr;
			}
		}
		return nullptr;
	}

	void SmBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_SmDrawObject.m_MeshResource = MeshRes;
	}

	void SmBaseDraw::SetMeshResource(const wstring& MeshKey) {
		this->SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}

	void SmBaseDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_SmDrawObject.m_TextureResource = TextureRes;

	}

	void SmBaseDraw::SetTextureResource(const wstring& TextureKey) {
		this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<TextureResource> SmBaseDraw::GetTextureResource() const {
		//�e�N�X�`�����Ȃ����null��Ԃ�
		auto shptr = pImpl->m_SmDrawObject.m_TextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;

	}

	shared_ptr<MultiMeshResource> SmBaseDraw::GetMultiMeshResource() const {
		//�Ȃ����null��Ԃ�
		auto shptr = pImpl->m_SmDrawObject.m_MultiMeshResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;

	}

	void SmBaseDraw::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr) {
		pImpl->m_SmDrawObject.m_MultiMeshResource = MeshResourcePtr;

	}

	void SmBaseDraw::SetMultiMeshResource(const wstring& ResKey) {
		this->SetMultiMeshResource(App::GetApp()->GetResource<MultiMeshResource>(ResKey));

	}

	bsm::Col4 SmBaseDraw::GetEmissive() const {
		return pImpl->m_SmDrawObject.m_Emissive;

	}

	void SmBaseDraw::SetEmissive(const bsm::Col4& col) {
		pImpl->m_SmDrawObject.m_Emissive = col;
	}

	bsm::Col4 SmBaseDraw::GetDiffuse() const {
		return pImpl->m_SmDrawObject.m_Diffuse;

	}

	void SmBaseDraw::SetDiffuse(const bsm::Col4& col) {
		pImpl->m_SmDrawObject.m_Diffuse = col;

	}

	bsm::Col4 SmBaseDraw::GetSpecular() const {
		return pImpl->m_SmDrawObject.m_Specular;

	}

	void SmBaseDraw::SetSpecular(const bsm::Col4& col) {
		pImpl->m_SmDrawObject.m_Specular = col;
	}

	bool SmBaseDraw::GetOwnShadowActive() const {
		return pImpl->m_SmDrawObject.m_OwnShadowActive;

	}

	bool SmBaseDraw::IsOwnShadowActive() const {
		return pImpl->m_SmDrawObject.m_OwnShadowActive;

	}

	void SmBaseDraw::SetOwnShadowActive(bool b) {
		pImpl->m_SmDrawObject.m_OwnShadowActive = b;
	}

	bool SmBaseDraw::GetModelDiffusePriority() const {
		return pImpl->m_SmDrawObject.m_ModelDiffusePriority;

	}

	bool SmBaseDraw::IsModelDiffusePriority() const {
		return pImpl->m_SmDrawObject.m_ModelDiffusePriority;

	}

	void SmBaseDraw::SetModelDiffusePriority(bool b) {
		pImpl->m_SmDrawObject.m_ModelDiffusePriority = b;

	}

	bool SmBaseDraw::GetModelEmissivePriority() const {
		return pImpl->m_SmDrawObject.m_ModelEmissivePriority;

	}

	bool SmBaseDraw::IsModelEmissivePriority() const {
		return pImpl->m_SmDrawObject.m_ModelEmissivePriority;

	}

	void SmBaseDraw::SetModelEmissivePriority(bool b) {
		pImpl->m_SmDrawObject.m_ModelEmissivePriority = b;
	}

	bool SmBaseDraw::GetModelTextureEnabled() const {
		return pImpl->m_SmDrawObject.m_ModelTextureEnabled;

	}

	bool SmBaseDraw::IsModelTextureEnabled() const {
		return pImpl->m_SmDrawObject.m_ModelTextureEnabled;

	}

	void SmBaseDraw::SetModelTextureEnabled(bool b) {
		pImpl->m_SmDrawObject.m_ModelTextureEnabled = b;

	}

	void SmBaseDraw::BoneInit() {
		pImpl->m_SmDrawObject.BoneInit();

	}

	void SmBaseDraw::InterpolationMatrix(const bsm::Mat4x4& m1, const bsm::Mat4x4& m2, float t, bsm::Mat4x4& out) {
		pImpl->m_SmDrawObject.InterpolationMatrix(m1, m2, t, out);

	}

	void SmBaseDraw::AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
		float SamplesParSecond) {
		pImpl->m_SmDrawObject.AddAnimation(Name, StartSample, SampleLength, Loop, SamplesParSecond);

	}

	void SmBaseDraw::ChangeCurrentAnimation(const wstring& AnemationName, float StartTime) {
		pImpl->m_SmDrawObject.ChangeCurrentAnimation(AnemationName, StartTime);

	}

	const wstring& SmBaseDraw::GetCurrentAnimation() const {
		return pImpl->m_SmDrawObject.m_CurrentAnimeName;

	}

	float SmBaseDraw::GetCurrentAnimationTime() const {
		return pImpl->m_SmDrawObject.m_CurrentAnimeTime;
	}

	bool SmBaseDraw::IsTargetAnimeEnd() const {
		auto& AnimData = pImpl->m_SmDrawObject.GetAnimationData();
		return AnimData.m_IsAnimeEnd;
	}


	bool SmBaseDraw::UpdateAnimation(float ElapsedTime) {
		return pImpl->m_SmDrawObject.UpdateAnimation(ElapsedTime);

	}

	const vector< bsm::Mat4x4 >* SmBaseDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_SmDrawObject.m_LocalBonesMatrix;
	}

	vector< bsm::Mat4x4 >& SmBaseDraw::GetVecLocalBones() {
		return pImpl->m_SmDrawObject.m_LocalBonesMatrix;
	}


	const vector< bsm::Mat4x4 >* SmBaseDraw::GetVecMultiLocalBonesPtr(size_t index) const {
		if (pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix.size() > index) {
			return &(pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix[index]);
		}
		return nullptr;
	}

	vector< bsm::Mat4x4 >& SmBaseDraw::GetVecMultiLocalBones(size_t index) {
		if (pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix.size() <= index) {
			throw BaseException(
				L"�C���f�b�N�X������𒴂��Ă܂�",
				L"if (pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix.size() <= index)",
				L"SmBaseDraw::GetVecMultiLocalBones"
			);
		}
		return pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix[index];
	}



	size_t SmBaseDraw::GetMaxInstance() const {
		return pImpl->m_SmDrawObject.m_MaxInstance;
	}

	void SmBaseDraw::ResizeMaxInstance(size_t NewSize) {
		pImpl->m_SmDrawObject.m_MaxInstance = NewSize;
		CreateMatrixBuffer();
	}

	void SmBaseDraw::AddMatrix(const bsm::Mat4x4& NewMat) {
		if (pImpl->m_SmDrawObject.m_MatrixVec.size() >= GetMaxInstance()) {
			throw BaseException(
				L"�C���X�^���X����𒴂��Ă܂�",
				L"if (pImpl->m_MatrixVec.size() >= GetMaxInstance())",
				L"SmBaseDraw::AddMatrix()"
			);
		}
		pImpl->m_SmDrawObject.m_MatrixVec.push_back(NewMat);
	}

	void SmBaseDraw::UpdateMultiMatrix(const vector<bsm::Mat4x4>& NewMatVec) {
		pImpl->m_SmDrawObject.m_MatrixVec.resize(NewMatVec.size());
		pImpl->m_SmDrawObject.m_MatrixVec = NewMatVec;
	}

	vector<bsm::Mat4x4>& SmBaseDraw::GetMatrixVec() const {
		return pImpl->m_SmDrawObject.m_MatrixVec;
	}

	void SmBaseDraw::ClearMatrixVec() {
		pImpl->m_SmDrawObject.m_MatrixVec.clear();
	}

	bool SmBaseDraw::IsAutoClearMatrixVec() const {
		return pImpl->m_SmDrawObject.m_AutoClearMatrixVec;
	}
	void SmBaseDraw::SetAutoClearMatrixVec(bool b) {
		pImpl->m_SmDrawObject.m_AutoClearMatrixVec = b;
	}
/*
	ComPtr<ID3D11Buffer>& SmBaseDraw::GetMatrixBuffer() const {
		return pImpl->m_SmDrawObject.m_MatrixBuffer;
	}
*/
	void SmBaseDraw::GetStaticMeshLocalPositions(vector<bsm::Vec3>& vertices) {
/*
����`
*/
	}

	void SmBaseDraw::GetStaticMeshWorldPositions(vector<bsm::Vec3>& vertices) {
		GetStaticMeshLocalPositions(vertices);
		//���[���h�s��̔��f
		auto WorldMat = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		for (auto& v : vertices) {
			v *= WorldMat;
		}
	}

	bool SmBaseDraw::HitTestStaticMeshSegmentTriangles(const bsm::Vec3& StartPos, const bsm::Vec3& EndPos, bsm::Vec3& HitPoint,
		TRIANGLE& RetTri, size_t& RetIndex) {
		GetStaticMeshWorldPositions(pImpl->m_SmDrawObject.m_TempPositions);
		for (size_t i = 0; i < pImpl->m_SmDrawObject.m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_SmDrawObject.m_TempPositions[i];
			tri.m_B = pImpl->m_SmDrawObject.m_TempPositions[i + 1];
			tri.m_C = pImpl->m_SmDrawObject.m_TempPositions[i + 2];
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


	void SmBaseDraw::GetSkinedMeshLocalPositions(vector<bsm::Vec3>& vertices) {
/*
����`
*/
	}

	void SmBaseDraw::GetSkinedMeshWorldPositions(vector<bsm::Vec3>& vertices) {
		GetSkinedMeshLocalPositions(vertices);
		//���[���h�s��̔��f
		auto WorldMat = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		for (auto& v : vertices) {
			v *= WorldMat;
		}
	}

	bool SmBaseDraw::HitTestSkinedMeshSegmentTriangles(const bsm::Vec3& StartPos, const bsm::Vec3& EndPos,
		bsm::Vec3& HitPoint, TRIANGLE& RetTri, size_t& RetIndex) {
		GetSkinedMeshWorldPositions(pImpl->m_SmDrawObject.m_TempPositions);
		for (size_t i = 0; i < pImpl->m_SmDrawObject.m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_SmDrawObject.m_TempPositions[i];
			tri.m_B = pImpl->m_SmDrawObject.m_TempPositions[i + 1];
			tri.m_C = pImpl->m_SmDrawObject.m_TempPositions[i + 2];
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



	///�V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j�쐬
	void SmBaseDraw::CreateShaderResourceView() {
		auto ShPtr = GetTextureResource();
		if (!ShPtr) {
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		//�e�N�X�`���n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
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
		if (IsOwnShadowActive()) {
			auto ShdowRender = Dev->GetShadowMapRenderTarget();
			CD3DX12_CPU_DESCRIPTOR_HANDLE ShadowHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);
			D3D12_SHADER_RESOURCE_VIEW_DESC shadowSrvDesc = {};
			shadowSrvDesc.Format = DXGI_FORMAT_R32_FLOAT;
			shadowSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			shadowSrvDesc.Texture2D.MipLevels = 1;
			shadowSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			Dev->GetD3DDevice()->CreateShaderResourceView(ShdowRender->GetDepthStencil().Get(), &shadowSrvDesc, ShadowHandle);
		}
	}


	void SmBaseDraw::RefreshTrace() {
		//�u�����h�X�e�[�g�ƃ��X�^���C�U�����ւ�
		if (GetGameObject()->IsAlphaActive()) {
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
			pImpl->m_Dx12DrawResources.m_PineLineDesc.BlendState = blend_desc;
		}
		else {
			pImpl->m_Dx12DrawResources.m_PineLineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		}
		pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);

		pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);

	}

	void SmBaseDraw::UpdateConstantBuffer() {
		memcpy(pImpl->m_Dx12DrawResources.m_pConstantBuffer, reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer),
			sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer));
	}


	///�`�揈��
	void SmBaseDraw::DrawObject() {
		auto ShMesh = GetMeshResource();
		auto ShTex = GetTextureResource();
		if (!ShMesh) {
			return;
		}
		//��������
		RefreshTrace();
		//�R�}���h���X�g�̃��Z�b�g
		if (GetGameObject()->IsAlphaActive()) {
			CommandList::Reset(pImpl->m_Dx12DrawResources.m_CullFrontPipelineState, pImpl->m_Dx12DrawResources.m_CommandList);
		}
		else {
			CommandList::Reset(pImpl->m_Dx12DrawResources.m_CullBackPipelineState, pImpl->m_Dx12DrawResources.m_CommandList);
		}
		ShMesh->UpdateResources<VertexPositionNormalTexture>(pImpl->m_Dx12DrawResources.m_CommandList);
		if (ShTex) {
			ShTex->UpdateResources(pImpl->m_Dx12DrawResources.m_CommandList);
		}
		//�`��
		pImpl->m_Dx12DrawResources.m_CommandList->SetGraphicsRootSignature(pImpl->m_Dx12DrawResources.m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { pImpl->m_Dx12DrawResources.m_DescriptorHeap.Get(), pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap.Get() };
		pImpl->m_Dx12DrawResources.m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		for (size_t i = 0; i < pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.size(); i++) {
			pImpl->m_Dx12DrawResources.m_CommandList->SetGraphicsRootDescriptorTable((UINT)i, pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		pImpl->m_Dx12DrawResources.m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		pImpl->m_Dx12DrawResources.m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		pImpl->m_Dx12DrawResources.m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		pImpl->m_Dx12DrawResources.m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pImpl->m_Dx12DrawResources.m_CommandList->IASetIndexBuffer(&ShMesh->GetIndexBufferView());
		pImpl->m_Dx12DrawResources.m_CommandList->IASetVertexBuffers(0, 1, &ShMesh->GetVertexBufferView());


		pImpl->m_Dx12DrawResources.m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		if (GetGameObject()->IsAlphaActive()) {
			pImpl->m_Dx12DrawResources.m_CommandList->SetPipelineState(pImpl->m_Dx12DrawResources.m_CullBackPipelineState.Get());
			pImpl->m_Dx12DrawResources.m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		}

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(pImpl->m_Dx12DrawResources.m_CommandList.Get());
	}

	void SmBaseDraw::CreatePNNotShadow() {
		//������
		

	}


	void SmBaseDraw::CreatePNTNotShadow() {
		//���[�g�V�O�l�`��
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrvSmpCbv();
		//�f�X�N�v���^�q�[�v
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//�f�X�N�v���^�q�[�v
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
			//�T���v���[�f�X�N�v���^�q�[�v
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
			//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.clear();
			CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SrvHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
				pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SamplerHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(CbvHandle);

		}
		//�T���v���[
		{
			auto SamplerDescriptorHandle = pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
		}
		//�R���X�^���g�o�b�t�@
		{
			//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
			UINT ConstBuffSize = (sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;
			//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"PNTStaticDraw::CreateNotShadow()"
			);
			//�R���X�^���g�o�b�t�@�̃r���[���쐬
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;
			//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
			//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);

			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"PNTStaticDraw::CreateNotShadow()"
			);

		}
		//�V�F�[�_���\�[�X�r���[�̓e�N�X�`���Z�b�g���ɍ쐬
		//�p�C�v���C���X�e�[�g�̍쐬
		{
			PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(pImpl->m_Dx12DrawResources.m_RootSignature, pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_CullBackPipelineState);
			//�R���X�^���g�o�b�t�@�X�V
			UpdateConstantBuffer();
			CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		}
	}


	void SmBaseDraw::CreatePNTWithShadow() {
		//���[�g�V�O�l�`��
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrv2Smp2Cbv();
		//�f�X�N�v���^�q�[�v
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

			//�f�X�N�v���^�q�[�v
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 2);
			//�T���v���[�f�X�N�v���^�q�[�v
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(2);
			//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.clear();
			CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle1(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SrvHandle1);
			CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle2(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SrvHandle2);

			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle1(
				pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SamplerHandle1);

			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle2(
				pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_SamplerDescriptorHandleIncrementSize
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(SamplerHandle2);

			CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				2,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(CbvHandle);
		}
		//�T���v���[
		{
			auto SamplerDescriptorHandleWithShadow1 = pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandleWithShadow1);

			CD3DX12_CPU_DESCRIPTOR_HANDLE SamplerDescriptorHandleWithShadow2{
				pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_SamplerDescriptorHandleIncrementSize
			};
			DynamicSampler::CreateSampler(SamplerState::ComparisonLinear, SamplerDescriptorHandleWithShadow2);
		}
		//�R���X�^���g�o�b�t�@
		{
			//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
			UINT ConstBuffSize = (sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;
			//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"PNTStaticDraw::CreateWithShadow()"
			);
			//�R���X�^���g�o�b�t�@�̃r���[���쐬
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;
			//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
			//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				2,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);

			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"PNTStaticDraw::CreateNotShadow()"
			);
		}
		//�V�F�[�_���\�[�X�r���[�̓e�N�X�`���Z�b�g���ɍ쐬
		//�p�C�v���C���X�e�[�g�̍쐬
		{
			//�V���h�E�t���p�C�v���C���X�e�[�g�̍쐬
			PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStaticShadow, PSPNTStaticShadow>(pImpl->m_Dx12DrawResources.m_RootSignature,
				pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_CullBackPipelineState);
			//�R���X�^���g�o�b�t�@�X�V
			UpdateConstantBuffer();
			CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		}
	}

	void SmBaseDraw::DrawPN() {

	}


	void SmBaseDraw::DrawPNT() {
		auto PtrStage = GetGameObject()->GetStage();
		auto PtrMeshResource = GetMeshResource();
		if (GetTextureResource()) {
			pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer.ActiveFlg.x = 1;
			CreateShaderResourceView();
		}
		else {
			pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer.ActiveFlg.x = 0;
		}
		SetConstants(pImpl->m_Dx12DrawResources.m_Dx12Constants);
		//�X�V
		UpdateConstantBuffer();
		DrawObject();

	}




	const Dx12DrawResources<SimpleConstants>& SmBaseDraw::GetSimpleDrawResources() const {
		return pImpl->m_Dx12DrawResources;
	}

	Dx12DrawResources<SimpleConstants>& SmBaseDraw::GetSimpleDrawResources() {
		return pImpl->m_Dx12DrawResources;

	}



	//--------------------------------------------------------------------------------------
	///	PNStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNStaticDraw::PNStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{
	}

	PNStaticDraw::~PNStaticDraw() {}

	void PNStaticDraw::OnCreate() {
		//���C�e�B���O�݂̂��Ƌɒ[�ɂȂ�̂Œ���
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
		//PN�e�����ɏ�����
		CreatePNNotShadow();
	}

	void PNStaticDraw::OnDraw() {
		DrawPN();
	}

	//--------------------------------------------------------------------------------------
	///	PCTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCTStaticDraw::PCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PCTStaticDraw::~PCTStaticDraw() {}

	void PCTStaticDraw::OnCreate() {}

	void PCTStaticDraw::OnDraw() {

	}


	//--------------------------------------------------------------------------------------
	///	PNTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{
	}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::SetOwnShadowActive(bool b) {
		if (IsOwnShadowActive()) {
			//���͉e�t��
			if (!b) {
				//PNT�e�����ɕύX
				CreatePNTNotShadow();
				SmBaseDraw::SetOwnShadowActive(false);
			}
		}
		else {
			//���͉e����
			if (b) {
				//PNT�e�t���ɕύX
				CreatePNTWithShadow();
				SmBaseDraw::SetOwnShadowActive(true);
			}
		}
	}


	void PNTStaticDraw::OnCreate() {
		//���C�e�B���O�݂̂��Ƌɒ[�ɂȂ�̂Œ���
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
		//PNT�e�����ɏ�����
		CreatePNTNotShadow();
	}

	void PNTStaticDraw::OnDraw() {
		DrawPNT();
	}

}
//end basecross
