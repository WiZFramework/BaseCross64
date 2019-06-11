/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
@copyright shike.y
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//�O�p�`�I�u�W�F�N�g
	class TriangleObject : public ObjectInterface, public ShapeInterface
	{

	public:
		TriangleObject();
		virtual ~TriangleObject();

		//������
		virtual void OnCreate() override;

		//�X�V
		void OnUpdate() override;

		//�`��X�V
		void OnDraw() override;

	private:
		
		//���[�g�V�O�l�`���̍쐬
		void CreateRootSingnature();

		//�p�C�v���C���X�e�[�g�̍쐬
		void CreatePipelineState();

		//�R�}���h���X�g�̍쐬
		void CreateCommandList();

		//�`�揈��
		void DrawObject();

		//���b�V��
		shared_ptr<MeshResource> m_TriangleMesh;

		//���[�g�V�O�l�`��
		ComPtr<ID3D12RootSignature> m_RootSignature;

		//�p�C�v���C���X�e�[�g
		ComPtr<ID3D12PipelineState> m_PipelineState;

		//�R�}���h���C��
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
	};
}
//end basecross
