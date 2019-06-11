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
		ShapeInterface()
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

		//�p�C�v���C���X�e�[�g�̍쐬
		CreatePipelineState();

		//�R�}���h���X�g�̍쐬
		CreateCommandList();
	}

	//�X�V
	void TriangleObject::OnUpdate()
	{
		//�Ȃɂ��Ȃ�
	}

	//�`��X�V
	void TriangleObject::OnDraw()
	{
		//m_TriangleMesh�̕`��
		DrawObject();
	}

	//���[�g�V�O�l�`���̍쐬
	void TriangleObject::CreateRootSingnature()
	{
		//��ԃV���v���ȃ��[�g�V�O�l�`���̍쐬
		m_RootSignature = RootSignature::CreateSimple();
	}

	//�p�C�v���C���X�e�[�g�̍쐬
	void TriangleObject::CreatePipelineState()
	{
		//�p�C�v���C���X�e�[�g�̒�`
		D3D12_GRAPHICS_PIPELINE_STATE_DESC PipeLineDesc;
		m_PipelineState 
			= PipelineState::CreateDefault2D<VertexPositionColor, VSPCDirect, PSPCDirect>(m_RootSignature, PipeLineDesc);
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

		//�R�}���h���X�g�̐ݒ�
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		m_CommandList->RSSetViewports(1, &Device->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Device->GetScissorRect());

		//�����_�����O�^�[�Q�b�g�r���[�̃n���h�����擾
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
}
//end basecross
