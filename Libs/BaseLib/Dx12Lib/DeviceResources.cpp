/*!
@file DeviceResources.cpp
@brief �e�N�X�`���A���b�V���ADx12�f�o�C�X���\�[�X����
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
		//�e�N�X�`���p�̃f�X�N�v���^
		D3D12_RESOURCE_DESC m_TextureResDesc;
		UINT m_TexturePixelSize;
		//���\�[�X
		ComPtr<ID3D12Resource> m_Texture;
		//GPU�A�b�v���[�h�̂��߂̃��\�[�X
		ComPtr<ID3D12Resource> m_TextureUploadHeap;
		//�e�N�X�`���t�@�C���ւ̃p�X
		wstring m_FileName{ L"" };
		//�e�N�X�`���f�[�^
		vector<uint8_t> m_Data;
		bool m_DataRefresh;
		Impl() :m_TexturePixelSize(4), m_DataRefresh(true) {}
		//�e�N�X�`���쐬
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
				Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE, &m_TextureResDesc,
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&m_Texture)),
				L"�e�N�X�`�����\�[�X�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"TextureResource::Impl::CreateTexture()"
			);
		}

		void CreateTexture(const Image* srcImages, size_t nimages, const DirectX::TexMetadata& metadata) {
			if (metadata.IsVolumemap()) {
				ThrowBaseException(
					L"���̌`���ɂ͑Ή����Ă܂���",
					L"if (metadata.IsVolumemap())",
					L"TextureResource::Impl::CreateTexture()"
				);
			}

			std::unique_ptr<D3D11CUSTAM_SUBRESOURCE_DATA[]> initData(new (std::nothrow) D3D11CUSTAM_SUBRESOURCE_DATA[metadata.mipLevels * metadata.arraySize]);
			if (!initData) {
				ThrowBaseException(
					L"�e�N�X�`���f�[�^�̗̈�擾�Ɏ��s���܂���",
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
							L"�f�[�^�̃C���f�b�N�X���͈͊O�ł�",
							L"if (index >= nimages)",
							L"TextureResource::Impl::CreateTexture()"
						);

					}
					const Image& img = srcImages[index];

					if (img.format != metadata.format) {
						ThrowBaseException(
							L"�f�[�^�̃t�H�[�}�b�g���Ⴂ�܂�",
							L"if (img.format != metadata.format)",
							L"TextureResource::Impl::CreateTexture()"
						);
					}
					if (!img.pixels) {
						ThrowBaseException(
							L"�f�[�^�̃|�C���^���s���ł�",
							L"if (!img.pixels)",
							L"TextureResource::Impl::CreateTexture()"
						);
					}
					if (idx >= (metadata.mipLevels * metadata.arraySize)) {
						ThrowBaseException(
							L"�f�[�^�̕��т��s��ł�",
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
				Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE, &m_TextureResDesc,
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&m_Texture)),
				L"�e�N�X�`�����\�[�X�̍쐬�Ɏ��s���܂���",
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

		//�f�[�^�̃R�s�[
		void CopyData(const vector<uint8_t>& data) {
			m_Data.clear();
			unsigned int rowPitch = (unsigned int)m_TextureResDesc.Width * m_TexturePixelSize;
			m_Data.resize(rowPitch * (unsigned int)m_TextureResDesc.Height);
			if (m_Data.size() > data.size()) {
				ThrowBaseException(
					L"�n���ꂽ�f�[�^�̃T�C�Y���A�c���̐��ɑ���܂���",
					L"if (pImpl->m_Data.size() > data.size())",
					L"TextureResource::Impl::CopyData()"
				);
			}
			for (uint32_t i = 0; i < m_Data.size(); i++) {
				m_Data[i] = data[i];
			}
			m_DataRefresh = true;
		}

		//GPU�A�b�v���[�h�o�b�t�@�̍쐬.
		void CreateTextureUploadHeap() {
			auto Dev = App::GetApp()->GetDeviceResources();
			const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_Texture.Get(), 0, 1);

			ThrowIfFailed(
				Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&m_TextureUploadHeap)),
				L"GPU�A�b�v���[�h�o�b�t�@�̍쐬�Ɏ��s���܂���",
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

		//�t�@�C���ł̍\�z
		try {

			pImpl->m_FileName = FileName;
			if (FileName == L"") {
				ThrowBaseException(
					L"�t�@�C�����w�肳��Ă��܂���",
					L"if(FileName == L\"\")",
					L"TextureResource::OnCreateWithParam()"
				);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(FileName.c_str());
			if (RetCode == -1) {
				wstring patherr = FileName;
				ThrowBaseException(
					L"�t�@�C�������݂��܂���",
					patherr.c_str(),
					L"TextureResource::OnCreateWithParam()"
				);
			}
			//�e�N�X�`���쐬
			DirectX::TexMetadata metadata;
			DirectX::ScratchImage image;

			if (TexType == L"WIC") {
				//�t�@�C���g���q�̒���
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
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						FileName,
						L"TextureResource::OnCreateWithParam()"
					);
				}
				else if (ExtStr == L".tga" || ExtStr == L".TGA") {
					ThrowIfFailed(
						DirectX::LoadFromTGAFile(FileName.c_str(), &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						FileName,
						L"TextureResource::OnCreateWithParam()"
					);
				}
				else {
					ThrowIfFailed(
						DirectX::LoadFromWICFile(FileName.c_str(), 0, &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						FileName,
						L"TextureResource::OnCreateWithParam()"
					);
				}
			}
			else if (TexType == L"DDS") {
				ThrowIfFailed(
					DirectX::LoadFromDDSFile(FileName.c_str(), 0, &metadata, image),
					L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
					FileName,
					L"TextureResource::OnCreateWithParam()"
				);
			}
			else if (TexType == L"TGA") {
				ThrowIfFailed(
					DirectX::LoadFromTGAFile(FileName.c_str(), &metadata, image),
					L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
					FileName,
					L"TextureResource::OnCreateWithParam()"
				);
			}
			else {
				ThrowBaseException(
					L"���̃^�C�v�͑Ή����Ă��܂���",
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

	//���\�[�X
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


	//�e�N�X�`���f�[�^
	vector<uint8_t>& TextureResource::GetData() const {
		return pImpl->m_Data;
	}
	void TextureResource::SetData(const vector<uint8_t>& data) {
		pImpl->CopyData(data);
	}
	void TextureResource::UpdateTexture(const vector<uint8_t>& data) {
		pImpl->CopyData(data);
	}


	//���\�[�X�̍X�V
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
	//	�p�r: Direct12�f�o�C�X�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DeviceResources::Impl {

		static const UINT FrameCount = 3;
		//�p�C�v���C��objects.
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

		///�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g
		shared_ptr<ShadowMapRenderTarget> m_ShadowMapRenderTarget;

		UINT m_RtvDescriptorSize;
		UINT m_DsvDescriptorSize;

		//�N���A�����p�̃I�u�W�F�N�g
		ComPtr<ID3D12RootSignature> m_RootSignature;
		ComPtr<ID3D12PipelineState> m_PipelineState;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;


		//�ėp���[�g�V�O�l�`���̃}�b�v
		map<wstring, ComPtr<ID3D12RootSignature>> m_RootSignatureMap;
		//�v���[���g�o���A�p�̃R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_PresentCommandList;
		//�R�}���h���X�g���s�p�̔z��
		vector<ID3D12CommandList*> m_DrawCommandLists;

		// Dx11�R���e�L�X�g
		ComPtr<ID3D11DeviceContext> m_d3d11DeviceContext;

		// Direct2D �`��R���|�[�l���g�B
		ComPtr<ID2D1Factory3>		m_d2dFactory;
		ComPtr<ID2D1Device2>		m_d2dDevice;
		ComPtr<ID2D1DeviceContext2>	m_d2dDeviceContext;

		// DirectWrite �`��R���|�[�l���g�B
		ComPtr<IDWriteFactory>		m_dwriteFactory;
		ComPtr<IWICImagingFactory2>	m_wicFactory;


		//�����I�u�W�F�N�g
		UINT m_FrameIndex;
		HANDLE m_FenceEvent;
		ComPtr<ID3D12Fence> m_Fence;
		UINT64 m_FenceValue;
		float m_dpi;
		float m_aspectRatio;
		//�\�z�Ɣj��
		Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		~Impl();
		//���\�[�X�̍\�z
		void CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//�A�_�v�^�[�擾
		void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
		//��������
		void WaitForPreviousFrame(bool ExceptionActive = true);
	};
	//�\�z
	DeviceResources::Impl::Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) :
		m_dpi(96.0f),
		m_Viewport(),
		m_ScissorRect()
	{
		try {
			//���\�[�X�̍\�z
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


	//�A�_�v�^�[�擾�w���p�[�֐�
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


	//���\�[�X�̍\�z
	void DeviceResources::Impl::CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) {

		m_Viewport.Width = static_cast<float>(Width);
		m_Viewport.Height = static_cast<float>(Height);
		m_Viewport.MaxDepth = 1.0f;

		m_ScissorRect.right = static_cast<LONG>(Width);
		m_ScissorRect.bottom = static_cast<LONG>(Height);


		m_aspectRatio = static_cast<float>(Width) / static_cast<float>(Height);

/*
		// Direct2D ���\�[�X�����������܂��B

*/

		UINT dxgiFactoryFlags = 0;
		UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		D2D1_FACTORY_OPTIONS d2dFactoryOptions = {};

