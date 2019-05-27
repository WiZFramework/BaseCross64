/*!
@file DrawComponents.cpp
@brief 描画コンポーネント実体
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
	///	汎用描画用
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
	//	用途: Implイディオム
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
	///	描画コンポーネントの親クラス実体
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
	//ParticleDraw準備のための構造体
	//--------------------------------------------------------------------------------------
	struct DrawParticleSprite {
		//パーティクルのカメラまでの距離
		float m_ToCaneraLength;
		//ワールド行列
		bsm::Mat4x4 m_WorldMatrix;
		//テクスチャ
		shared_ptr<TextureResource> m_TextureRes;
		//Diffuseカラー
		bsm::Col4 m_Diffuse;
		DrawParticleSprite() :
			m_ToCaneraLength(0),
			m_Diffuse(1, 1, 1, 1)

		{}
	};



	//--------------------------------------------------------------------------------------
	//	struct PCTParticleDraw::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct PCTParticleDraw::Impl {
		//描画コンテキスト
		shared_ptr<MeshResource> m_MeshResource;	//メッシュリソース
		vector<DrawParticleSprite> m_DrawParticleSpriteVec;
		const size_t m_MaxInstance;				///<インスタンス最大値
//		ComPtr<ID3D11Buffer> m_MatrixBuffer;	///<行列用の頂点バッファ
												//加算処理するかどうか
		bool m_Addtype;
		Impl(size_t MaxInstance, bool AddType) :
			m_MaxInstance(MaxInstance),
			m_Addtype(AddType)
		{}
		~Impl() {}
		//頂点バッファの作成
		void CreateParticleBuffers();
	};


	void PCTParticleDraw::Impl::CreateParticleBuffers() {
		try {
			float HelfSize = 0.5f;
			bsm::Vec4 col(1.0f, 1.0f, 1.0f, 1.0f);
			//頂点配列
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, HelfSize, 0),  col,bsm::Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, HelfSize, 0), col, bsm::Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, -HelfSize, 0),  col,bsm::Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, -HelfSize, 0),  col, bsm::Vec2(1.0f, 1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//メッシュの作成
			m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, false);

			//インスタンス行列バッファの作成
			//Max値で作成する
			vector<bsm::Mat4x4> matrices(m_MaxInstance, bsm::Mat4x4());
//			MeshResource::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);

		}
		catch (...) {
			throw;
		}
	}




	//--------------------------------------------------------------------------------------
	///	PCTParticle描画コンポーネント(パーティクル描画)
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
		//メッシュ
		shared_ptr<MeshResource> m_SpriteMesh;
		///テクスチャリソース
		weak_ptr<TextureResource> m_TextureResource;
		//エミッシブ色
		bsm::Col4 m_Emissive;
		//デフューズ色
		bsm::Col4 m_Diffuse;
		//スプライト用のDx12リソース
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
		//行列の取得
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		bsm::Mat4x4 World, Proj;
		World = PtrTrans->Get2DWorldMatrix();
		//射影行列の決定
		GetGameObject()->OnGet2DDrawProjMatrix(Proj);
		//行列の合成
		World *= Proj;
		//エミッシブ
		SpCb.m_ConstantBuffer.Emissive = GetEmissive();
		//デフィーズはすべて通す
		SpCb.m_ConstantBuffer.Diffuse = GetDiffuse();
		//行列の設定
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
		//テクスチャがなければnullを返す
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
		//ルートシグネチャ
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrvSmpCbv();

		//デスクプリタヒープ
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//CbvSrvデスクプリタヒープ
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
			//サンプラーデスクプリタヒープ
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
			//GPU側デスクプリタヒープのハンドルの配列の作成
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
		//サンプラー
		{
			auto SamplerDescriptorHandle = pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::SamplerNone, SamplerDescriptorHandle);
		}
		//コンスタントバッファ
		{
			//コンスタントバッファは256バイトにアラインメント
			UINT ConstBuffSize = (sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;

			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"コンスタントバッファ用のアップリードヒープ作成に失敗しました",
				L"Device->GetDevice()->CreateCommittedResource()",
				L"SpriteBaseDraw::CreatePC()"
			);

			//コンスタントバッファのビューを作成
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;

			//コンスタントバッファビューを作成すべきデスクリプタヒープ上のハンドルを取得
			//シェーダーリソースがある場合、コンスタントバッファはシェーダーリソースビューのあとに配置する
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				0,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);
			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);

			//コンスタントバッファのアップロードヒープのマップ
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"コンスタントバッファのマップに失敗しました",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"SpriteBaseDraw::CreatePC()"
			);
		}
		//パイプラインステート
		{
			//パイプラインステートの定義
			PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(pImpl->m_Dx12DrawResources.m_RootSignature, pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}
		//コマンドリストは裏面カリングに初期化
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_CullBackPipelineState);
			//コンスタントバッファ更新
			UpdateConstantBuffer();
			CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		}
	}

	void SpriteBaseDraw::CreatePT() {
		//ルートシグネチャ
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrvSmpCbv();
		//デスクプリタヒープ
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//デスクプリタヒープ
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
			//サンプラーデスクプリタヒープ
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
			//GPU側デスクプリタヒープのハンドルの配列の作成
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
		//サンプラー
		{
			auto SamplerDescriptorHandle = pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle);
		}
		//コンスタントバッファ
		{
			//コンスタントバッファは256バイトにアラインメント
			UINT ConstBuffSize = (sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;
			//コンスタントバッファリソース（アップロードヒープ）の作成
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"コンスタントバッファ用のアップロードヒープ作成に失敗しました",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"SpriteBaseDraw::CreatePT()"
			);
			//コンスタントバッファのビューを作成
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;
			//コンスタントバッファビューを作成すべきデスクプリタヒープ上のハンドルを取得
			//シェーダリソースがある場合コンスタントバッファはシェーダリソースビューのあとに設置する
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);

			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//コンスタントバッファのアップロードヒープのマップ
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"コンスタントバッファのマップに失敗しました",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"SpriteBaseDraw::CreatePT()"
			);

		}
		//シェーダリソースビューはテクスチャセット時に作成
		//パイプラインステートの作成
		{
			PipelineState::CreateDefault2D<VertexPositionTexture, VSPTSprite, PSPTSprite>(pImpl->m_Dx12DrawResources.m_RootSignature, pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}
		//コマンドリストは裏面カリングに初期化
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_CullBackPipelineState);
			//コンスタントバッファ更新
			UpdateConstantBuffer();
			CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		}
	}

	void SpriteBaseDraw::CreatePCT() {
		//ルートシグネチャ
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrvSmpCbv();

		//デスクプリタヒープ
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//デスクプリタヒープ
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
			//サンプラーデスクプリタヒープ
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
			//GPU側デスクプリタヒープのハンドルの配列の作成
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

		//サンプラー
		{
			auto SamplerDescriptorHandle = pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::LinearWrap, SamplerDescriptorHandle);
		}

		//コンスタントバッファ
		{
			//コンスタントバッファは256バイトにアラインメント
			UINT ConstBuffSize = (sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;
			//コンスタントバッファリソース（アップロードヒープ）の作成
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"コンスタントバッファ用のアップロードヒープ作成に失敗しました",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"SpriteBaseDraw::CreatePT()"
			);
			//コンスタントバッファのビューを作成
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;
			//コンスタントバッファビューを作成すべきデスクプリタヒープ上のハンドルを取得
			//シェーダリソースがある場合コンスタントバッファはシェーダリソースビューのあとに設置する
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);

			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//コンスタントバッファのアップロードヒープのマップ
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"コンスタントバッファのマップに失敗しました",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"SpriteBaseDraw::CreatePT()"
			);

		}

		//パイプラインステートの作成
		{
			PipelineState::CreateDefault2D<VertexPositionColorTexture, VSPCTSprite, PSPCTSprite>(pImpl->m_Dx12DrawResources.m_RootSignature, pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}

		//コマンドリストは裏面カリングに初期化
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_CullBackPipelineState);
			//コンスタントバッファ更新
			UpdateConstantBuffer();
			CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		}
	}


	///シェーダーリソースビュー（テクスチャ）作成
	void SpriteBaseDraw::CreateShaderResourceView() {
		auto ShPtr = GetTextureResource();
		if (!ShPtr) {
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		//テクスチャハンドルを作成
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//テクスチャのシェーダリソースビューを作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//フォーマット
		srvDesc.Format = ShPtr->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = ShPtr->GetTextureResDesc().MipLevels;
		//シェーダリソースビュー
		Dev->GetD3DDevice()->CreateShaderResourceView(
			ShPtr->GetTexture().Get(),
			&srvDesc,
			Handle);
	}


	void SpriteBaseDraw::RefreshTrace() {
		//ブレンドステートとラスタライザ差し替え
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
		//更新
		UpdateConstantBuffer();
		DrawObject<VertexPositionColor>();
	}

	void SpriteBaseDraw::DrawPT() {
		auto PtrStage = GetGameObject()->GetStage();
		auto PtrMeshResource = GetMeshResource();
		CreateShaderResourceView();
		SetConstants(pImpl->m_Dx12DrawResources.m_Dx12Constants);
		//更新
		UpdateConstantBuffer();
		DrawObject<VertexPositionTexture>();

	}

	void SpriteBaseDraw::DrawPCT() {
		CreateShaderResourceView();
		SetConstants(pImpl->m_Dx12DrawResources.m_Dx12Constants);
		//更新
		UpdateConstantBuffer();
		DrawObject<VertexPositionColorTexture>();
	}

	//--------------------------------------------------------------------------------------
	///	PCSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
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
			//作成されてなかったら作成する
			float HelfSize = 0.5f;
			//頂点配列
			vector<VertexPositionColor> Vertices = {
				{ VertexPositionColor(bsm::Vec3(-HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			{ VertexPositionColor(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			{ VertexPositionColor(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			{ VertexPositionColor(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			CreateMesh(Vertices, indices);
		}
		CreatePC();
	}

	void PCSpriteDraw::OnDraw() {
		DrawPC();
	}

	//--------------------------------------------------------------------------------------
	///	PTSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
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
			//作成されてなかったら作成する
			float HelfSize = 0.5f;
			//頂点配列
			vector<VertexPositionTexture> Vertices = {
				{ VertexPositionTexture(bsm::Vec3(-HelfSize, HelfSize, 0), bsm::Vec2(0.0f, 0.0f)) },
			{ VertexPositionTexture(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Vec2(1.0f, 0.0f)) },
			{ VertexPositionTexture(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Vec2(0.0f, 1.0f)) },
			{ VertexPositionTexture(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Vec2(1.0f, 1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			CreateMesh(Vertices, indices);
		}
		CreatePT();
	}

	void PTSpriteDraw::OnDraw() {
		DrawPT();
	}

	//--------------------------------------------------------------------------------------
	///	PCTSprite描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//パイプラインステートをデフォルトの2D
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
			//作成されてなかったら作成する
			float HelfSize = 0.5f;
			//頂点配列
			vector<VertexPositionColorTexture> Vertices = {
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, HelfSize, 0),bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(1.0f, 1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//メッシュの作成（変更できる）
			CreateMesh(Vertices, indices);
		}

		CreatePCT();
	}

	void PCTSpriteDraw::OnDraw() {
		DrawPCT();
	}



	//--------------------------------------------------------------------------------------
	//	struct Shadowmap::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Shadowmap::Impl {
		static float m_LightHeight;	//ライトの高さ（向きをこの値で掛ける）
		static float m_LightNear;	//ライトのNear
		static float m_LightFar;		//ライトのFar
		static float m_ViewWidth;
		static float m_ViewHeight;

		//メッシュリソース
		weak_ptr<MeshResource> m_MeshResource;
		//マルチメッシュリソース
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
	//	用途: シャドウマップコンポーネント（前処理用）
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
					L"メッシュリソースが見つかりません",
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
					L"メッシュキーが空白です",
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
					L"メッシュリソースが見つかりません",
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
					L"メッシュキーが空白です",
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
		//コンスタントバッファ付ルートシグネチャ
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateCbv();
		auto Dev = App::GetApp()->GetDeviceResources();
		//デスクプリタヒープ作成
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
				= Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//CbvSrvデスクプリタヒープ(コンスタントバッファのみ)
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);
			//GPU側デスクプリタヒープのハンドルの配列の作成
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.clear();
			CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			pImpl->m_Dx12DrawResources.m_GPUDescriptorHandleVec.push_back(CbvHandle);
		}
		///コンスタントバッファ作成
		{
			//コンスタントバッファは256バイトにアラインメント
			UINT ConstBuffSize = (sizeof(ShadowConstants) + 255) & ~255;
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"コンスタントバッファ用のアップロードヒープ作成に失敗しました",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"Shadowmap::Impl::CreateConstantBuffer()"
			);
			//コンスタントバッファのビューを作成
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			//コンスタントバッファは256バイトにアラインメント
			cbvDesc.SizeInBytes = ConstBuffSize;
			//コンスタントバッファビューを作成すべきデスクプリタヒープ上のハンドルを取得
			//シェーダリソースがある場合コンスタントバッファはシェーダリソースビューのあとに設置する
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//コンスタントバッファのアップロードヒープのマップ
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"コンスタントバッファのマップに失敗しました",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"Shadowmap::CreateNotShadow()"
			);
		}
		///パイプラインステート作成
		{
			pImpl->m_Dx12DrawResources.m_PipelineState 
				= PipelineState::CreateShadowmap3D<VertexPositionNormalTexture, VSShadowmap>(pImpl->m_Dx12DrawResources.m_RootSignature, pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}
		///コマンドリスト作成
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_PipelineState);
			CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		}
	}

	void Shadowmap::UpdateConstantBuffer() {
		//更新
		memcpy(pImpl->m_Dx12DrawResources.m_pConstantBuffer, reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer),
			sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer));
	}

	void Shadowmap::DrawObject() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto ShMesh = GetMeshResource(false);
		if (!ShMesh) {
			return;
		}
		//コマンドリストのリセット
		CommandList::Reset(pImpl->m_Dx12DrawResources.m_PipelineState, pImpl->m_Dx12DrawResources.m_CommandList);
		//メッシュが更新されていればリソース更新
		ShMesh->UpdateResources<VertexPositionNormalTexture>(pImpl->m_Dx12DrawResources.m_CommandList);
		//ルートシグネチャのセット
		pImpl->m_Dx12DrawResources.m_CommandList->SetGraphicsRootSignature(pImpl->m_Dx12DrawResources.m_RootSignature.Get());
		//デスクプリタヒープのセット
		ID3D12DescriptorHeap* ppHeaps[] = { pImpl->m_Dx12DrawResources.m_DescriptorHeap.Get() };
		pImpl->m_Dx12DrawResources.m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		//GPUデスクプリタヒープハンドルのセット
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

		//デプスステンシルビューのハンドルを取得
		auto SMRenderTarget = Dev->GetShadowMapRenderTarget();
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = SMRenderTarget->GetDsvHandle();
		//取得したハンドルをセット
		pImpl->m_Dx12DrawResources.m_CommandList->OMSetRenderTargets(0, nullptr, FALSE, &dsvHandle);

		pImpl->m_Dx12DrawResources.m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pImpl->m_Dx12DrawResources.m_CommandList->IASetVertexBuffers(0, 1, &ShMesh->GetVertexBufferView());
		//インデックスバッファをセット
		pImpl->m_Dx12DrawResources.m_CommandList->IASetIndexBuffer(&ShMesh->GetIndexBufferView());
		//インデックス描画
		pImpl->m_Dx12DrawResources.m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		//コマンドリストのクローズ
		CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		//デバイスにコマンドリストを送る
		Dev->InsertDrawCommandLists(pImpl->m_Dx12DrawResources.m_CommandList.Get());
	}



	void Shadowmap::OnDraw() {
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage) {
			return;
		}
		//メッシュリソースの取得
		auto PtrMeshResource = GetMeshResource();

		//行列の定義
		bsm::Mat4x4 World, LightView, LightProj;
		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//ワールド行列の決定
		World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		//ビュー行列の決定
		auto StageView = PtrStage->GetView();
		//ライトの取得
		auto StageLight = PtrGameObject->OnGetDrawLight();
		//位置の取得
		auto Pos = PtrTrans->GetWorldMatrix().transInMatrix();
		bsm::Vec3 PosSpan = StageLight.m_Directional;
		PosSpan *= 0.1f;
		Pos += PosSpan;
		//ワールド行列の決定
		World.affineTransformation(
			PtrTrans->GetScale(),			//スケーリング
			PtrTrans->GetPivot(),		//回転の中心（重心）
			PtrTrans->GetQuaternion(),				//回転角度
			Pos				//位置
		);
		bsm::Mat4x4 RealWorldMatrix;
		//ワールド行列の決定
//		if (data.m_UseMeshToTransformMatrix) {
//			RealWorldMatrix = data.m_MeshToTransformMatrix * GetMeshToTransformMatrix();
//			RealWorldMatrix *= World;
//		}
//		else {
			RealWorldMatrix = GetMeshToTransformMatrix() * World;
//		}
		//ビュー行列の決定
//		auto StageView = PtrStage->GetView();
		bsm::Vec3 LightDir = -1.0 * StageLight.m_Directional;
		bsm::Vec3 LightAt = StageView->GetTargetCamera()->GetAt();
		bsm::Vec3 LightEye = LightAt + (LightDir * GetLightHeight());
		//ライトのビューと射影を計算
		LightView.lookatLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
		LightProj.orthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());

		pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer.mWorld = bsm::transpose(RealWorldMatrix);
		pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer.mView = bsm::transpose(LightView);
		pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer.mProj = bsm::transpose(LightProj);

		UpdateConstantBuffer();
		DrawObject();

	}


	//static変数の実体
	vector<bsm::Vec3> DrawObjectBase::m_TempPositions;


	//--------------------------------------------------------------------------------------
	///	Simple描画に使用する構造体(影対応)
	//--------------------------------------------------------------------------------------
	struct SmDrawObject :public DrawObjectBase {
		//頂点変更する場合のメッシュ（オリジナル）
		shared_ptr<MeshResource> m_OriginalMeshResource;
		//オリジナルメッシュを使うかどうか
		bool m_UseOriginalMeshResource;
		///テクスチャリソース
		weak_ptr<TextureResource> m_TextureResource;
		///エミッシブ色
		bsm::Col4 m_Emissive;
		/// デフューズ色
		bsm::Col4 m_Diffuse;
		/// スペキュラー色
		bsm::Col4 m_Specular;
		///影を投影するかどうか
		bool m_OwnShadowActive;
		//モデルに入っているDiffuseを使うかどうか
		bool m_ModelDiffusePriority;
		//モデルに入っているEmissiveを使うかどうか
		bool m_ModelEmissivePriority;
		//モデルに入っているテクスチャを使うかどうか
		bool m_ModelTextureEnabled;
		///Instance描画用
		///Instance最大値
		size_t m_MaxInstance;
		/// 行列用のメッシュ
		shared_ptr<MeshResource> m_InstanceMatrixMesh;
		///行列の配列
		vector<bsm::Mat4x4> m_MatrixVec;
		//行列を自動クリアするかどうか
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
	///	Simple描画オブジェクトの親
	//--------------------------------------------------------------------------------------
	SmBaseDraw::SmBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		//パイプラインステートをデフォルトの３D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	SmBaseDraw::~SmBaseDraw() {}

	void SmBaseDraw::SetConstants(Dx12Constants<SimpleConstants>& SmCb) {
		//行列の定義
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//行列の定義
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//ワールド行列の決定
//		if (data.m_UseMeshToTransformMatrix) {
//			World = data.m_MeshToTransformMatrix * GetMeshToTransformMatrix();
//			World *= PtrTrans->GetWorldMatrix();
//		}
//		else {
			World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
//		}
		//転置する
		World.transpose();
		//カメラを得る
		auto CameraPtr = GetGameObject()->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		SmCb.m_ConstantBuffer.World = World;
		SmCb.m_ConstantBuffer.View = ViewMat;
		SmCb.m_ConstantBuffer.Projection = ProjMat;
		//エミッシブ
		SmCb.m_ConstantBuffer.Emissive = GetEmissive();
		//デフィーズ
		SmCb.m_ConstantBuffer.Diffuse = GetDiffuse();
		//スペキュラー
		SmCb.m_ConstantBuffer.Specular = GetSpecular();
		//ライティング
		auto StageLight = GetGameObject()->OnGetDrawLight();
		SmCb.m_ConstantBuffer.LightDir = StageLight.m_Directional;
		SmCb.m_ConstantBuffer.LightDir.w = 1.0f;
		SmCb.m_ConstantBuffer.EyePos = CameraPtr->GetEye();
		SmCb.m_ConstantBuffer.EyePos.w = 1.0f;
		//影用
		if (GetOwnShadowActive()) {
			bsm::Vec3 CalcLightDir = -1.0 * StageLight.m_Directional;
			bsm::Vec3 LightAt = CameraPtr->GetAt();
			bsm::Vec3 LightEye = CalcLightDir;
			LightEye *= Shadowmap::GetLightHeight();
			LightEye = LightAt + LightEye;
			SmCb.m_ConstantBuffer.LightPos = LightEye;
			SmCb.m_ConstantBuffer.LightPos.w = 1.0f;
			bsm::Mat4x4 LightView, LightProj;
			//ライトのビューと射影を計算
			LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
				Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
			SmCb.m_ConstantBuffer.LightView = bsm::transpose(LightView);
			SmCb.m_ConstantBuffer.LightProjection = bsm::transpose(LightProj);
		}
		//ボーンの設定
/*
		size_t BoneSz = pImpl->m_SmDrawObject.m_LocalBonesMatrix.size();
		if (BoneSz > 0) {
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				bsm::Mat4x4 mat = pImpl->m_SmDrawObject.m_LocalBonesMatrix[b];
				mat.transpose();
				SmCb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
				SmCb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				SmCb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}
		}
		else if (pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix.size() > data.m_MultiMeshIndex) {
			//マルチメッシュのボーンがあった
			//ボーンの設定
			BoneSz = pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix[data.m_MultiMeshIndex].size();
			if (BoneSz > 0) {
				UINT cb_count = 0;
				for (size_t b = 0; b < BoneSz; b++) {
					bsm::Mat4x4 mat = pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix[data.m_MultiMeshIndex][b];
					mat.transpose();
					SmCb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
					SmCb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
					SmCb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
					cb_count += 3;
				}
			}
		}
*/
	}

	//行列バッファの作成
	void SmBaseDraw::CreateMatrixBuffer() {
/*
		//インスタンス行列バッファの作成
		//Max値で作成する
		vector<bsm::Mat4x4> matrices(pImpl->m_SmDrawObject.m_MaxInstance);
		for (auto& m : matrices) {
			m = bsm::Mat4x4();
		}
		MeshResource::CreateDynamicVertexBuffer(pImpl->m_SmDrawObject.m_MatrixBuffer, matrices);
*/
	}

	//行列バッファのマップ
	void SmBaseDraw::MapMatrixBuffer() {
/*
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//インスタンスバッファにマップ
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//行列のマップ
		if (FAILED(pID3D11DeviceContext->Map(pImpl->m_SmDrawObject.m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"行列のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"SmBaseDraw::MapMatrixBuffer()"
			);
		}
		//行列の変更
		auto* matrices = (bsm::Mat4x4*)mappedBuffer.pData;
		bsm::Mat4x4 World;
		for (size_t i = 0; i < pImpl->m_SmDrawObject.m_MatrixVec.size(); i++) {
			//ワールド行列
			World = pImpl->m_SmDrawObject.m_MatrixVec[i];
			//転置する
			World.transpose();
			matrices[i] = World;
		}
		//アンマップ
		pID3D11DeviceContext->Unmap(pImpl->m_SmDrawObject.m_MatrixBuffer.Get(), 0);
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
		//テクスチャがなければnullを返す
		auto shptr = pImpl->m_SmDrawObject.m_TextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;

	}

	shared_ptr<MultiMeshResource> SmBaseDraw::GetMultiMeshResource() const {
		//なければnullを返す
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
				L"インデックスが上限を超えてます",
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
				L"インスタンス上限を超えてます",
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
		auto ReshRes = GetMeshResource();
		if (!ReshRes) {
			throw BaseException(
				L"メッシュリソースがありません",
				L"if (!ReshRes)",
				L"SmBaseDraw::GetStaticMeshLocalPositions()"
			);
		}
		vertices.clear();
		ReshRes->GetLocalPositions(vertices);
*/
	}

	void SmBaseDraw::GetStaticMeshWorldPositions(vector<bsm::Vec3>& vertices) {
		GetStaticMeshLocalPositions(vertices);
		//ワールド行列の反映
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
		if (GetVecLocalBones().size() == 0) {
			throw BaseException(
				L"ボーン行列がありません",
				L"if (GetVecLocalBones().size() == 0)",
				L"SmBaseDraw::GetSkinedMeshLocalPositions()"
			);
		}
		auto ReshRes = GetMeshResource();
		if (!ReshRes) {
			throw BaseException(
				L"メッシュリソースがありません",
				L"if (!ReshRes)",
				L"SmBaseDraw::GetSkinedMeshLocalPositions()"
			);
		}
		vertices.clear();
		auto& Bones = GetVecLocalBones();
		auto& PosVec = ReshRes->GetVerteces();
		auto& SkinVec = ReshRes->GetSkins();
		for (auto& v : PosVec) {
			vertices.push_back(v.position);
		}
		//スキニング処理
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

	void SmBaseDraw::GetSkinedMeshWorldPositions(vector<bsm::Vec3>& vertices) {
		GetSkinedMeshLocalPositions(vertices);
		//ワールド行列の反映
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



	///シェーダーリソースビュー（テクスチャ）作成
	void SmBaseDraw::CreateShaderResourceView() {
		auto ShPtr = GetTextureResource();
		if (!ShPtr) {
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		//テクスチャハンドルを作成
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//テクスチャのシェーダリソースビューを作成
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//フォーマット
		srvDesc.Format = ShPtr->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = ShPtr->GetTextureResDesc().MipLevels;
		//シェーダリソースビュー
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
		//ブレンドステートとラスタライザ差し替え
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


	///描画処理
	void SmBaseDraw::DrawObject() {
		auto ShMesh = GetMeshResource();
		auto ShTex = GetTextureResource();
		if (!ShMesh) {
			return;
		}
		//透明処理
		RefreshTrace();
		//コマンドリストのリセット
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
		//描画
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

		//コマンドリストのクローズ
		CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		//デバイスにコマンドリストを送る
		Dev->InsertDrawCommandLists(pImpl->m_Dx12DrawResources.m_CommandList.Get());
	}

	void SmBaseDraw::CreatePNNotShadow() {
		//未実装
		

	}


	void SmBaseDraw::CreatePNTNotShadow() {
		//ルートシグネチャ
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrvSmpCbv();
		//デスクプリタヒープ
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//デスクプリタヒープ
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
			//サンプラーデスクプリタヒープ
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
			//GPU側デスクプリタヒープのハンドルの配列の作成
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
		//サンプラー
		{
			auto SamplerDescriptorHandle = pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
		}
		//コンスタントバッファ
		{
			//コンスタントバッファは256バイトにアラインメント
			UINT ConstBuffSize = (sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;
			//コンスタントバッファリソース（アップロードヒープ）の作成
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"コンスタントバッファ用のアップロードヒープ作成に失敗しました",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"PNTStaticDraw::CreateNotShadow()"
			);
			//コンスタントバッファのビューを作成
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;
			//コンスタントバッファビューを作成すべきデスクプリタヒープ上のハンドルを取得
			//シェーダリソースがある場合コンスタントバッファはシェーダリソースビューのあとに設置する
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);

			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//コンスタントバッファのアップロードヒープのマップ
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"コンスタントバッファのマップに失敗しました",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"PNTStaticDraw::CreateNotShadow()"
			);

		}
		//シェーダリソースビューはテクスチャセット時に作成
		//パイプラインステートの作成
		{
			PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(pImpl->m_Dx12DrawResources.m_RootSignature, pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}
		//コマンドリストは裏面カリングに初期化
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_CullBackPipelineState);
			//コンスタントバッファ更新
			UpdateConstantBuffer();
			CommandList::Close(pImpl->m_Dx12DrawResources.m_CommandList);
		}
	}


	void SmBaseDraw::CreatePNTWithShadow() {
		//ルートシグネチャ
		pImpl->m_Dx12DrawResources.m_RootSignature = RootSignature::CreateSrv2Smp2Cbv();
		//デスクプリタヒープ
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHandleIncrementSize =
				Dev->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

			//デスクプリタヒープ
			pImpl->m_Dx12DrawResources.m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 2);
			//サンプラーデスクプリタヒープ
			pImpl->m_Dx12DrawResources.m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(2);
			//GPU側デスクプリタヒープのハンドルの配列の作成
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
		//サンプラー
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
		//コンスタントバッファ
		{
			//コンスタントバッファは256バイトにアラインメント
			UINT ConstBuffSize = (sizeof(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBuffer) + 255) & ~255;
			//コンスタントバッファリソース（アップロードヒープ）の作成
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap)),
				L"コンスタントバッファ用のアップロードヒープ作成に失敗しました",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"PNTStaticDraw::CreateWithShadow()"
			);
			//コンスタントバッファのビューを作成
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;
			//コンスタントバッファビューを作成すべきデスクプリタヒープ上のハンドルを取得
			//シェーダリソースがある場合コンスタントバッファはシェーダリソースビューのあとに設置する
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				pImpl->m_Dx12DrawResources.m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				2,
				pImpl->m_Dx12DrawResources.m_DescriptorHandleIncrementSize
			);

			Dev->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//コンスタントバッファのアップロードヒープのマップ
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(pImpl->m_Dx12DrawResources.m_Dx12Constants.m_ConstantBufferUploadHeap->Map(0, &readRange,
				reinterpret_cast<void**>(&pImpl->m_Dx12DrawResources.m_pConstantBuffer)),
				L"コンスタントバッファのマップに失敗しました",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"PNTStaticDraw::CreateNotShadow()"
			);
		}
		//シェーダリソースビューはテクスチャセット時に作成
		//パイプラインステートの作成
		{
			//シャドウ付きパイプラインステートの作成
			PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStaticShadow, PSPNTStaticShadow>(pImpl->m_Dx12DrawResources.m_RootSignature,
				pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			pImpl->m_Dx12DrawResources.m_CullFrontPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
			pImpl->m_Dx12DrawResources.m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			pImpl->m_Dx12DrawResources.m_CullBackPipelineState = PipelineState::CreateDirect(pImpl->m_Dx12DrawResources.m_PineLineDesc);
		}
		//コマンドリストは裏面カリングに初期化
		{
			pImpl->m_Dx12DrawResources.m_CommandList = CommandList::CreateDefault(pImpl->m_Dx12DrawResources.m_CullBackPipelineState);
			//コンスタントバッファ更新
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
		//更新
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
	///	PNStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNStaticDraw::PNStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{
	}

	PNStaticDraw::~PNStaticDraw() {}

	void PNStaticDraw::OnCreate() {
		//ライティングのみだと極端になるので調整
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
		//PN影無しに初期化
		CreatePNNotShadow();
	}

	void PNStaticDraw::OnDraw() {
		DrawPN();
	}

	//--------------------------------------------------------------------------------------
	///	PCTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PCTStaticDraw::PCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PCTStaticDraw::~PCTStaticDraw() {}

	void PCTStaticDraw::OnCreate() {}

	void PCTStaticDraw::OnDraw() {

	}


	//--------------------------------------------------------------------------------------
	///	PNTStatic描画コンポーネント
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{
	}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::SetOwnShadowActive(bool b) {
		if (IsOwnShadowActive()) {
			//今は影付き
			if (!b) {
				//PNT影無しに変更
				CreatePNTNotShadow();
				SmBaseDraw::SetOwnShadowActive(false);
			}
		}
		else {
			//今は影無し
			if (b) {
				//PNT影付きに変更
				CreatePNTWithShadow();
				SmBaseDraw::SetOwnShadowActive(true);
			}
		}
	}


	void PNTStaticDraw::OnCreate() {
		//ライティングのみだと極端になるので調整
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
		//PNT影無しに初期化
		CreatePNTNotShadow();
	}

	void PNTStaticDraw::OnDraw() {
		DrawPNT();
	}

}
//end basecross
