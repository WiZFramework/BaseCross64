/*!
@file DeviceResources.cpp
@brief テクスチャ、メッシュ、Dx12デバイスリソース実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	struct D3D11CUSTAM_SUBRESOURCE_DATA
	{
		const void *pSysMem;
		UINT SysMemPitch;
		UINT SysMemSlicePitch;
	};

	//--------------------------------------------------------------------------------------
	//	struct TextureResource::Impl;
	//--------------------------------------------------------------------------------------
	struct TextureResource::Impl {
		//テクスチャ用のデスクプリタ
		D3D12_RESOURCE_DESC m_TextureResDesc;
		UINT m_TexturePixelSize;
		//リソース
		ComPtr<ID3D12Resource> m_Texture;
		//GPUアップロードのためのリソース
		ComPtr<ID3D12Resource> m_TextureUploadHeap;
		//テクスチャファイルへのパス
		wstring m_FileName{ L"" };
		//テクスチャデータ
		vector<uint8_t> m_Data;
		bool m_DataRefresh;
		Impl() :m_TexturePixelSize(4), m_DataRefresh(true) {}
		//テクスチャ作成
		void CreateTexture(uint32_t Width, uint32_t Height) {
			m_TexturePixelSize = 4;
			::ZeroMemory(&m_TextureResDesc, sizeof(D3D12_RESOURCE_DESC));
			m_TextureResDesc.MipLevels = 1;
			m_TextureResDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			m_TextureResDesc.Width = Width;
			m_TextureResDesc.Height = Height;
			m_TextureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			m_TextureResDesc.DepthOrArraySize = 1;
			m_TextureResDesc.SampleDesc.Count = 1;
			m_TextureResDesc.SampleDesc.Quality = 0;
			m_TextureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			auto Dev = App::GetApp()->GetDeviceResources();
			ThrowIfFailed(
				Dev->GetD3DDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE, &m_TextureResDesc,
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&m_Texture)),
				L"テクスチャリソースの作成に失敗しました",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"TextureResource::Impl::CreateTexture()"
			);
		}

		void CreateTexture(const Image* srcImages, size_t nimages, const DirectX::TexMetadata& metadata) {
			if (metadata.IsVolumemap()) {
				ThrowBaseException(
					L"この形式には対応してません",
					L"if (metadata.IsVolumemap())",
					L"TextureResource::Impl::CreateTexture()"
				);
			}

			std::unique_ptr<D3D11CUSTAM_SUBRESOURCE_DATA[]> initData(new (std::nothrow) D3D11CUSTAM_SUBRESOURCE_DATA[metadata.mipLevels * metadata.arraySize]);
			if (!initData) {
				ThrowBaseException(
					L"テクスチャデータの領域取得に失敗しました",
					L"if (!initData)",
					L"TextureResource::Impl::CreateTexture()"
				);

			}

			//--- 1D or 2D texture case ---------------------------------------------------
			size_t idx = 0;
			for (size_t item = 0; item < metadata.arraySize; ++item)
			{
				for (size_t level = 0; level < metadata.mipLevels; ++level)
				{
					size_t index = metadata.ComputeIndex(level, item, 0);
					if (index >= nimages) {
						ThrowBaseException(
							L"データのインデックスが範囲外です",
							L"if (index >= nimages)",
							L"TextureResource::Impl::CreateTexture()"
						);

					}
					const Image& img = srcImages[index];

					if (img.format != metadata.format) {
						ThrowBaseException(
							L"データのフォーマットが違います",
							L"if (img.format != metadata.format)",
							L"TextureResource::Impl::CreateTexture()"
						);
					}
					if (!img.pixels) {
						ThrowBaseException(
							L"データのポインタが不正です",
							L"if (!img.pixels)",
							L"TextureResource::Impl::CreateTexture()"
						);
					}
					if (idx >= (metadata.mipLevels * metadata.arraySize)) {
						ThrowBaseException(
							L"データの並びが不定です",
							L"if (idx >= (metadata.mipLevels * metadata.arraySize))",
							L"TextureResource::Impl::CreateTexture()"
						);
					}
					initData[idx].pSysMem = img.pixels;
					initData[idx].SysMemPitch = static_cast<DWORD>(img.rowPitch);
					initData[idx].SysMemSlicePitch = static_cast<DWORD>(img.slicePitch);
					++idx;
				}
			}

			DXGI_FORMAT tformat = metadata.format;

			switch (metadata.dimension)
			{
			case TEX_DIMENSION_TEXTURE1D:
			{
				::ZeroMemory(&m_TextureResDesc, sizeof(D3D12_RESOURCE_DESC));
				m_TextureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
				m_TextureResDesc.Format = tformat;
				m_TextureResDesc.Width = static_cast<UINT>(metadata.width);
				m_TextureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
				m_TextureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.depth);
				m_TextureResDesc.SampleDesc.Count = 1;
				m_TextureResDesc.SampleDesc.Quality = 0;
				m_TextureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
			}
			break;

			case TEX_DIMENSION_TEXTURE2D:
			{

				::ZeroMemory(&m_TextureResDesc, sizeof(D3D12_RESOURCE_DESC));
				m_TextureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
				m_TextureResDesc.Format = tformat;
				m_TextureResDesc.Width = static_cast<UINT>(metadata.width);
				m_TextureResDesc.Height = static_cast<UINT>(metadata.height);
				m_TextureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
				if (metadata.IsCubemap()) {
					m_TextureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
				}
				else {
					m_TextureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.depth);
				}
				m_TextureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
				m_TextureResDesc.SampleDesc.Count = 1;
				m_TextureResDesc.SampleDesc.Quality = 0;
				m_TextureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			}
			break;

			case TEX_DIMENSION_TEXTURE3D:
			{
				::ZeroMemory(&m_TextureResDesc, sizeof(D3D12_RESOURCE_DESC));
				m_TextureResDesc.MipLevels = static_cast<UINT16>(metadata.mipLevels);
				m_TextureResDesc.Format = tformat;
				m_TextureResDesc.Width = static_cast<UINT>(metadata.width);
				m_TextureResDesc.Height = static_cast<UINT>(metadata.height);
				m_TextureResDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
				m_TextureResDesc.DepthOrArraySize = static_cast<UINT16>(metadata.arraySize);
				m_TextureResDesc.SampleDesc.Count = 1;
				m_TextureResDesc.SampleDesc.Quality = 0;
				m_TextureResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			}
			break;
			}

			m_TexturePixelSize = initData[0].SysMemPitch / (UINT)m_TextureResDesc.Width;
			auto Dev = App::GetApp()->GetDeviceResources();
			ThrowIfFailed(
				Dev->GetD3DDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE, &m_TextureResDesc,
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&m_Texture)),
				L"テクスチャリソースの作成に失敗しました",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"TextureResource::Impl::CreateTexture()"
			);
			//
			m_Data.clear();
			uint8_t* ptr = (uint8_t*)initData[0].pSysMem;
			unsigned int rowPitch = (unsigned int)m_TextureResDesc.Width * m_TexturePixelSize;
			m_Data.resize(rowPitch * (unsigned int)m_TextureResDesc.Height);
			for (uint32_t i = 0; i < m_Data.size(); i++) {
				m_Data[i] = (uint8_t)ptr[i];
			}
			m_DataRefresh = true;


		}

		//データのコピー
		void CopyData(const vector<uint8_t>& data) {
			m_Data.clear();
			unsigned int rowPitch = (unsigned int)m_TextureResDesc.Width * m_TexturePixelSize;
			m_Data.resize(rowPitch * (unsigned int)m_TextureResDesc.Height);
			if (m_Data.size() > data.size()) {
				ThrowBaseException(
					L"渡されたデータのサイズが、縦横の数に足りません",
					L"if (pImpl->m_Data.size() > data.size())",
					L"TextureResource::Impl::CopyData()"
				);
			}
			for (uint32_t i = 0; i < m_Data.size(); i++) {
				m_Data[i] = data[i];
			}
			m_DataRefresh = true;
		}

		//GPUアップロードバッファの作成.
		void CreateTextureUploadHeap() {
			auto Dev = App::GetApp()->GetDeviceResources();
			const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_Texture.Get(), 0, 1);

			ThrowIfFailed(
				Dev->GetD3DDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&m_TextureUploadHeap)),
				L"GPUアップロードバッファの作成に失敗しました",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"TextureResource::Impl::CreateTextureUploadHeap()"
			);
		}

	};

	//--------------------------------------------------------------------------------------
	// class TextureResource : public BaseResource
	//--------------------------------------------------------------------------------------
	TextureResource::TextureResource() :
		BaseResource(),
		pImpl(new Impl())
	{}
	TextureResource::~TextureResource() {}

	void TextureResource::OnCreateWithParam(uint32_t Width, uint32_t Height, const vector<uint8_t>& data) {
		pImpl->CreateTexture(Width, Height);
		pImpl->CopyData(data);
		pImpl->CreateTextureUploadHeap();
	}

	shared_ptr<TextureResource> TextureResource::CreateTextureResource(const wstring& FileName, const wstring& TexType) {
		return ObjectFactory::CreateWithParam<TextureResource>(FileName, TexType);
	}

	void TextureResource::OnCreateWithParam(const wstring& FileName, const wstring& TexType) {

		//ファイルでの構築
		try {

			pImpl->m_FileName = FileName;
			if (FileName == L"") {
				ThrowBaseException(
					L"ファイルが指定されていません",
					L"if(FileName == L\"\")",
					L"TextureResource::OnCreateWithParam()"
				);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(FileName.c_str());
			if (RetCode == -1) {
				wstring patherr = FileName;
				ThrowBaseException(
					L"ファイルが存在しません",
					patherr.c_str(),
					L"TextureResource::OnCreateWithParam()"
				);
			}
			//テクスチャ作成
			DirectX::TexMetadata metadata;
			DirectX::ScratchImage image;

			if (TexType == L"WIC") {
				//ファイル拡張子の調査
				wchar_t Drivebuff[_MAX_DRIVE];
				wchar_t Dirbuff[_MAX_DIR];
				wchar_t FileNamebuff[_MAX_FNAME];
				wchar_t Extbuff[_MAX_EXT];

				::ZeroMemory(Drivebuff, sizeof(Drivebuff));
				::ZeroMemory(Dirbuff, sizeof(Dirbuff));
				::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
				::ZeroMemory(Extbuff, sizeof(Extbuff));

				_wsplitpath_s(FileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);

				wstring ExtStr = Extbuff;

				if (ExtStr == L".dds" || ExtStr == L".DDS") {
					ThrowIfFailed(
						DirectX::LoadFromDDSFile(FileName.c_str(), 0, &metadata, image),
						L"テクスチャの読み込みに失敗しました",
						FileName,
						L"TextureResource::OnCreateWithParam()"
					);
				}
				else if (ExtStr == L".tga" || ExtStr == L".TGA") {
					ThrowIfFailed(
						DirectX::LoadFromTGAFile(FileName.c_str(), &metadata, image),
						L"テクスチャの読み込みに失敗しました",
						FileName,
						L"TextureResource::OnCreateWithParam()"
					);
				}
				else {
					ThrowIfFailed(
						DirectX::LoadFromWICFile(FileName.c_str(), 0, &metadata, image),
						L"テクスチャの読み込みに失敗しました",
						FileName,
						L"TextureResource::OnCreateWithParam()"
					);
				}
			}
			else if (TexType == L"DDS") {
				ThrowIfFailed(
					DirectX::LoadFromDDSFile(FileName.c_str(), 0, &metadata, image),
					L"テクスチャの読み込みに失敗しました",
					FileName,
					L"TextureResource::OnCreateWithParam()"
				);
			}
			else if (TexType == L"TGA") {
				ThrowIfFailed(
					DirectX::LoadFromTGAFile(FileName.c_str(), &metadata, image),
					L"テクスチャの読み込みに失敗しました",
					FileName,
					L"TextureResource::OnCreateWithParam()"
				);
			}
			else {
				ThrowBaseException(
					L"そのタイプは対応していません",
					TexType.c_str(),
					L"TextureResource::OnCreateWithParam()"
				);
			}

			pImpl->CreateTexture(image.GetImages(), image.GetImageCount(), metadata);
			pImpl->CreateTextureUploadHeap();

		}
		catch (...) {
			throw;
		}
	}


	shared_ptr<TextureResource> TextureResource::CreateTextureResource(uint32_t Width, uint32_t Height, const vector<uint8_t>& data) {
		return ObjectFactory::CreateWithParam<TextureResource>(Width, Height, data);
	}

	//リソース
	ComPtr<ID3D12Resource> TextureResource::GetTexture() const {
		return pImpl->m_Texture;

	}
	uint32_t TextureResource::GetTextureWidth() {
		return (uint32_t)pImpl->m_TextureResDesc.Width;

	}
	uint32_t TextureResource::GetTextureHeight() {
		return (uint32_t)pImpl->m_TextureResDesc.Height;
	}
	uint32_t TextureResource::GetPixelSize() {
		return pImpl->m_TexturePixelSize;
	}
	const D3D12_RESOURCE_DESC& TextureResource::GetTextureResDesc() const {
		return pImpl->m_TextureResDesc;
	}


	//テクスチャデータ
	vector<uint8_t>& TextureResource::GetData() const {
		return pImpl->m_Data;
	}
	void TextureResource::SetData(const vector<uint8_t>& data) {
		pImpl->CopyData(data);
	}
	void TextureResource::UpdateTexture(const vector<uint8_t>& data) {
		pImpl->CopyData(data);
	}


	//リソースの更新
	void TextureResource::UpdateResources(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
		if (!pImpl->m_DataRefresh) {
			return;
		}
		D3D12_SUBRESOURCE_DATA textureData = {};
		textureData.pData = &pImpl->m_Data[0];
		textureData.RowPitch = GetTextureWidth() * GetPixelSize();
		textureData.SlicePitch = textureData.RowPitch * GetTextureHeight();

		UpdateSubresources(commandList.Get(),
			GetTexture().Get(),
			pImpl->m_TextureUploadHeap.Get(),
			0, 0, 1, &textureData);

		commandList->ResourceBarrier(
			1,
			&CD3DX12_RESOURCE_BARRIER::Transition(
				GetTexture().Get(),
				D3D12_RESOURCE_STATE_COPY_DEST,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
			)
		);

		pImpl->m_DataRefresh = false;
	}



	//--------------------------------------------------------------------------------------
	//	struct DeviceResources::Impl;
	//	用途: Direct12デバイスイディオム
	//--------------------------------------------------------------------------------------
	struct DeviceResources::Impl {

		static const UINT FrameCount = 3;
		//パイプラインobjects.
		D3D12_VIEWPORT m_Viewport;
		D3D12_RECT m_ScissorRect;
		ComPtr<IDXGISwapChain3> m_SwapChain;
		ComPtr<ID3D12Device> m_Device;
		ComPtr<ID3D11On12Device> m_d3d11On12Device;
		ComPtr<ID3D12Resource> m_RenderTargets[FrameCount];
		ComPtr<ID3D11Resource> m_WrappedBackBuffers[FrameCount];
		ComPtr<ID2D1Bitmap1> m_d2dRenderTargets[FrameCount];
		ComPtr<ID3D12Resource> m_DepthStencil;
		
		ComPtr<ID3D12CommandAllocator> m_CommandAllocator[FrameCount];
		ComPtr<ID3D12CommandQueue> m_CommandQueue;
		//RenderTargerView Heap
		ComPtr<ID3D12DescriptorHeap> m_RtvHeap;
		//DepsStensilViewHeap
		ComPtr<ID3D12DescriptorHeap> m_DsvHeap;

		///シャドウマップのレンダリングターゲット
		shared_ptr<ShadowMapRenderTarget> m_ShadowMapRenderTarget;

		UINT m_RtvDescriptorSize;
		UINT m_DsvDescriptorSize;

		//クリア処理用のオブジェクト
		ComPtr<ID3D12RootSignature> m_RootSignature;
		ComPtr<ID3D12PipelineState> m_PipelineState;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;


		//汎用ルートシグネチャのマップ
		map<wstring, ComPtr<ID3D12RootSignature>> m_RootSignatureMap;
		//プレゼントバリア用のコマンドリスト
		ComPtr<ID3D12GraphicsCommandList> m_PresentCommandList;
		//コマンドリスト実行用の配列
		vector<ID3D12CommandList*> m_DrawCommandLists;

		// Dx11コンテキスト
		ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;

		// Direct2D 描画コンポーネント。
		ComPtr<ID2D1Factory3>		m_d2dFactory;
		ComPtr<ID2D1Device2>		m_d2dDevice;
		ComPtr<ID2D1DeviceContext2>	m_d2dDeviceContext;

		// DirectWrite 描画コンポーネント。
		ComPtr<IDWriteFactory>		m_dwriteFactory;
		ComPtr<IWICImagingFactory2>	m_wicFactory;


		//同期オブジェクト
		UINT m_FrameIndex;
		HANDLE m_FenceEvent;
		ComPtr<ID3D12Fence> m_Fence;
		UINT64 m_FenceValue;
		float m_dpi;
		float m_aspectRatio;
		//構築と破棄
		Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		~Impl();
		//リソースの構築
		void CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//アダプター取得
		void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
		//同期処理
		void WaitForPreviousFrame(bool ExceptionActive = true);
	};
	//構築
	DeviceResources::Impl::Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) :
		m_dpi(96.0f),
		m_Viewport(),
		m_ScissorRect()
	{
		try {
			//リソースの構築
			CreateDeviceResources(hWnd, isFullScreen, Width, Height);
		}
		catch (...) {
			throw;
		}
	}

	DeviceResources::Impl::~Impl() {
		WaitForPreviousFrame(false);
		CloseHandle(m_FenceEvent);
	}


	//アダプター取得ヘルパー関数
	_Use_decl_annotations_
		void DeviceResources::Impl::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
	{
		ComPtr<IDXGIAdapter1> adapter;
		*ppAdapter = nullptr;

		for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				// Don't select the Basic Render Driver adapter.
				// If you want a software adapter, pass in "/warp" on the command line.
				continue;
			}

			// Check to see if the adapter supports Direct3D 12, but don't create the
			// actual device yet.
			if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}

		*ppAdapter = adapter.Detach();
	}


	//リソースの構築
	void DeviceResources::Impl::CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) {

		m_Viewport.Width = static_cast<float>(Width);
		m_Viewport.Height = static_cast<float>(Height);
		m_Viewport.MaxDepth = 1.0f;

		m_ScissorRect.right = static_cast<LONG>(Width);
		m_ScissorRect.bottom = static_cast<LONG>(Height);


		m_aspectRatio = static_cast<float>(Width) / static_cast<float>(Height);

/*
		// Direct2D リソースを初期化します。

*/

		UINT dxgiFactoryFlags = 0;
		UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		D2D1_FACTORY_OPTIONS d2dFactoryOptions = {};

