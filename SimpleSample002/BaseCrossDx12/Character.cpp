/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
@copyright shike.y
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//�O�p�I�u�W�F�N�g
	TriangleObject::TriangleObject() :
		ObjectInterface(),
		ShapeInterface(),
		m_Pos(0.f, 0.f, 0.f),
		m_MoveSpan(0.01f, 0.f, 0.f)
	{
	}

	TriangleObject::~TriangleObject()
	{
	}

	//������
	void TriangleObject::OnCreate()
	{
		//���_�f�[�^���烁�b�V�����\�[�X���쐬
		vector<VertexPositionColor> vertices = 
		{
			{ Vec3(0.0f, 0.5f , 0.0f),Col4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ Vec3(0.5f, -0.5f, 0.5f),Col4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ Vec3(-0.5f, -0.5f, 0.5f),Col4(0.0f, 0.0f, 1.0f, 1.0f) }
		};

		//���_���W����O�p�`�̃��b�V�����쐬
		/*���_�� X0 Y0�Ƃ��Ē��_3�ӏ�����L�̔z��ō쐬
				.
			   / \
			  /   \
			 /     \
			.______ \.
		*/
		m_TriangleMesh = MeshResource::CreateMeshResource(vertices, false);

		//���[�g�V�O�l�`���̍쐬
		CreateRootSingnature();

		//�f�X�N���v�^�q�[�v�̍쐬
		CreateDescriptorHeap();

		//�R���X�^���g�o�b�t�@�̍쐬
		CreateConstantBuffer();

		//�p�C�v���C���X�e�[�g�̍쐬
		CreatePipelineState();

		//�R�}���h���X�g�̍쐬
		CreateCommandList();

		//�R���X�^���g�o�b�t�@�̍X�V
		UpdateConstantBuffer();
	}

	//�X�V
	void TriangleObject::OnUpdate()
	{
		m_Pos += m_MoveSpan;
		if (abs(m_Pos.x) >= 0.5f)
		{
			m_MoveSpan *= -1.0f;
		}
	}

	//�`��X�V
	void TriangleObject::OnDraw()
	{
		//�R���X�^���g�o�b�t�@�̍X�V
		UpdateConstantBuffer();
		//m_TriangleMesh�̕`��
		DrawObject();
	}

	//���[�g�V�O�l�`���̍쐬
	void TriangleObject::CreateRootSingnature()
	{
		//��ԃV���v���ȃ��[�g�V�O�l�`���̍쐬
		m_RootSignature = RootSignature::CreateCbv();
	}

	//�f�X�N���v�^�q�[�v�̍쐬
	void TriangleObject::CreateDescriptorHeap()
	{
		auto Device = App::GetApp()->GetDeviceResources();

		m_CbvSrvDescriptorHandleIncrementSize
			= Device->GetD3DDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//CbvSrv�f�X�N���v�^�q�[�v(�R���X�^���g�o�b�t�@�̂�)
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);

		//GPU���f�X�N���v�^�q�[�v�̃n���h���z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}

	//�R���X�^���g�o�b�t�@�̍쐬
	void TriangleObject::CreateConstantBuffer()
	{
		auto Device = App::GetApp()->GetDeviceResources();

		ThrowIfFailed(Device->GetD3DDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SpriteConstantBuffer) + 255) & ~255),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Device->GetDevice()->CreateCommittedResource()",
			L"TriangleObject::CreateConstantBuffer()"
		);
		
		//�R���X�^���g�o�b�t�@�r���[�̍쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();

		//�R���X�^���g�o�b�t�@��255�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = (sizeof(SpriteConstantBuffer) + 255) & ~255;

		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N���v�^�q�[�v��̃n���h�����擾
		//�V�F�[�_�[���\�[�X������ꍇ�A�R���X�^���g�o�b�t�@�̓V�F�[�_�[���\�[�X�r���[�̂��Ƃɔz�u����
		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
			m_CbvSrvUavDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		Device->GetD3DDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);

		//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
		CD3DX12_RANGE readRange(0, 0);
		ThrowIfFailed(m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
			L"�R���X�^���g�o�b�t�@�̃}�b�v�쐬�Ɏ��s���܂���",
			L"m_ConstantBufferUploadHeap->Map",
			L"TriangleObject::CreateConstantBuffer()"
		);
	}

	//�p�C�v���C���X�e�[�g�̍쐬
	void TriangleObject::CreatePipelineState()
	{
		//�p�C�v���C���X�e�[�g�̒�`
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		m_PipelineState 
			= PipelineState::CreateDefault2D<VertexPositionColor, VSPCSprite, PSPCSprite>(m_RootSignature, PipeLineDesc);
	}

	//�R�}���h���X�g�̍쐬
	void TriangleObject::CreateCommandList()
	{
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	//�`�揈��
	void TriangleObject::DrawObject()
	{
		auto Device = App::GetApp()->GetDeviceResources();

		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);

		//���b�V���̍X�V������΁A���\�[�X�̍X�V
		m_TriangleMesh->UpdateResources<VertexPositionColor>(m_CommandList);

		//���[�g�V�O�l�`���̃Z�b�g
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());

		//�f�X�N���v�^�q�[�v�̃Z�b�g
		ID3D12DescriptorHeap* ppHeap[] = { m_CbvSrvUavDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);
		
		//GPU�f�X�N���v�^�q�[�v�ɃZ�b�g
		for (UINT i = 0; i < m_GPUDescriptorHandleVec.size(); i++)
		{
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}

		m_CommandList->RSSetViewports(1, &Device->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Device->GetScissorRect());

		//�����_�[�^�[�Q�b�g�r���[�n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Device->GetRtvHandle();

		//�f�v�X�X�e���V���r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Device->GetDsvHandle();
		//�擾�����n���h�����Z�b�g
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetVertexBuffers(0, 1, &m_TriangleMesh->GetVertexBufferView());
		m_CommandList->DrawInstanced(m_TriangleMesh->GetNumVertices(), 1, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);

		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Device->InsertDrawCommandLists(m_CommandList.Get());
	}

	//�R���X�^���g�o�b�t�@�̍X�V
	void TriangleObject::UpdateConstantBuffer()
	{
		//�R���X�^���g�o�b�t�@�̏���
		m_SpriteConstantBuffer.Emissive = Col4(0.f, 0.f, 0.f, 1.f);

		Mat4x4 mat;
		mat.translation(m_Pos);
		m_SpriteConstantBuffer.World = mat;

		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_SpriteConstantBuffer), sizeof(m_SpriteConstantBuffer));
	}
}
//end basecross
