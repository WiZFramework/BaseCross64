/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	�����̎���
	//--------------------------------------------------------------------------------------
	CubeObject::CubeObject(const wstring& TextureFileName, bool Trace, const Vec3& Pos, bool Flat) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(Pos),
		m_Flat(Flat)
	{}
	CubeObject::~CubeObject() {}

	void CubeObject::CreateBuffers() {
		float HelfSize = 0.5f;
		vector<Vec3> PosVec = {
			{ Vec3(-HelfSize, HelfSize, -HelfSize) },
			{ Vec3(HelfSize, HelfSize, -HelfSize) },
			{ Vec3(-HelfSize, -HelfSize, -HelfSize) },
			{ Vec3(HelfSize, -HelfSize, -HelfSize) },
			{ Vec3(HelfSize, HelfSize, HelfSize) },
			{ Vec3(-HelfSize, HelfSize, HelfSize) },
			{ Vec3(HelfSize, -HelfSize, HelfSize) },
			{ Vec3(-HelfSize, -HelfSize, HelfSize) },
		};
		vector<UINT> PosIndeces = {
			0, 1, 2, 3,
			1, 4, 3, 6,
			4, 5, 6, 7,
			5, 0, 7, 2,
			5, 4, 0, 1,
			2, 3, 7, 6,
		};


		vector<Vec3> FaceNormalVec = {
			{ Vec3(0, 0, -1.0f) },
			{ Vec3(1.0f, 0, 0) },
			{ Vec3(0, 0, 1.0f) },
			{ Vec3(-1.0f, 0, 0) },
			{ Vec3(0, 1.0f, 0) },
			{ Vec3(0, -1.0f, 0) }
		};

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		UINT BasePosCount = 0;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				VertexPositionNormalTexture Data;
				Data.position = PosVec[PosIndeces[BasePosCount + j]];
				if (m_Flat) {
					//�t���b�g�\���̏ꍇ�͖@���͒��_�����ɂ���
					Data.normal = Data.position;
					Data.normal.normalize();
				}
				else {
					//�t���b�g�\�����Ȃ��ꍇ�́A�@���͖ʂ̌���
					Data.normal = FaceNormalVec[i];
				}
				switch (j) {
				case 0:
					Data.textureCoordinate = Vec2(0, 0);
					break;
				case 1:
					Data.textureCoordinate = Vec2(1.0f, 0);
					break;
				case 2:
					Data.textureCoordinate = Vec2(0, 1.0f);
					break;
				case 3:
					Data.textureCoordinate = Vec2(1.0f, 1.0f);
					break;
				}
				vertices.push_back(Data);
			}

			indices.push_back((uint16_t)BasePosCount + 0);
			indices.push_back((uint16_t)BasePosCount + 1);
			indices.push_back((uint16_t)BasePosCount + 2);
			indices.push_back((uint16_t)BasePosCount + 1);
			indices.push_back((uint16_t)BasePosCount + 3);
			indices.push_back((uint16_t)BasePosCount + 2);

			BasePosCount += 4;
		}

		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_CubeMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}

	void CubeObject::UpdateConstantBuffer() {
		//�s��̒�`
		Mat4x4 World, View, Proj;
		//���[���h�s��̌���
		World.affineTransformation(
			m_Scale,			//�X�P�[�����O
			Vec3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		//�]�u����
		World.transpose();
		//�r���[�s��̌���
		View = XMMatrixLookAtLH(Vec3(0, 2.0, -5.0f), Vec3(0, 0, 0), Vec3(0, 1.0f, 0));
		//�]�u����
		View.transpose();
		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		//�]�u����
		Proj.transpose();
		//���C�e�B���O
		Vec4 LightDir(0.5f, -1.0f, 0.5f, 0.0f);
		LightDir.normalize();
		m_StaticConstantBuffer.World = World;
		m_StaticConstantBuffer.View = View;
		m_StaticConstantBuffer.Projection = Proj;
		m_StaticConstantBuffer.LightDir = LightDir;
		m_StaticConstantBuffer.Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		m_StaticConstantBuffer.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_StaticConstantBuffer),
			sizeof(m_StaticConstantBuffer));
	}


	///���[�g�V�O�l�`���쐬
	void CubeObject::CreateRootSignature() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�������łɓo�^����Ă����炻����g�p
		m_RootSignature = Dev->GetRootSignature(L"SrvSmpCbv");
		if (m_RootSignature != nullptr) {
			return;
		}
		//�f�X�N�v���^�����W��3��
		CD3DX12_DESCRIPTOR_RANGE ranges[3];
		//0�Ԗڂ̓V�F�[�_���\�[�X�r���[
		ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
		//1�Ԗڂ̓T���v���[
		ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
		//2�Ԗڂ̓R���X�^���g�o�b�t�@
		ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
		//���[�g�p�����[�^��3��
		CD3DX12_ROOT_PARAMETER rootParameters[3];
		//0�Ԗڂ̓V�F�[�_���\�[�X�r���[�̃����W���s�N�Z���V�F�[�_��
		rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
		//1�Ԗڂ̓T���v���[�̃����W���s�N�Z���V�F�[�_��
		rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
		//2�Ԗڂ̓R���X�^���g�o�b�t�@�̃����W�����ׂĂɃV�F�[�_��
		rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_ALL);
		//���[�g�V�O�l�`���̒�`
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		//���[�g�V�O�l�`����`�̏������i���[�g�p�����[�^��ݒ�j
		rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		//�e�u���u�i�o�C�i���C���[�W�j
		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		//���[�g�V�O�l�`����`�̃V���A���C�Y
		ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error),
			L"���[�g�V�O�l�`���̃V���A���C�Y�Ɏ��s���܂���",
			L"D3D12SerializeRootSignature()",
			L"CubeObject::CreateRootSignature()"
		);
		//���[�g�V�O�l�`���̍쐬
		ThrowIfFailed(
			Dev->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(),
				signature->GetBufferSize(), IID_PPV_ARGS(&m_RootSignature)),
			L"���[�g�V�O�l�`���̍쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateRootSignature()",
			L"CubeObject::CreateRootSignature()"
		);
		//���[�g�V�O�l�`���̓o�^�iDx12�ɂ͊֌W�Ȃ��j
		Dev->SetRootSignature(L"SrvSmpCbv", m_RootSignature);
	}
	///�f�X�N�v���^�q�[�v�쐬
	void CubeObject::CreateDescriptorHeap() {
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CbvSrvDescriptorHandleIncrementSize =
			Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		//�R���X�^���g�o�b�t�@�ƃV�F�[�_�[���\�[�X�p�f�X�N�v���^�q�[�v�i���v2�j
		D3D12_DESCRIPTOR_HEAP_DESC CbvSrvHeapDesc = {};
		CbvSrvHeapDesc.NumDescriptors = 2;
		CbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		CbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(Dev->GetDevice()->CreateDescriptorHeap(&CbvSrvHeapDesc, IID_PPV_ARGS(&m_CbvSrvUavDescriptorHeap)),
			L"CbvSrvUav�p�̃f�X�N�v���^�q�[�v�̍쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateDescriptorHeap()",
			L"CubeObject::CreateDescriptorHeap()"
		);
		//�T���v���[�f�X�N�v���^�q�[�v�i1�j
		D3D12_DESCRIPTOR_HEAP_DESC SamplerHeapDesc = {};
		SamplerHeapDesc.NumDescriptors = 1;
		SamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		SamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(Dev->GetDevice()->CreateDescriptorHeap(&SamplerHeapDesc, IID_PPV_ARGS(&m_SamplerDescriptorHeap)),
			L"�T���v���[�p�̃f�X�N�v���^�q�[�v�̍쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateDescriptorHeap()",
			L"CubeObject::CreateDescriptorHeap()"
		);
		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SrvHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
			m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(SamplerHandle);
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}
	///�T���v���[�쐬
	void CubeObject::CreateSampler() {
		//�T���v���[�f�X�N�v���^�q�[�v����CPU���n���h���𓾂�
		auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
		//�T���v���[��`
		D3D12_SAMPLER_DESC samplerDesc = {};
		//���j�A�N�����v�ɐݒ�
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		auto Dev = App::GetApp()->GetDeviceResources();
		//�n���h���ƃT���v���[��`�����т���
		Dev->GetDevice()->CreateSampler(&samplerDesc, SamplerDescriptorHandle);
	}
	///�R���X�^���g�o�b�t�@�쐬
	void CubeObject::CreateConstantBuffer() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(StaticConstantBuffer) + 255) & ~255;
		//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
		ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"CubeObject::CreateConstantBuffer()"
		);
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffSize;
		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		//�V�F�[�_�[���\�[�X��1�ŁA�擪����C���N�������g�T�C�Y����
		//���ꂽ�ꏊ���R���X�^���g�o�b�t�@�̃n���h���̏ꏊ�ł���
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			1,
			m_CbvSrvDescriptorHandleIncrementSize
		);
		Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvHandle);
		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"CubeObject::CreateConstantBuffer()"
		);
	}
	///�V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j�쐬
	void CubeObject::CreateShaderResourceView() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�e�N�X�`���n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g(�e�N�X�`�����\�[�X����擾)
		srvDesc.Format = m_TextureResource->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = m_TextureResource->GetTextureResDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[�̍쐬
		Dev->GetDevice()->CreateShaderResourceView(
			m_TextureResource->GetTexture().Get(),
			&srvDesc,
			srvHandle);
	}
	///�p�C�v���C���X�e�[�g�쐬
	void CubeObject::CreatePipelineState() {
		//���X�^���C�U�X�e�[�g�̍쐬�i�f�t�H���g�j
		CD3DX12_RASTERIZER_DESC rasterizerStateDesc(D3D12_DEFAULT);
		//���ʃJ�����O�ɕύX
		rasterizerStateDesc.CullMode = D3D12_CULL_MODE_BACK;
		//�p�C�v���C���X�e�[�g�̍쐬
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		ZeroMemory(&PineLineDesc, sizeof(PineLineDesc));
		PineLineDesc.InputLayout = { VertexPositionNormalTexture::GetVertexElement(), VertexPositionNormalTexture::GetNumElements() };
		PineLineDesc.pRootSignature = m_RootSignature.Get();
		PineLineDesc.VS =
		{
			reinterpret_cast<UINT8*>(VSPNTStatic::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
			VSPNTStatic::GetPtr()->GetShaderComPtr()->GetBufferSize()
		};
		PineLineDesc.PS =
		{
			reinterpret_cast<UINT8*>(PSPNTStatic::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
			PSPNTStatic::GetPtr()->GetShaderComPtr()->GetBufferSize()
		};
		PineLineDesc.RasterizerState = rasterizerStateDesc;
		PineLineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		PineLineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		PineLineDesc.SampleMask = UINT_MAX;
		PineLineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		PineLineDesc.NumRenderTargets = 1;
		PineLineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		PineLineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		PineLineDesc.SampleDesc.Count = 1;
		//���������Ȃ�u�����h�X�e�[�g�����ւ�
		if (m_Trace) {
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
			PineLineDesc.BlendState = blend_desc;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		//�܂�CullBack�̃p�C�v���C���X�e�[�g���쐬
		ThrowIfFailed(Dev->GetDevice()->CreateGraphicsPipelineState(&PineLineDesc, IID_PPV_ARGS(&m_CullBackPipelineState)),
			L"CullBack�p�C�v���C���X�e�[�g�̍쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateGraphicsPipelineState()",
			L"CubeObject::CreatePipelineState()"
		);
		PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		//������CullFront�̃p�C�v���C���X�e�[�g���쐬
		ThrowIfFailed(Dev->GetDevice()->CreateGraphicsPipelineState(&PineLineDesc, IID_PPV_ARGS(&m_CullFrontPipelineState)),
			L"CullFront�p�C�v���C���X�e�[�g�̍쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateGraphicsPipelineState()",
			L"CubeObject::CreatePipelineState()"
		);
	}
	///�R�}���h���X�g�쐬
	void CubeObject::CreateCommandList() {
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		ThrowIfFailed(Dev->GetDevice()->CreateCommandList(
			0,
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			Dev->GetCommandAllocator().Get(),
			m_CullBackPipelineState.Get(),
			IID_PPV_ARGS(&m_CommandList)),
			L"�R�}���h���X�g�̍쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommandList()",
			L"CubeObject::CreateCommandList()"
		);
		ThrowIfFailed(m_CommandList->Close(),
			L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
			L"commandList->Close()",
			L"CubeObject::CreateCommandList()"
		);
	}

	///�`�揈��
	void CubeObject::DrawObject(){
		auto Dev = App::GetApp()->GetDeviceResources();
		//�R�}���h���X�g�̃��Z�b�g
		if (m_Trace) {
			ThrowIfFailed(m_CommandList->Reset(Dev->GetCommandAllocator().Get(), m_CullFrontPipelineState.Get()),
				L"�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂���",
				L"m_CommandList->Reset(Dev->GetCommandAllocator().Get(),pipelineState.Get())",
				L"CubeObject::OnDraw()"
			);
		}
		else {
			ThrowIfFailed(m_CommandList->Reset(Dev->GetCommandAllocator().Get(), m_CullBackPipelineState.Get()),
				L"�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂���",
				L"m_CommandList->Reset(Dev->GetCommandAllocator().Get(),pipelineState.Get())",
				L"CubeObject::OnDraw()"
			);
		}
		//���b�V�����X�V����Ă���΃��\�[�X�X�V
		m_CubeMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		//�e�N�X�`�����X�V����Ă���΃��\�[�X�X�V
		m_TextureResource->UpdateResources(m_CommandList);
		//���[�g�V�O�l�`���̃Z�b�g
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
		//�f�X�N�v���^�q�[�v�̃Z�b�g
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		//GPU���f�X�N�v���^�n���h���̃Z�b�g
		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable((UINT)i, m_GPUDescriptorHandleVec[i]);
		}
		//�r���[�|�[�g�̃Z�b�g
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		//�V�U�[���N�g�̃Z�b�g
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());
		//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Dev->GetRtvHandle();
		//�f�v�X�X�e���V���r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Dev->GetDsvHandle();
		//�擾�����n���h�����Z�b�g
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		//�`������i�O�p�`���X�g�j���Z�b�g
		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//���_�o�b�t�@���Z�b�g
		m_CommandList->IASetVertexBuffers(0, 1, &m_CubeMesh->GetVertexBufferView());
		//�C���f�b�N�X�o�b�t�@���Z�b�g
		m_CommandList->IASetIndexBuffer(&m_CubeMesh->GetIndexBufferView());
		//�`��
		m_CommandList->DrawIndexedInstanced(m_CubeMesh->GetNumIndicis(), 1, 0, 0, 0);
		if (m_Trace) {
			//�����̏ꍇ�́ACullBack�ɕύX����2��ڕ`��
			m_CommandList->SetPipelineState(m_CullBackPipelineState.Get());
			m_CommandList->DrawIndexedInstanced(m_CubeMesh->GetNumIndicis(), 1, 0, 0, 0);
		}
		//�R�}���h���X�g�̃N���[�Y
		ThrowIfFailed(m_CommandList->Close(),
			L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
			L"m_CommandList->Close()",
			L"CubeObject::OnDraw()"
		);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}


	void CubeObject::OnCreate() {
		//���b�V���̍쐬
		CreateBuffers();
		//�e�N�X�`���̍쐬
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		m_Scale = Vec3(1.0f, 1.0f, 1.0f);
		m_Qt.identity();
		///�e�������֐��Ăяo��
		///���[�g�V�O�l�`���쐬
		CreateRootSignature();
		///�f�X�N�v���^�q�[�v�쐬
		CreateDescriptorHeap();
		///�T���v���[�쐬
		CreateSampler();
		///�V�F�[�_�[���\�[�X�r���[�쐬
		CreateShaderResourceView();
		///�R���X�^���g�o�b�t�@�쐬
		CreateConstantBuffer();
		///�p�C�v���C���X�e�[�g�쐬
		CreatePipelineState();
		///�R�}���h���X�g�쐬
		CreateCommandList();

		//�R���X�^���g�o�b�t�@�̍X�V
		UpdateConstantBuffer();
	}
	void CubeObject::OnUpdate() {
		Quat QtSpan;
		QtSpan.rotation(0.02f, Vec3(0, 1.0f, 0));
		m_Qt *= QtSpan;
		m_Qt.normalize();
	}
	void CubeObject::OnDraw() {
		//�R���X�^���g�o�b�t�@�̍X�V
		UpdateConstantBuffer();
		//�`��
		DrawObject();
	}





}
//end basecross
