/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
@copyright shike.y
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	�l�p�`�X�v���C�g����
	//--------------------------------------------------------------------------------------
	SquareSprite::SquareSprite() :
		ObjectInterface(),
		ShapeInterface(),
		m_Scale(128.0f, 128.0f),
		m_Rot(0),
		m_Pos(0, 0),
		m_PosSpan(3.0f, 0)
	{}
	SquareSprite::~SquareSprite() {}

	//�R���X�^���g�o�b�t�@�X�V
	void SquareSprite::UpdateConstantBuffer() {
		
		//�s��̒�`
		Mat4x4 World, Proj;
		
		//���[���h�s��̌���
		World.affineTransformation2D(
			m_Scale,			//�X�P�[�����O
			Vec2(0, 0),			//��]�̒��S
			m_Rot,				//��]�p�x
			m_Pos				//�ʒu
		);

		//�ˉe�s��̌���
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj = XMMatrixOrthographicLH(w, h, -1.0, 1.0f);

		//�s��̍���
		World *= Proj;

		//�R���X�^���g�o�b�t�@�̏���
		m_SpriteConstantBuffer.Emissive = Col4(0.0f, 0.0f, 0, 1.0f);
		m_SpriteConstantBuffer.World = World;

		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_SpriteConstantBuffer),
			sizeof(m_SpriteConstantBuffer));
	}



	///���[�g�V�O�l�`���쐬
	void SquareSprite::CreateRootSignature() {
		//�R���X�^���g�o�b�t�@�t���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateCbv();
	}

	///�f�X�N�v���^�q�[�v�쐬
	void SquareSprite::CreateDescriptorHeap() {
		auto Device = App::GetApp()->GetDeviceResources();

		m_CbvSrvDescriptorHandleIncrementSize
			= Device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//CbvSrv�f�X�N�v���^�q�[�v(�R���X�^���g�o�b�t�@�̂�)
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);

		//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);

		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}

	///�R���X�^���g�o�b�t�@�쐬
	void SquareSprite::CreateConstantBuffer() {
		auto Device = App::GetApp()->GetDeviceResources();

		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBuffSize = (sizeof(SpriteConstantBuffer) + 255) & ~255;
		ThrowIfFailed(Device->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Dev->GetDevice()->CreateCommittedResource()",
			L"SquareSprite::CreateConstantBuffer()"
		);

		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		cbvDesc.SizeInBytes = ConstBuffSize;

		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
		//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		Device->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);

		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
			L"pImpl->m_ConstantBufferUploadHeap->Map()",
			L"SquareSprite::CreateConstantBuffer()"
		);
	}

	///�p�C�v���C���X�e�[�g�쐬
	void SquareSprite::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState
			= PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(m_RootSignature, PineLineDesc);
	}

	///�R�}���h���X�g�쐬
	void SquareSprite::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	///�`�揈��
	void SquareSprite::DrawObject() {
		auto Device = App::GetApp()->GetDeviceResources();
		
		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);
		
		//���b�V�����X�V����Ă���΃��\�[�X�X�V
		m_SquareMesh->UpdateResources<VertexPositionColor>(m_CommandList);
		
		//���[�g�V�O�l�`���̃Z�b�g
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		
		//�f�X�N�v���^�q�[�v�̃Z�b�g
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		
		//GPU�f�X�N�v���^�q�[�v�n���h���̃Z�b�g
		for (UINT i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}
		
		//�r���[�|�[�g�̃Z�b�g
		m_CommandList->RSSetViewports(1, &Device->GetViewport());
		
		//�V�U�[��`�i�N���b�v��`�j�̃Z�b�g
		m_CommandList->RSSetScissorRects(1, &Device->GetScissorRect());

		//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Device->GetRtvHandle();
		//�f�v�X�X�e���V���r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Device->GetDsvHandle();
		//�擾�����n���h�����Z�b�g
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
		//�`����@�̃Z�b�g�i�O�p�`���X�g�j
		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//���_�o�b�t�@�̃Z�b�g
		m_CommandList->IASetVertexBuffers(0, 1, &m_SquareMesh->GetVertexBufferView());
		//�C���f�b�N�X�o�b�t�@���Z�b�g
		m_CommandList->IASetIndexBuffer(&m_SquareMesh->GetIndexBufferView());
		//�C���f�b�N�X�`��
		m_CommandList->DrawIndexedInstanced(m_SquareMesh->GetNumIndicis(), 1, 0, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Device->InsertDrawCommandLists(m_CommandList.Get());
	}

	void SquareSprite::OnCreate() {
		float HelfSize = 0.5f;
		
		//���_�z��
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(Vec3(-HelfSize, HelfSize, 0), Col4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, HelfSize, 0), Col4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, -HelfSize, 0), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 0.0f, 1.0f, 1.0f)) },
		};
		
		//�C���f�b�N�X�z��
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		
		//���b�V���̍쐬
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		///���[�g�V�O�l�`���쐬
		CreateRootSignature();
		
		///�f�X�N�v���^�q�[�v�쐬
		CreateDescriptorHeap();
		
		///�R���X�^���g�o�b�t�@�쐬
		CreateConstantBuffer();
		
		///�p�C�v���C���X�e�[�g�쐬
		CreatePipelineState();
		
		///�R�}���h���X�g�쐬
		CreateCommandList();
		
		///�R���X�^���g�o�b�t�@�̍X�V
		UpdateConstantBuffer();
	}

	void SquareSprite::OnUpdate() {
		if (m_PosSpan.x > 0) {
			m_Rot -= 0.05f;
		}
		else {
			m_Rot += 0.05f;
		}
		m_Pos += m_PosSpan;
		if (abs(m_Pos.x) > 400.0f) {
			m_PosSpan *= -1.0f;
		}
	}

	void SquareSprite::OnDraw() {
		
		//�R���X�^���g�o�b�t�@�̍X�V
		UpdateConstantBuffer();
		
		//�`��
		DrawObject();
	}
	
}
//end basecross
