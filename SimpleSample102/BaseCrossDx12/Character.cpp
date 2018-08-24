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
	CubeObject::CubeObject() :
		ObjectInterface(),
		ShapeInterface()
	{}
	CubeObject::~CubeObject() {}

	void CubeObject::CreateBuffers() {
		float HelfSize = 0.5f;
		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(Vec3(-HelfSize, HelfSize, -HelfSize), Col4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, HelfSize, -HelfSize), Col4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, -HelfSize, -HelfSize), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, -HelfSize, -HelfSize), Col4(1.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, HelfSize, HelfSize), Col4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, HelfSize, HelfSize), Col4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(HelfSize, -HelfSize, HelfSize), Col4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vec3(-HelfSize, -HelfSize, HelfSize), Col4(1.0f, 0.0f, 1.0f, 1.0f)) }
		};
		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
			1, 4, 3,
			4, 6, 3,
			4, 5, 6,
			5, 7, 6,
			5, 0, 7,
			0, 2, 7,
			5, 4, 0,
			4, 1, 0,
			6, 7, 3,
			7, 2, 3
		};
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_CubeMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}


	///���[�g�V�O�l�`���쐬
	void CubeObject::CreateRootSignature() {
		//�R���X�^���g�o�b�t�@�t�����[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateCbv();
	}

	///�f�X�N�v���^�q�[�v�쐬
	void CubeObject::CreateDescriptorHeap() {
		auto Device = App::GetApp()->GetDeviceResources();

		m_CbvSrvDescriptorHandleIncrementSize
			= Device->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		//CbvSrv�f�X�N�v���^�q�[�v(�R���X�^���g�o�b�t�@�̂�)
		m_CbvSrvUavDescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1);

		//GPU���f�X�N�v���^�q�[�v�̃n���h���z��̍쐬
		m_GPUDescriptorHandleVec.clear();
		CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
			m_CbvSrvUavDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		m_GPUDescriptorHandleVec.push_back(CbvHandle);
	}

	///�R���X�^���g�o�b�t�@�쐬
	void CubeObject::CreateConstantBuffer() {
		auto Device = App::GetApp()->GetDeviceResources();

		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		UINT ConstBufferSize = (sizeof(StaticConstantBuffer) + 255) & ~255;

		ThrowIfFailed(Device->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(ConstBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_ConstantBufferUploadHeap)),
			L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
			L"Device->GetDevice()->CreateCommittedResource()",
			L"CubeObject::CreateConstantBuffer()"
		);
		
		//�R���X�^���g�o�b�t�@�̃r���[���쐬
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};

		cbvDesc.BufferLocation = m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
		//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
		cbvDesc.SizeInBytes = ConstBufferSize;

		//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
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
			L"CubeObject::CreateConstantBuffer()"
		);


	}
	///�p�C�v���C���X�e�[�g�쐬
	void CubeObject::CreatePipelineState() {
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PineLineDesc;
		m_PipelineState = PipelineState::CreateDefault3D<VertexPositionColor, VSPCStatic, PSPCStatic>(m_RootSignature, PineLineDesc);
	}

	///�R�}���h���X�g�쐬
	void CubeObject::CreateCommandList() {
		m_CommandList = CommandList::CreateDefault(m_PipelineState);
		CommandList::Close(m_CommandList);
	}

	///�R���X�^���g�o�b�t�@�̍X�V
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

		m_StaticConstantBuffer.World = World;
		m_StaticConstantBuffer.View = View;
		m_StaticConstantBuffer.Projection = Proj;
		m_StaticConstantBuffer.Emissive = Col4(0, 0, 0, 0);

		//�X�V
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_StaticConstantBuffer),
			sizeof(m_StaticConstantBuffer));
	}

	///�`�揈��
	void CubeObject::DrawObject() {
		auto Device = App::GetApp()->GetDeviceResources();

		//�R�}���h���X�g�̃��Z�b�g
		CommandList::Reset(m_PipelineState, m_CommandList);

		//���b�V�����X�V����Ă���΃��\�[�X�X�V
		m_CubeMesh->UpdateResources<VertexPositionColor>(m_CommandList);

		//���[�g�V�O�l�`���̃Z�b�g
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());

		//�f�X�N�v���^�q�[�v�̃Z�b�g
		ID3D12DescriptorHeap* ppHeaps[] = { m_CbvSrvUavDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		//GPU�f�X�N�v���^�q�[�v�n���h���̃Z�b�g
		for (UINT i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable(i, m_GPUDescriptorHandleVec[i]);
		}

		m_CommandList->RSSetViewports(1, &Device->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Device->GetScissorRect());

		//�����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle = Device->GetRtvHandle();

		//�f�v�X�X�e���V���r���[�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle = Device->GetDsvHandle();

		//�擾�����n���h�����Z�b�g
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetVertexBuffers(0, 1, &m_CubeMesh->GetVertexBufferView());

		//�C���f�b�N�X�o�b�t�@���Z�b�g
		m_CommandList->IASetIndexBuffer(&m_CubeMesh->GetIndexBufferView());

		//�C���f�b�N�X�`��
		m_CommandList->DrawIndexedInstanced(m_CubeMesh->GetNumIndicis(), 1, 0, 0, 0);

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);

		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Device->InsertDrawCommandLists(m_CommandList.Get());
	}


	void CubeObject::OnCreate() {
		CreateBuffers();
		m_Scale = Vec3(1.0f, 1.0f, 1.0f);
		m_Qt.identity();
		m_Pos = Vec3(0, 0, 0.0);
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