#if defined(_DEBUG)
		//D3D12 debug 有効.
		{
			ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();

				// Enable additional debug layers.
				dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
				d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
				d2dFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
			}
		}
#endif

/*
#if defined(_DEBUG)
		// Filter a debug error coming from the 11on12 layer.
		ComPtr<ID3D12InfoQueue> infoQueue;
		if (SUCCEEDED(m_Device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
		{
			// Suppress whole categories of messages.
			//D3D12_MESSAGE_CATEGORY categories[] = {};

			// Suppress messages based on their severity level.
			D3D12_MESSAGE_SEVERITY severities[] =
			{
				D3D12_MESSAGE_SEVERITY_INFO,
			};

			// Suppress individual messages by their ID.
			D3D12_MESSAGE_ID denyIds[] =
			{
				// This occurs when there are uninitialized descriptors in a descriptor table, even when a
				// shader does not access the missing descriptors.
				D3D12_MESSAGE_ID_INVALID_DESCRIPTOR_HANDLE,
			};

			D3D12_INFO_QUEUE_FILTER filter = {};
			//filter.DenyList.NumCategories = _countof(categories);
			//filter.DenyList.pCategoryList = categories;
			filter.DenyList.NumSeverities = _countof(severities);
			filter.DenyList.pSeverityList = severities;
			filter.DenyList.NumIDs = _countof(denyIds);
			filter.DenyList.pIDList = denyIds;

			ThrowIfFailed(infoQueue->PushStorageFilter(&filter),
				L"",
				L"infoQueue->PushStorageFilter(&filter)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
		}
#endif
*/


		//DXGIFactoryの作成
		ComPtr<IDXGIFactory4> factory;
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)),
			L"DXGIFactoryの作成に失敗しました",
			L"CreateDXGIFactory1(IID_PPV_ARGS(&factory)",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);

		//アダプタの取得
		{
			//ハードウェアアダプタの取得
			ComPtr<IDXGIAdapter1> hardwareAdapter;
			GetHardwareAdapter(factory.Get(), &hardwareAdapter);
			if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)))) {
				//失敗したらラップアダプタの取得
				ComPtr<IDXGIAdapter> warpAdapter;
				ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)),
					L"ラップアダプタの作成に失敗しました",
					L"factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter))",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

				ThrowIfFailed(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)),
					L"デバイスの作成に失敗しました",
					L"D3D12CreateDevice(warpAdapter.Get(),D3D_FEATURE_LEVEL_11_0,IID_PPV_ARGS(&m_Device))",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

			}
		}

		//コマンドキューの作成.
		{
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			ThrowIfFailed(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)),
				L"コマンドキューの作成に失敗しました",
				L"m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue))",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
		}

		//スワップチェーンの作成.
		{
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
			swapChainDesc.BufferCount = FrameCount;
			swapChainDesc.Width = Width;
			swapChainDesc.Height = Height;
			swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.SampleDesc.Count = 1;
			//swapChainDesc.OutputWindow = hWnd;
			//swapChainDesc.Windowed = TRUE;

			ComPtr<IDXGISwapChain1> swapChain;
			ThrowIfFailed(factory->CreateSwapChainForHwnd(
				m_CommandQueue.Get(),
				hWnd,
				&swapChainDesc,
				nullptr,
				nullptr, 
				&swapChain
			),
				L"スワップチェーンの作成に失敗しました",
				L"factory->CreateSwapChain(m_CommandQueue.Get(), &swapChainDesc, &swapChain)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);

			ThrowIfFailed(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER),
				L"Windowモードからフルスクリーンへの変更停止が設定できませんでした",
				L"factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);

			ThrowIfFailed(swapChain.As(&m_SwapChain),
				L"スワップチェーンのバージョン変更に失敗しました",
				L"swapChain.As(&m_SwapChain)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			//フレームインデックスを設定しておく
			m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();
		}
		//Dx11デバイスの作成
		{
			ComPtr<ID3D11Device> d3d11Device;
			ThrowIfFailed(D3D11On12CreateDevice(
				m_Device.Get(),
				d3d11DeviceFlags,
				nullptr,
				0,
				reinterpret_cast<IUnknown**>(m_CommandQueue.GetAddressOf()),
				1,
				0,
				&d3d11Device,
				&m_d3d11DeviceContext,
				nullptr
			),
				L"スワップチェーンのバージョン変更に失敗しました",
				L"swapChain.As(&m_SwapChain)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);

			ThrowIfFailed(d3d11Device.As(&m_d3d11On12Device),
				L"",
				L"d3d11Device.As(&m_d3d11On12Device)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
		}
		// Create D2D/DWrite components.
		{
			D2D1_DEVICE_CONTEXT_OPTIONS deviceOptions = D2D1_DEVICE_CONTEXT_OPTIONS_NONE;
			ThrowIfFailed(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &d2dFactoryOptions, &m_d2dFactory),
				L"",
				L"D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &d2dFactoryOptions, &m_d2dFactory)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);

			ComPtr<IDXGIDevice> dxgiDevice;
			ThrowIfFailed(m_d3d11On12Device.As(&dxgiDevice),
				L"",
				L"m_d3d11On12Device.As(&dxgiDevice)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);

			ThrowIfFailed(m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice),
				L"",
				L"m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);

			ThrowIfFailed(m_d2dDevice->CreateDeviceContext(deviceOptions, &m_d2dDeviceContext),
				L"",
				L"m_d2dDevice->CreateDeviceContext(deviceOptions, &m_d2dDeviceContext)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);

			ThrowIfFailed(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_dwriteFactory),
				L"",
				L"DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &m_dwriteFactory)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
		}
		// Query the desktop's dpi settings, which will be used to create
		 // D2D's render targets.
		float dpiX;
		float dpiY;
		m_d2dFactory->GetDesktopDpi(&dpiX, &dpiY);
		D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
			D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
			dpiX,
			dpiY
		);

		//デスクプリタヒープの作成.
		{
			//レンダーターゲットビューのデスクプリタヒープ作成
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = FrameCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ThrowIfFailed(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvHeap)),
				L"レンダーターゲットビューのデスクプリタヒープ作成に失敗しました",
				L"m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvHeap)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			// デプスステンシルビューのデスクプリタヒープ作成
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ThrowIfFailed(m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DsvHeap)),
				L"デプスステンシルビューのデスクプリタヒープ作成に失敗しました",
				L"m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DsvHeap)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			m_RtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		}

		//RTVのフレームリソースの作成
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RtvHeap->GetCPUDescriptorHandleForHeapStart());
			//FrameCount数だけRTVのフレームリソースの作成
			for (UINT n = 0; n < FrameCount; n++)
			{
				ThrowIfFailed(m_SwapChain->GetBuffer(n, IID_PPV_ARGS(&m_RenderTargets[n])),
					L"RTVのフレームリソースの作成に失敗しました",
					L"m_SwapChain->GetBuffer(n, IID_PPV_ARGS(&m_RenderTargets[n])",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);
				m_Device->CreateRenderTargetView(m_RenderTargets[n].Get(), nullptr, rtvHandle);
			
				// Create a wrapped 11On12 resource of this back buffer. Since we are 
				// rendering all D3D12 content first and then all D2D content, we specify 
				// the In resource state as RENDER_TARGET - because D3D12 will have last 
				// used it in this state - and the Out resource state as PRESENT. When 
				// ReleaseWrappedResources() is called on the 11On12 device, the resource
				D3D11_RESOURCE_FLAGS d3d11Flags = { D3D11_BIND_RENDER_TARGET };

				ThrowIfFailed(m_d3d11On12Device->CreateWrappedResource(
					m_RenderTargets[n].Get(),
					&d3d11Flags,
					D3D12_RESOURCE_STATE_RENDER_TARGET,
					D3D12_RESOURCE_STATE_PRESENT,
					IID_PPV_ARGS(&m_WrappedBackBuffers[n])
				),
					L"d3d11On12デバイスの作成に失敗しました",
					L"m_d3d11On12Device->CreateWrappedResource()",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

				// Create a render target for D2D to draw directly to this back buffer.
				ComPtr<IDXGISurface> surface;
				ThrowIfFailed(m_WrappedBackBuffers[n].As(&surface),
					L"WrappedBackBuffersD2DRenderTargetの作成に失敗しました",
					L"m_WrappedBackBuffers[n].As(&surface)",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

				ThrowIfFailed(m_d2dDeviceContext->CreateBitmapFromDxgiSurface(
					surface.Get(),
					&bitmapProperties,
					&m_d2dRenderTargets[n]
				),
					L"BitmapFromDxgiSurfaceの作成に失敗しました",
					L"m_d2dDeviceContext->CreateBitmapFromDxgiSurface()",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

				rtvHandle.Offset(1, m_RtvDescriptorSize);

				ThrowIfFailed(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator[n])),
					L"コマンドアロケータの作成に失敗しました",
					L"CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator[n])",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);
			}
		}

		//DSV(デプスステンシルビュー)
		{
			D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
			depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
			depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

			D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
			depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
			depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
			depthOptimizedClearValue.DepthStencil.Stencil = 0;

			ThrowIfFailed(m_Device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, Width, Height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				&depthOptimizedClearValue,
				IID_PPV_ARGS(&m_DepthStencil)
			),
				L"デプスステンシルリソース作成に失敗しました",
				L"m_Device->CreateCommittedResource)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);

			m_Device->CreateDepthStencilView(m_DepthStencil.Get(), &depthStencilDesc, m_DsvHeap->GetCPUDescriptorHandleForHeapStart());
		}

		{
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;

			ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error),
				L"空のルートシグネチャのシリアライズに失敗しました",
				L"D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			ThrowIfFailed(m_Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)),
				L"ルートシグネチャの作成に失敗しました",
				L"Dev->GetDevice()->CreateRootSignature)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
		}


		// Direct2D デバイス オブジェクトと、対応するコンテキストを作成します。

		/*


		*/


		//画面クリア用のコマンドリスト
		ThrowIfFailed(
			m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator[m_FrameIndex].Get(), nullptr, IID_PPV_ARGS(&m_CommandList)),
			L"コマンドリストの作成に失敗しました",
			L"m_Device->CreateCommandList()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(m_CommandList->Close(),
			L"コマンドリストのクローズに失敗しました",
			L"m_CommandList->Close()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);
		//プレゼント用のコマンドリスト
		ThrowIfFailed(
			m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator[m_FrameIndex].Get(), nullptr, IID_PPV_ARGS(&m_PresentCommandList)),
			L"コマンドリストの作成に失敗しました",
			L"m_Device->CreateCommandList()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(m_PresentCommandList->Close(),
			L"コマンドリストのクローズに失敗しました",
			L"m_CommandList->Close()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);
	}

	//同期処理
	void DeviceResources::Impl::WaitForPreviousFrame(bool ExceptionActive) {
		// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
		// This is code implemented as such for simplicity. More advanced samples 
		// illustrate how to use fences for efficient resource usage.

		// Signal and increment the fence value.
		const UINT64 fence = m_FenceValue;
		if (ExceptionActive) {
			ThrowIfFailed(m_CommandQueue->Signal(m_Fence.Get(), fence),
				L"コマンドキューのシグナルに失敗しました",
				L"m_CommandQueue->Signal(m_Fence.Get(), fence)",
				L"Dx12DeviceResources::Impl::WaitForPreviousFrame()"
			);
			m_FenceValue++;
			// Wait until the previous frame is finished.
			if (m_Fence->GetCompletedValue() < fence)
			{

				ThrowIfFailed(m_Fence->SetEventOnCompletion(fence, m_FenceEvent),
					L"イベントの設定に失敗しました",
					L"m_Fence->SetEventOnCompletion(fence, m_FenceEvent)",
					L"Dx12DeviceResources::Impl::WaitForPreviousFrame()"
				);
				WaitForSingleObject(m_FenceEvent, INFINITE);
			}
			m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();
		}
		else {
			//例外を投げない。デストラクタ用
			m_CommandQueue->Signal(m_Fence.Get(), fence);
			m_FenceValue++;
			if (m_Fence->GetCompletedValue() < fence)
			{
				m_Fence->SetEventOnCompletion(fence, m_FenceEvent);
				WaitForSingleObject(m_FenceEvent, INFINITE);
			}
			m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();
		}
	}

	//--------------------------------------------------------------------------------------
	//	class Dx12DeviceResources : public DeviceResources;
	//--------------------------------------------------------------------------------------

	shared_ptr<ShadowMapRenderTarget> DeviceResources::CreateShadowMapRenderTarget(float ShadowMapDimension) {
		//シャドウマップのレンダリングターゲットを作成
		pImpl->m_ShadowMapRenderTarget = make_shared<ShadowMapRenderTarget>(ShadowMapDimension);
		return pImpl->m_ShadowMapRenderTarget;
	}


	shared_ptr<ShadowMapRenderTarget> DeviceResources::GetShadowMapRenderTarget(float ShadowMapDimension) {
		if (!pImpl->m_ShadowMapRenderTarget) {
			ThrowBaseException(
				L"シャドウマップが作成されていません",
				L"if (!pImpl->m_ShadowMapRenderTarget)",
				L"DeviceResources::GetShadowMapRenderTarget()"
			);
		}
		return pImpl->m_ShadowMapRenderTarget;
	}


	void DeviceResources::ClearShadowmapViews() {
		auto ShadowTarget = GetShadowMapRenderTarget();
		ShadowTarget->ClearViews();
	}

	void DeviceResources::StartShadowmapDraw() {
		auto ShadowTarget = GetShadowMapRenderTarget();
		ShadowTarget->StartRenderTarget();
	}

	void DeviceResources::EndShadowmapDraw() {
		auto ShadowTarget = GetShadowMapRenderTarget();
		ShadowTarget->EndRenderTarget();
	}





	//通常描画のクリア
	void DeviceResources::ClearDefaultViews(const bsm::Col4& col) {
		ThrowIfFailed(pImpl->m_CommandAllocator[pImpl->m_FrameIndex]->Reset(),
			L"コマンドアロケータのリセットに失敗しました",
			L"m_CommandAllocator->Reset()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);

		ThrowIfFailed(pImpl->m_CommandList->Reset(pImpl->m_CommandAllocator[pImpl->m_FrameIndex].Get(), pImpl->m_PipelineState.Get()),
			L"コマンドリストのリセットに失敗しました",
			L"pImpl->m_CommandList->Reset()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);

		pImpl->m_CommandList->SetGraphicsRootSignature(pImpl->m_RootSignature.Get());
		pImpl->m_CommandList->RSSetViewports(1, &GetViewport());
		pImpl->m_CommandList->RSSetScissorRects(1, &GetScissorRect());
		// Indicate that the back buffer will be used as a render target.
		pImpl->m_CommandList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(pImpl->m_RenderTargets[pImpl->m_FrameIndex].Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(pImpl->m_RtvHeap->GetCPUDescriptorHandleForHeapStart(), pImpl->m_FrameIndex, pImpl->m_RtvDescriptorSize);
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(pImpl->m_DsvHeap->GetCPUDescriptorHandleForHeapStart());
		pImpl->m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		// Record commands.
		const float clearColor[] = { col.x, col.y, col.z, col.w };
		pImpl->m_CommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		pImpl->m_CommandList->ClearDepthStencilView(pImpl->m_DsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
		ThrowIfFailed(pImpl->m_CommandList->Close(),
			L"コマンドリストのクローズに失敗しました",
			L"m_CommandList->Close()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);
		InsertDrawCommandLists(pImpl->m_CommandList.Get());
	}

	void DeviceResources::InsertDrawCommandLists(ID3D12CommandList* Tgt) {

	//	pImpl->m_CommandQueue->ExecuteCommandLists(1, &Tgt);

		pImpl->m_DrawCommandLists.push_back(Tgt);
	}

	void DeviceResources::Present(unsigned int SyncInterval, unsigned int  Flags) {

		ThrowIfFailed(pImpl->m_PresentCommandList->Reset(pImpl->m_CommandAllocator[pImpl->m_FrameIndex].Get(), pImpl->m_PipelineState.Get()),
			L"コマンドリストのリセットに失敗しました",
			L"pImpl->m_CommandList->Reset()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);

		//プレゼント用のバリアを張る
		pImpl->m_PresentCommandList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(pImpl->m_RenderTargets[pImpl->m_FrameIndex].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		ThrowIfFailed(pImpl->m_PresentCommandList->Close(),
			L"コマンドリストのクローズに失敗しました",
			L"m_CommandList->Close()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);
		InsertDrawCommandLists(pImpl->m_PresentCommandList.Get());

		// Execute the command list.
		pImpl->m_CommandQueue->ExecuteCommandLists((UINT)pImpl->m_DrawCommandLists.size(), &pImpl->m_DrawCommandLists[0]);

		// ステートをクリアし，デフォルト状態にします.
		// TODO : ここに記述していいか確認要必要
		pImpl->m_d3d11DeviceContext->ClearState();
		pImpl->m_d3d11DeviceContext->Flush();

		// Present the frame.
		ThrowIfFailed(pImpl->m_SwapChain->Present(SyncInterval, Flags),
			L"スワップチェーンのプレゼントに失敗しました",
			L"pImpl->m_SwapChain->Present(1, 0)",
			L"Dx12DeviceResources::OnDraw()"
		);
		pImpl->WaitForPreviousFrame();
		pImpl->m_DrawCommandLists.clear();
	}

	void DeviceResources::ExecuteCommandLists() {
		// Execute the command list.
		if (pImpl->m_DrawCommandLists.size() > 0) {
			pImpl->m_CommandQueue->ExecuteCommandLists((UINT)pImpl->m_DrawCommandLists.size(), &pImpl->m_DrawCommandLists[0]);
			pImpl->m_DrawCommandLists.clear();
		}
	}

	void DeviceResources::WaitForPreviousFrame() {
		pImpl->WaitForPreviousFrame();
	}


	DeviceResources::DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) :
		pImpl(new Impl(hWnd, isFullScreen, Width, Height))
	{}

	DeviceResources::~DeviceResources() {
	}

	//コンテンツの作成後の処理
	void DeviceResources::AfterInitContents(bool ShadowActive) {
		if (ShadowActive) {
			CreateShadowMapRenderTarget(2048.0f);
		}
		//同期オブジェクトの作成と同期処理
		{

			ThrowIfFailed(pImpl->m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pImpl->m_Fence)),
				L"フェンスの作成に失敗しました",
				L"m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			pImpl->m_FenceValue = 1;

			//フレーム同期のためのイベントハンドルの作成
			pImpl->m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (pImpl->m_FenceEvent == nullptr)
			{
				ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()),
					L"ラストエラーの取得に失敗しました",
					L"HRESULT_FROM_WIN32(GetLastError())",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);
			}
			pImpl->WaitForPreviousFrame();
		}
	}


	UINT DeviceResources::GetFrameCount() const {
		return pImpl->FrameCount;
	}


	ComPtr<IDXGISwapChain3> DeviceResources::GetSwapChain() const {
		return pImpl->m_SwapChain;

	}
	ComPtr<ID3D12Device> DeviceResources::GetD3DDevice() const {
		return pImpl->m_Device;
	}

	ComPtr<ID3D11On12Device> DeviceResources::Get11On12Device() const {
		return pImpl->m_d3d11On12Device;
	}

	ComPtr<ID3D11DeviceContext> DeviceResources::GetD3D11DeviceContext() const
	{
		return pImpl->m_d3d11DeviceContext;
	}

	ComPtr<ID3D12Resource>* DeviceResources::GetRenderTargets() const {
		return pImpl->m_RenderTargets;

	}
	ComPtr<ID3D12Resource> DeviceResources::GetRenderTarget(UINT Index) const {
		if (Index >= pImpl->FrameCount) {
			ThrowBaseException(
				L"インデックスが上限を超えています",
				L"if (Index >= pImpl->FrameCount)",
				L"Dx12DeviceResources::GetRenderTarget()"
			);
		}
		return pImpl->m_RenderTargets[Index];

	}

	ComPtr<ID2D1Bitmap1> DeviceResources::Get2DRenderTargets(UINT Index) const
	{
		if (Index >= pImpl->FrameCount) {
			ThrowBaseException(
				L"インデックスが上限を超えています",
				L"if (Index >= pImpl->FrameCount)",
				L"Dx12DeviceResources::GetRenderTarget()"
			);
		}
		return pImpl->m_d2dRenderTargets[Index];
	}

	ComPtr<ID3D11Resource> DeviceResources::GetWrappedBackBuffer(UINT Index) const
	{
		if (Index >= pImpl->FrameCount) {
			ThrowBaseException(
				L"インデックスが上限を超えています",
				L"if (Index >= pImpl->FrameCount)",
				L"Dx12DeviceResources::GetRenderTarget()"
			);
		}
		return pImpl->m_WrappedBackBuffers[Index];

	}

	ComPtr<ID3D12CommandAllocator> DeviceResources::GetCommandAllocator() const {
		return pImpl->m_CommandAllocator[pImpl->FrameCount];
	}

	ComPtr<ID3D12CommandAllocator> DeviceResources::GetCommandAllocator(UINT Index) const {
		return pImpl->m_CommandAllocator[Index];
	}

	ComPtr<ID3D12CommandQueue> DeviceResources::GetCommandQueue() const {
		return pImpl->m_CommandQueue;

	}
	ComPtr<ID3D12DescriptorHeap> DeviceResources::GetRtvHeap() const {
		return pImpl->m_RtvHeap;
	}
	ComPtr<ID3D12DescriptorHeap> DeviceResources::GetDsvHeap() const {
		return pImpl->m_DsvHeap;
	}


	UINT DeviceResources::GetRtvDescriptorSize() const {
		return pImpl->m_RtvDescriptorSize;
	}
	const D3D12_VIEWPORT& DeviceResources::GetViewport() const {
		return pImpl->m_Viewport;
	}
	const D3D12_RECT& DeviceResources::GetScissorRect() const {
		return pImpl->m_ScissorRect;
	}

	UINT DeviceResources::GetFrameIndex() const {
		return pImpl->m_FrameIndex;

	}

	ComPtr<ID3D12RootSignature> DeviceResources::GetRootSignature(const wstring& Key) {
		auto it = pImpl->m_RootSignatureMap.find(Key);
		if (it == pImpl->m_RootSignatureMap.end()) {
			return nullptr;
		}
		return pImpl->m_RootSignatureMap[Key];
	}

	void  DeviceResources::SetRootSignature(const wstring& Key, const ComPtr<ID3D12RootSignature>& rootsig) {
		if (GetRootSignature(Key) != nullptr) {
			ThrowBaseException(
				L"すでにそのルートシグネチャは存在します",
				Key,
				L"DeviceResources::SetRootSignature()"
			);
		}
		pImpl->m_RootSignatureMap[Key] = rootsig;
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE DeviceResources::GetRtvHandle() const {
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			GetFrameIndex(),
			GetRtvDescriptorSize());
		return rtvHandle;
	}

	CD3DX12_CPU_DESCRIPTOR_HANDLE DeviceResources::GetDsvHandle() const {
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		return dsvHandle;
	}

	// D2D アクセサー。
	ID2D1Factory2*			DeviceResources::GetD2DFactory() const { return pImpl->m_d2dFactory.Get(); }
	ID2D1Device1*			DeviceResources::GetD2DDevice() const { return pImpl->m_d2dDevice.Get(); }
	ID2D1DeviceContext2*	DeviceResources::GetD2DDeviceContext() const { return pImpl->m_d2dDeviceContext.Get(); }
	IDWriteFactory*			DeviceResources::GetDWriteFactory() const { return pImpl->m_dwriteFactory.Get(); }
	IWICImagingFactory2*	DeviceResources::GetWicImagingFactory() const { return pImpl->m_wicFactory.Get(); }


	//--------------------------------------------------------------------------------------
	//	struct RenderTarget::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct RenderTarget::Impl {
		Impl() {
		}
	};

	//--------------------------------------------------------------------------------------
	//	class RenderTarget;
	//	用途: レンダリングターゲット
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RenderTarget::RenderTarget() :
		pImpl(new Impl())
	{}
	RenderTarget::~RenderTarget() {}


	//--------------------------------------------------------------------------------------
	//	struct ShadowMapRenderTarget::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct ShadowMapRenderTarget::Impl {

		//シャドウマップの大きさ
		const float m_ShadowMapDimension;
		//シャドウマップのデスクプリタヒープ
		ComPtr<ID3D12DescriptorHeap> m_ShadowmapDsvHeap;
		//シャドウマップのデプスステンシル
		ComPtr<ID3D12Resource> m_ShadowmapDepthStencil;
		//クリア用オブジェクト
		ComPtr<ID3D12RootSignature> m_RootSignature;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		//End用コマンドリスト
		ComPtr<ID3D12GraphicsCommandList> m_EndCommandList;

		Impl(float ShadowMapDimension) :
			m_ShadowMapDimension(ShadowMapDimension)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	シャドウマップのレンダリングターゲット
	//--------------------------------------------------------------------------------------
	ShadowMapRenderTarget::ShadowMapRenderTarget(float ShadowMapDimension):
		pImpl(new Impl(ShadowMapDimension))
	{
		try {
			//シャドウマップはcolは未使用
			auto Dev = App::GetApp()->GetDeviceResources();

			//シャドウマップ用デスクプリタヒープ
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ThrowIfFailed(Dev->GetD3DDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&pImpl->m_ShadowmapDsvHeap)),
				L"シャドウマップデプスステンシルビューのデスクプリタヒープ作成に失敗しました",
				L"Dev->GetDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_ShadowmapDsvHeap)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);

			D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
			depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
			depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
			depthOptimizedClearValue.DepthStencil.Stencil = 0;

			CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);

			CD3DX12_RESOURCE_DESC shadowTextureDesc(
				D3D12_RESOURCE_DIMENSION_TEXTURE2D,
				0,
				static_cast<UINT>(2048),
				static_cast<UINT>(2048),
				1,
				1,
				DXGI_FORMAT_R32_TYPELESS,
				1,
				0,
				D3D12_TEXTURE_LAYOUT_UNKNOWN,
				D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&shadowTextureDesc,
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				&depthOptimizedClearValue,
				IID_PPV_ARGS(&pImpl->m_ShadowmapDepthStencil)
			),
				L"シャドウマップデプスステンシルリソース作成に失敗しました",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);

			D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
			depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
			depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;



			//デプスステンシルビューの作成
			Dev->GetD3DDevice()->CreateDepthStencilView(pImpl->m_ShadowmapDepthStencil.Get(), &depthStencilViewDesc,
				pImpl->m_ShadowmapDsvHeap->GetCPUDescriptorHandleForHeapStart());

			//ルートシグネチャ
			{
				CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
				rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

				ComPtr<ID3DBlob> signature;
				ComPtr<ID3DBlob> error;

				ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error),
					L"空のルートシグネチャのシリアライズに失敗しました",
					L"D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error)",
					L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
				);
				ThrowIfFailed(Dev->GetD3DDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&pImpl->m_RootSignature)),
					L"ルートシグネチャの作成に失敗しました",
					L"Dev->GetDevice()->CreateRootSignature)",
					L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
				);
			}

			ComPtr<ID3D12PipelineState> PipelineState;
			//クリア用コマンドリスト
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				Dev->GetCommandAllocator(Dev->GetFrameIndex()).Get(),
				PipelineState.Get(),
				IID_PPV_ARGS(&pImpl->m_CommandList)),
				L"コマンドリストの作成に失敗しました",
				L"Dev->GetDevice()->CreateCommandList()",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);
			CommandList::Close(pImpl->m_CommandList);

			//End用コマンドリスト
			ThrowIfFailed(Dev->GetD3DDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				Dev->GetCommandAllocator(Dev->GetFrameIndex()).Get(),
				PipelineState.Get(),
				IID_PPV_ARGS(&pImpl->m_EndCommandList)),
				L"コマンドリストの作成に失敗しました",
				L"Dev->GetDevice()->CreateCommandList()",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);
			CommandList::Close(pImpl->m_EndCommandList);


			
		}
		catch (...) {
			throw;
		}
	}
	ShadowMapRenderTarget::~ShadowMapRenderTarget() {}

	float ShadowMapRenderTarget::GetShadowMapDimension() const {
		return pImpl->m_ShadowMapDimension;
	}

	//レンダリングターゲットをクリアする
	void ShadowMapRenderTarget::ClearViews(const bsm::Col4& col) {
		//シャドウマップはcolは未使用
		auto Dev = App::GetApp()->GetDeviceResources();
		CommandList::Reset(pImpl->m_CommandList);
		pImpl->m_CommandList->SetGraphicsRootSignature(pImpl->m_RootSignature.Get());

		pImpl->m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		pImpl->m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		// Record commands.
		pImpl->m_CommandList->ClearDepthStencilView(pImpl->m_ShadowmapDsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

		pImpl->m_CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			pImpl->m_ShadowmapDepthStencil.Get(), 
			D3D12_RESOURCE_STATE_DEPTH_WRITE, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

		CommandList::Close(pImpl->m_CommandList);
		Dev->InsertDrawCommandLists(pImpl->m_CommandList.Get());
	}
	//レンダリングターゲットを開始する
	void ShadowMapRenderTarget::StartRenderTarget() {}

	void ShadowMapRenderTarget::EndRenderTarget() {
		auto Dev = App::GetApp()->GetDeviceResources();
		CommandList::Reset(pImpl->m_EndCommandList);
		pImpl->m_EndCommandList->SetGraphicsRootSignature(pImpl->m_RootSignature.Get());

		pImpl->m_EndCommandList->RSSetViewports(1, &Dev->GetViewport());
		pImpl->m_EndCommandList->RSSetScissorRects(1, &Dev->GetScissorRect());

		pImpl->m_EndCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(
			pImpl->m_ShadowmapDepthStencil.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_DEPTH_WRITE));
		CommandList::Close(pImpl->m_EndCommandList);
		Dev->InsertDrawCommandLists(pImpl->m_EndCommandList.Get());
	}


	CD3DX12_CPU_DESCRIPTOR_HANDLE ShadowMapRenderTarget::GetDsvHandle() const {
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			pImpl->m_ShadowmapDsvHeap->GetCPUDescriptorHandleForHeapStart(), 0, 0
		);
		return dsvHandle;
	}

	ComPtr<ID3D12Resource> ShadowMapRenderTarget::GetDepthStencil() const {
		return pImpl->m_ShadowmapDepthStencil;
	}


	namespace Dx12ShaderHelper {
		//--------------------------------------------------------------------------------------
		//　void CreateShaderFlomCso(
		//		const wstring& fileName,	//csoファイル
		//		ID3DBlob** pResult	//Blob
		//	);
		//	用途: CSOデータからシェーダ用のBlobを作成する
		//--------------------------------------------------------------------------------------
		void CreateShaderFlomCso(const wstring& fileName, ID3DBlob** pResult) {
			try {
				if (fileName == L"") {
					ThrowBaseException(
						L"ファイルが指定されていません",
						L"if(fileName == L\"\")",
						L"Dx12ShaderResource::Impl::CreateShaderFlomCso()"
					);
				}
				DWORD RetCode;
				RetCode = GetFileAttributes(fileName.c_str());
				if (RetCode == -1) {
					ThrowBaseException(
						L"ファイルが存在しません",
						fileName,
						L"Dx12ShaderResource::Impl::CreateShaderFlomCso()"
					);
				}
				ThrowIfFailed(
					D3DReadFileToBlob(fileName.c_str(), pResult),
					L"Blpbの作成に失敗しました。",
					fileName,
					L"Dx12ShaderResource::Impl::CreateShaderFlomCso()"
				);

			}
			catch (...) {
				throw;
			}
		}
	}

	//end namespace Dx12ShaderHelper;



	//--------------------------------------------------------------------------------------
	//	class Dx12ShaderResource;
	//	用途: シェーダ関連リソースのインターフェイス
	//--------------------------------------------------------------------------------------
	Dx12ShaderResource::Dx12ShaderResource() {}
	Dx12ShaderResource::~Dx12ShaderResource() {}


	//シェーダアクセッサ
	ID3DBlob* Dx12ShaderResource::GetShaderBlob(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr) {
		//ミューテックス
		std::mutex Mutex;
		//ラムダ式利用
		return Util::DemandCreate(ShaderComPtr, Mutex, [&](ID3DBlob** pResult)
		{
			Dx12ShaderHelper::CreateShaderFlomCso(Filename, pResult);
		});
	}

	ComPtr<ID3DBlob>& Dx12ShaderResource::GetShaderBlobComPtr(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr) {
		//ミューテックス
		std::mutex Mutex;
		//ラムダ式利用
		Util::DemandCreate(ShaderComPtr, Mutex, [&](ID3DBlob** pResult)
		{
			Dx12ShaderHelper::CreateShaderFlomCso(Filename, pResult);
		});
		return ShaderComPtr;
	}


}
//end basecross