#if defined(_DEBUG)
		//D3D12 debug �L��.
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


		//DXGIFactory�̍쐬
		ComPtr<IDXGIFactory4> factory;
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)),
			L"DXGIFactory�̍쐬�Ɏ��s���܂���",
			L"CreateDXGIFactory1(IID_PPV_ARGS(&factory)",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);

		//�A�_�v�^�̎擾
		{
			//�n�[�h�E�F�A�A�_�v�^�̎擾
			ComPtr<IDXGIAdapter1> hardwareAdapter;
			GetHardwareAdapter(factory.Get(), &hardwareAdapter);
			if (FAILED(D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)))) {
				//���s�����烉�b�v�A�_�v�^�̎擾
				ComPtr<IDXGIAdapter> warpAdapter;
				ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)),
					L"���b�v�A�_�v�^�̍쐬�Ɏ��s���܂���",
					L"factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter))",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

				ThrowIfFailed(D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device)),
					L"�f�o�C�X�̍쐬�Ɏ��s���܂���",
					L"D3D12CreateDevice(warpAdapter.Get(),D3D_FEATURE_LEVEL_11_0,IID_PPV_ARGS(&m_Device))",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

			}
		}

		//�R�}���h�L���[�̍쐬.
		{
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			ThrowIfFailed(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)),
				L"�R�}���h�L���[�̍쐬�Ɏ��s���܂���",
				L"m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue))",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
		}

		//�X���b�v�`�F�[���̍쐬.
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
				L"�X���b�v�`�F�[���̍쐬�Ɏ��s���܂���",
				L"factory->CreateSwapChain(m_CommandQueue.Get(), &swapChainDesc, &swapChain)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);

			ThrowIfFailed(factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER),
				L"Window���[�h����t���X�N���[���ւ̕ύX��~���ݒ�ł��܂���ł���",
				L"factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);

			ThrowIfFailed(swapChain.As(&m_SwapChain),
				L"�X���b�v�`�F�[���̃o�[�W�����ύX�Ɏ��s���܂���",
				L"swapChain.As(&m_SwapChain)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			//�t���[���C���f�b�N�X��ݒ肵�Ă���
			m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();
		}
		//Dx11�f�o�C�X�̍쐬
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
				L"�X���b�v�`�F�[���̃o�[�W�����ύX�Ɏ��s���܂���",
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

		//�f�X�N�v���^�q�[�v�̍쐬.
		{
			//�����_�[�^�[�Q�b�g�r���[�̃f�X�N�v���^�q�[�v�쐬
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
			rtvHeapDesc.NumDescriptors = FrameCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ThrowIfFailed(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvHeap)),
				L"�����_�[�^�[�Q�b�g�r���[�̃f�X�N�v���^�q�[�v�쐬�Ɏ��s���܂���",
				L"m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RtvHeap)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			// �f�v�X�X�e���V���r���[�̃f�X�N�v���^�q�[�v�쐬
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ThrowIfFailed(m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DsvHeap)),
				L"�f�v�X�X�e���V���r���[�̃f�X�N�v���^�q�[�v�쐬�Ɏ��s���܂���",
				L"m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DsvHeap)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			m_RtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		}

		//RTV�̃t���[�����\�[�X�̍쐬
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_RtvHeap->GetCPUDescriptorHandleForHeapStart());
			//FrameCount������RTV�̃t���[�����\�[�X�̍쐬
			for (UINT n = 0; n < FrameCount; n++)
			{
				ThrowIfFailed(m_SwapChain->GetBuffer(n, IID_PPV_ARGS(&m_RenderTargets[n])),
					L"RTV�̃t���[�����\�[�X�̍쐬�Ɏ��s���܂���",
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
					L"d3d11On12�f�o�C�X�̍쐬�Ɏ��s���܂���",
					L"m_d3d11On12Device->CreateWrappedResource()",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

				// Create a render target for D2D to draw directly to this back buffer.
				ComPtr<IDXGISurface> surface;
				ThrowIfFailed(m_WrappedBackBuffers[n].As(&surface),
					L"WrappedBackBuffersD2DRenderTarget�̍쐬�Ɏ��s���܂���",
					L"m_WrappedBackBuffers[n].As(&surface)",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

				ThrowIfFailed(m_d2dDeviceContext->CreateBitmapFromDxgiSurface(
					surface.Get(),
					&bitmapProperties,
					&m_d2dRenderTargets[n]
				),
					L"BitmapFromDxgiSurface�̍쐬�Ɏ��s���܂���",
					L"m_d2dDeviceContext->CreateBitmapFromDxgiSurface()",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);

				rtvHandle.Offset(1, m_RtvDescriptorSize);

				ThrowIfFailed(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator[n])),
					L"�R�}���h�A���P�[�^�̍쐬�Ɏ��s���܂���",
					L"CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator[n])",
					L"Dx12DeviceResources::Impl::CreateDeviceResources()"
				);
			}
		}

		//DSV(�f�v�X�X�e���V���r���[)
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
				L"�f�v�X�X�e���V�����\�[�X�쐬�Ɏ��s���܂���",
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
				L"��̃��[�g�V�O�l�`���̃V���A���C�Y�Ɏ��s���܂���",
				L"D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			ThrowIfFailed(m_Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)),
				L"���[�g�V�O�l�`���̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateRootSignature)",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
		}


		// Direct2D �f�o�C�X �I�u�W�F�N�g�ƁA�Ή�����R���e�L�X�g���쐬���܂��B

		/*


		*/


		//��ʃN���A�p�̃R�}���h���X�g
		ThrowIfFailed(
			m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator[m_FrameIndex].Get(), nullptr, IID_PPV_ARGS(&m_CommandList)),
			L"�R�}���h���X�g�̍쐬�Ɏ��s���܂���",
			L"m_Device->CreateCommandList()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(m_CommandList->Close(),
			L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
			L"m_CommandList->Close()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);
		//�v���[���g�p�̃R�}���h���X�g
		ThrowIfFailed(
			m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator[m_FrameIndex].Get(), nullptr, IID_PPV_ARGS(&m_PresentCommandList)),
			L"�R�}���h���X�g�̍쐬�Ɏ��s���܂���",
			L"m_Device->CreateCommandList()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(m_PresentCommandList->Close(),
			L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
			L"m_CommandList->Close()",
			L"Dx12DeviceResources::Impl::CreateDeviceResources()"
		);
	}

	//��������
	void DeviceResources::Impl::WaitForPreviousFrame(bool ExceptionActive) {
		// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
		// This is code implemented as such for simplicity. More advanced samples 
		// illustrate how to use fences for efficient resource usage.

		// Signal and increment the fence value.
		const UINT64 fence = m_FenceValue;
		if (ExceptionActive) {
			ThrowIfFailed(m_CommandQueue->Signal(m_Fence.Get(), fence),
				L"�R�}���h�L���[�̃V�O�i���Ɏ��s���܂���",
				L"m_CommandQueue->Signal(m_Fence.Get(), fence)",
				L"Dx12DeviceResources::Impl::WaitForPreviousFrame()"
			);
			m_FenceValue++;
			// Wait until the previous frame is finished.
			if (m_Fence->GetCompletedValue() < fence)
			{

				ThrowIfFailed(m_Fence->SetEventOnCompletion(fence, m_FenceEvent),
					L"�C�x���g�̐ݒ�Ɏ��s���܂���",
					L"m_Fence->SetEventOnCompletion(fence, m_FenceEvent)",
					L"Dx12DeviceResources::Impl::WaitForPreviousFrame()"
				);
				WaitForSingleObject(m_FenceEvent, INFINITE);
			}
			m_FrameIndex = m_SwapChain->GetCurrentBackBufferIndex();
		}
		else {
			//��O�𓊂��Ȃ��B�f�X�g���N�^�p
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
		//�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g���쐬
		pImpl->m_ShadowMapRenderTarget = make_shared<ShadowMapRenderTarget>(ShadowMapDimension);
		return pImpl->m_ShadowMapRenderTarget;
	}


	shared_ptr<ShadowMapRenderTarget> DeviceResources::GetShadowMapRenderTarget(float ShadowMapDimension) {
		if (!pImpl->m_ShadowMapRenderTarget) {
			ThrowBaseException(
				L"�V���h�E�}�b�v���쐬����Ă��܂���",
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





	//�ʏ�`��̃N���A
	void DeviceResources::ClearDefaultViews(const bsm::Col4& col) {
		ThrowIfFailed(pImpl->m_CommandAllocator[pImpl->m_FrameIndex]->Reset(),
			L"�R�}���h�A���P�[�^�̃��Z�b�g�Ɏ��s���܂���",
			L"m_CommandAllocator->Reset()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);

		ThrowIfFailed(pImpl->m_CommandList->Reset(pImpl->m_CommandAllocator[pImpl->m_FrameIndex].Get(), pImpl->m_PipelineState.Get()),
			L"�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂���",
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
			L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
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
			L"�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂���",
			L"pImpl->m_CommandList->Reset()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);

		//�v���[���g�p�̃o���A�𒣂�
		pImpl->m_PresentCommandList->ResourceBarrier(1,
			&CD3DX12_RESOURCE_BARRIER::Transition(pImpl->m_RenderTargets[pImpl->m_FrameIndex].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		ThrowIfFailed(pImpl->m_PresentCommandList->Close(),
			L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
			L"m_CommandList->Close()",
			L"Dx12DeviceResources::ClearDefultViews()"
		);
		InsertDrawCommandLists(pImpl->m_PresentCommandList.Get());

		// Execute the command list.
		pImpl->m_CommandQueue->ExecuteCommandLists((UINT)pImpl->m_DrawCommandLists.size(), &pImpl->m_DrawCommandLists[0]);

		// �X�e�[�g���N���A���C�f�t�H���g��Ԃɂ��܂�.
		// TODO : �����ɋL�q���Ă������m�F�v�K�v
		pImpl->m_d3d11DeviceContext->ClearState();
		pImpl->m_d3d11DeviceContext->Flush();

		// Present the frame.
		ThrowIfFailed(pImpl->m_SwapChain->Present(SyncInterval, Flags),
			L"�X���b�v�`�F�[���̃v���[���g�Ɏ��s���܂���",
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

	//�R���e���c�̍쐬��̏���
	void DeviceResources::AfterInitContents(bool ShadowActive) {
		if (ShadowActive) {
			CreateShadowMapRenderTarget(2048.0f);
		}
		//�����I�u�W�F�N�g�̍쐬�Ɠ�������
		{

			ThrowIfFailed(pImpl->m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&pImpl->m_Fence)),
				L"�t�F���X�̍쐬�Ɏ��s���܂���",
				L"m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence))",
				L"Dx12DeviceResources::Impl::CreateDeviceResources()"
			);
			pImpl->m_FenceValue = 1;

			//�t���[�������̂��߂̃C�x���g�n���h���̍쐬
			pImpl->m_FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			if (pImpl->m_FenceEvent == nullptr)
			{
				ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()),
					L"���X�g�G���[�̎擾�Ɏ��s���܂���",
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
	ComPtr<ID3D12Device> DeviceResources::GetDevice() const {
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
				L"�C���f�b�N�X������𒴂��Ă��܂�",
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
				L"�C���f�b�N�X������𒴂��Ă��܂�",
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
				L"�C���f�b�N�X������𒴂��Ă��܂�",
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
				L"���łɂ��̃��[�g�V�O�l�`���͑��݂��܂�",
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

	// D2D �A�N�Z�T�[�B
	ID2D1Factory2*			DeviceResources::GetD2DFactory() const { return pImpl->m_d2dFactory.Get(); }
	ID2D1Device1*			DeviceResources::GetD2DDevice() const { return pImpl->m_d2dDevice.Get(); }
	ID2D1DeviceContext2*	DeviceResources::GetD2DDeviceContext() const { return pImpl->m_d2dDeviceContext.Get(); }
	IDWriteFactory*			DeviceResources::GetDWriteFactory() const { return pImpl->m_dwriteFactory.Get(); }
	IWICImagingFactory2*	DeviceResources::GetWicImagingFactory() const { return pImpl->m_wicFactory.Get(); }


	//--------------------------------------------------------------------------------------
	//	struct RenderTarget::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct RenderTarget::Impl {
		Impl() {
		}
	};

	//--------------------------------------------------------------------------------------
	//	class RenderTarget;
	//	�p�r: �����_�����O�^�[�Q�b�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	RenderTarget::RenderTarget() :
		pImpl(new Impl())
	{}
	RenderTarget::~RenderTarget() {}


	//--------------------------------------------------------------------------------------
	//	struct ShadowMapRenderTarget::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct ShadowMapRenderTarget::Impl {

		//�V���h�E�}�b�v�̑傫��
		const float m_ShadowMapDimension;
		//�V���h�E�}�b�v�̃f�X�N�v���^�q�[�v
		ComPtr<ID3D12DescriptorHeap> m_ShadowmapDsvHeap;
		//�V���h�E�}�b�v�̃f�v�X�X�e���V��
		ComPtr<ID3D12Resource> m_ShadowmapDepthStencil;
		//�N���A�p�I�u�W�F�N�g
		ComPtr<ID3D12RootSignature> m_RootSignature;
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		//End�p�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_EndCommandList;

		Impl(float ShadowMapDimension) :
			m_ShadowMapDimension(ShadowMapDimension)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g
	//--------------------------------------------------------------------------------------
	ShadowMapRenderTarget::ShadowMapRenderTarget(float ShadowMapDimension):
		pImpl(new Impl(ShadowMapDimension))
	{
		try {
			//�V���h�E�}�b�v��col�͖��g�p
			auto Dev = App::GetApp()->GetDeviceResources();

			//�V���h�E�}�b�v�p�f�X�N�v���^�q�[�v
			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
			dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			ThrowIfFailed(Dev->GetDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&pImpl->m_ShadowmapDsvHeap)),
				L"�V���h�E�}�b�v�f�v�X�X�e���V���r���[�̃f�X�N�v���^�q�[�v�쐬�Ɏ��s���܂���",
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

			ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
				&heapProperties,
				D3D12_HEAP_FLAG_NONE,
				&shadowTextureDesc,
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				&depthOptimizedClearValue,
				IID_PPV_ARGS(&pImpl->m_ShadowmapDepthStencil)
			),
				L"�V���h�E�}�b�v�f�v�X�X�e���V�����\�[�X�쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);

			D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
			depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
			depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;



			//�f�v�X�X�e���V���r���[�̍쐬
			Dev->GetDevice()->CreateDepthStencilView(pImpl->m_ShadowmapDepthStencil.Get(), &depthStencilViewDesc,
				pImpl->m_ShadowmapDsvHeap->GetCPUDescriptorHandleForHeapStart());

			//���[�g�V�O�l�`��
			{
				CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
				rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

				ComPtr<ID3DBlob> signature;
				ComPtr<ID3DBlob> error;

				ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error),
					L"��̃��[�g�V�O�l�`���̃V���A���C�Y�Ɏ��s���܂���",
					L"D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error)",
					L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
				);
				ThrowIfFailed(Dev->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&pImpl->m_RootSignature)),
					L"���[�g�V�O�l�`���̍쐬�Ɏ��s���܂���",
					L"Dev->GetDevice()->CreateRootSignature)",
					L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
				);
			}

			ComPtr<ID3D12PipelineState> PipelineState;
			//�N���A�p�R�}���h���X�g
			ThrowIfFailed(Dev->GetDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				Dev->GetCommandAllocator(Dev->GetFrameIndex()).Get(),
				PipelineState.Get(),
				IID_PPV_ARGS(&pImpl->m_CommandList)),
				L"�R�}���h���X�g�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommandList()",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);
			CommandList::Close(pImpl->m_CommandList);

			//End�p�R�}���h���X�g
			ThrowIfFailed(Dev->GetDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				Dev->GetCommandAllocator(Dev->GetFrameIndex()).Get(),
				PipelineState.Get(),
				IID_PPV_ARGS(&pImpl->m_EndCommandList)),
				L"�R�}���h���X�g�̍쐬�Ɏ��s���܂���",
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

	//�����_�����O�^�[�Q�b�g���N���A����
	void ShadowMapRenderTarget::ClearViews(const bsm::Col4& col) {
		//�V���h�E�}�b�v��col�͖��g�p
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
	//�����_�����O�^�[�Q�b�g���J�n����
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
		//�@void CreateShaderFlomCso(
		//		const wstring& fileName,	//cso�t�@�C��
		//		ID3DBlob** pResult	//Blob
		//	);
		//	�p�r: CSO�f�[�^����V�F�[�_�p��Blob���쐬����
		//--------------------------------------------------------------------------------------
		void CreateShaderFlomCso(const wstring& fileName, ID3DBlob** pResult) {
			try {
				if (fileName == L"") {
					ThrowBaseException(
						L"�t�@�C�����w�肳��Ă��܂���",
						L"if(fileName == L\"\")",
						L"Dx12ShaderResource::Impl::CreateShaderFlomCso()"
					);
				}
				DWORD RetCode;
				RetCode = GetFileAttributes(fileName.c_str());
				if (RetCode == -1) {
					ThrowBaseException(
						L"�t�@�C�������݂��܂���",
						fileName,
						L"Dx12ShaderResource::Impl::CreateShaderFlomCso()"
					);
				}
				ThrowIfFailed(
					D3DReadFileToBlob(fileName.c_str(), pResult),
					L"Blpb�̍쐬�Ɏ��s���܂����B",
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
	//	�p�r: �V�F�[�_�֘A���\�[�X�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	Dx12ShaderResource::Dx12ShaderResource() {}
	Dx12ShaderResource::~Dx12ShaderResource() {}


	//�V�F�[�_�A�N�Z�b�T
	ID3DBlob* Dx12ShaderResource::GetShaderBlob(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr) {
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//�����_�����p
		return Util::DemandCreate(ShaderComPtr, Mutex, [&](ID3DBlob** pResult)
		{
			Dx12ShaderHelper::CreateShaderFlomCso(Filename, pResult);
		});
	}

	ComPtr<ID3DBlob>& Dx12ShaderResource::GetShaderBlobComPtr(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr) {
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//�����_�����p
		Util::DemandCreate(ShaderComPtr, Mutex, [&](ID3DBlob** pResult)
		{
			Dx12ShaderHelper::CreateShaderFlomCso(Filename, pResult);
		});
		return ShaderComPtr;
	}


}
//end basecross
