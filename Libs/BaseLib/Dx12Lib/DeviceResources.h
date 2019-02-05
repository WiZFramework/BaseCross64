/*!
@file DeviceResources.h
@brief �e�N�X�`���A���b�V���ADx12�f�o�C�X���\�[�X
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {

#define BASECROSS_DXVERSION 12

	//--------------------------------------------------------------------------------------
	/// ��������ɕێ�����Dx12�e�N�X�`�����\�[�X
	//--------------------------------------------------------------------------------------
	class TextureResource : public BaseResource {
		friend class ObjectFactory;
		void OnCreateWithParam(uint32_t Width, uint32_t Height, const vector<uint8_t>& data);
		void OnCreateWithParam(const wstring& FileName, const wstring& TexType);
		TextureResource();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TextureResource();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ��������Ƀe�N�X�`�����\�[�X�̍쐬
		@param[in]	Width ��
		@param[in]	Height ����
		@return	�e�N�X�`�����\�[�X��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		static  shared_ptr<TextureResource> CreateTextureResource(uint32_t Width, uint32_t Height, const vector<uint8_t>& data);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �摜�t�@�C������e�N�X�`�����\�[�X�̍쐬
		@param[in]	FileName �t�@�C����
		@param[in]	TexType �摜�^�C�v
		@return	�e�N�X�`�����\�[�X��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		static  shared_ptr<TextureResource> CreateTextureResource(const wstring& FileName, const wstring& TexType = L"WIC");
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���̕��𓾂�
		@return	�e�N�X�`���̕�
		*/
		//--------------------------------------------------------------------------------------
		uint32_t GetTextureWidth();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���̍����𓾂�
		@return	�e�N�X�`���̍���
		*/
		//--------------------------------------------------------------------------------------
		uint32_t GetTextureHeight();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �s�N�Z�����𓾂�
		@return	�s�N�Z����
		*/
		//--------------------------------------------------------------------------------------
		uint32_t GetPixelSize();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���̃��\�[�X�f�X�N�v���^�𓾂�
		@return	�e�N�X�`���̃��\�[�X�f�X�N�v���^
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_RESOURCE_DESC& GetTextureResDesc() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���̃��\�[�X�𓾂�
		@return	�e�N�X�`���̃��\�[�X
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Resource> GetTexture() const;
		//�e�N�X�`���f�[�^
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���f�[�^�𓾂�
		@return	�e�N�X�`���f�[�^�̔z��
		*/
		//--------------------------------------------------------------------------------------
		vector<uint8_t>& GetData() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���f�[�^��ݒ肷��
		@param[in]	data �f�[�^�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetData(const vector<uint8_t>& data);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���f�[�^��ݒ肷��(SetData�֐��Ɠ���)
		@param[in]	data �f�[�^�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateTexture(const vector<uint8_t>& data);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���\�[�X���X�V����
		@param[in]	commandList �R�}���h���X�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateResources(const ComPtr<ID3D12GraphicsCommandList>& commandList);
		//���z�֐��i��֐��j
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	���_���Ƃ̃X�L�j���O���
	//--------------------------------------------------------------------------------------
	struct SkinPrimData {
		uint32_t indices[4];
		float weights[4];
	};

	//--------------------------------------------------------------------------------------
	///	��̃��b�V���f�[�^
	//--------------------------------------------------------------------------------------
	struct MeshPrimData {
		//���_�o�b�t�@
		ComPtr<ID3D12Resource> m_VertexBuffer;
		//�C���f�b�N�X�o�b�t�@
		ComPtr<ID3D12Resource> m_IndexBuffer;
		//���_�̐�
		UINT m_NumVertices;
		//�C���f�b�N�X�̐�
		UINT m_NumIndicis;
		//���̃��b�V���̌`�C���f�b�N�X
		type_index m_MeshTypeIndex;
		//�X�g���C�h��
		UINT m_NumStride;
		//�`��g�|���W�[
		D3D12_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
		//�o�b�N�A�b�v�f�[�^
		shared_ptr<BackupDataBase> m_BackUpData;
		//���b�V���Ƃƃ^���X�t�H�[���̍����s��i���b�V���P�ʂŐݒ肷��ꍇ�j
		bsm::Mat4x4 m_MeshToTransformMatrix;
		//���b�V���̍����s���ݒ肷�邩�ǂ���
		bool m_UseMeshToTransformMatrix;
		//�e�N�X�`�����\�[�X(���b�V���P�ʂŐݒ肷��ꍇ)
		weak_ptr<TextureResource> m_TextureResource;
		//�}�e���A���̔z��i���f���Ŏg�p�j
		vector<MaterialEx> m_MaterialExVec;
		//�ȉ��A�{�[���p
		//�{�[�����ǂ���
		bool m_IsSkining;
		//�{�[���̐�
		UINT m_BoneCount;
		//�T���v�����O��
		UINT m_SampleCount;
		//�T���v�����O���ꂽ�{�[���s��
		vector<bsm::Mat4x4> m_SampleMatrixVec;
		//�}���`���b�V���̏ꍇ�̃��b�V���C���f�b�N�X
		UINT m_MultiMeshIndex;
		//�ėp�Ɏg���郍�[�J�����_�̔z��
		vector<VertexPosition> m_Vertices;
		//�X�L�j���O���
		vector<SkinPrimData> m_Skins;
		MeshPrimData() :
			m_IsSkining(false),
			m_BoneCount(0),
			m_SampleCount(0),
			m_MeshTypeIndex(typeid(VertexPosition)),	//�֋X��VertexPosition�ɏ�����
			m_NumStride(sizeof(VertexPosition)),
			m_PrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
			m_MeshToTransformMatrix(),
			m_UseMeshToTransformMatrix(false),
			m_MultiMeshIndex(0)
		{}
	};



	//--------------------------------------------------------------------------------------
	/// Dx12���b�V���N���X
	//--------------------------------------------------------------------------------------
	class MeshResource : public BaseResource {
		friend class ObjectFactory;
		ComPtr<ID3D12Resource> m_VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D12Resource> m_IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		ComPtr<ID3D12Resource> m_VertexBufferUploadHeap;
		ComPtr<ID3D12Resource> m_IndexBufferUploadHeap;
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
		UINT m_NumVertices;				//���_�̐�
		UINT m_NumIndicis;				//�C���f�b�N�X��
		bool m_AccessWrite;
		bool m_DataRefresh;
		shared_ptr<BackupDataBase> m_BackupData;
		MeshResource() :
			m_VertexBufferView{ 0 },
			m_IndexBufferView{ 0 },
			m_NumVertices(0),
			m_NumIndicis(0),
			m_AccessWrite(false),
			m_DataRefresh(true)
		{}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MeshResource() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C���f�b�N�X���܂ރ��b�V�����\�[�X�̍쐬
		@tparam	T ���_�̌^
		@param[in]	vertices ���_�̔z��
		@param[in]	indices �C���f�b�N�X�̔z��
		@param[in]	AccessWrite ���_�ύX�\���ǂ���
		@return	���b�V�����\�[�X��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, const vector<uint16_t>& indices, bool AccessWrite) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			Ptr->m_AccessWrite = AccessWrite;
			//�f�[�^�̃o�b�N�A�b�v

			auto DataPtr = make_shared< BackupData<T> >();
			DataPtr->m_Vertices.clear();
			for (auto v : vertices) {
				DataPtr->m_Vertices.push_back(v);
			}
			DataPtr->m_Indices.clear();
			for (auto i : indices) {
				DataPtr->m_Indices.push_back(i);
			}
			Ptr->m_BackupData = DataPtr;
			UINT vertexBufferSize = (UINT)(sizeof(T) * vertices.size());
			//���_�o�b�t�@�̍쐬
			{
				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_VertexBuffer)),
					L"���_�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);

				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_VertexBufferUploadHeap)),
					L"���_�A�b�v���[�h�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);
				// Initialize the vertex buffer view.
				Ptr->m_VertexBufferView.BufferLocation = Ptr->m_VertexBuffer->GetGPUVirtualAddress();
				Ptr->m_VertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(T));
				Ptr->m_VertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(T) * vertices.size());
			}
			//���_���̐ݒ�
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			//�C���f�b�N�X�o�b�t�@�̍쐬
			UINT indexBufferSize = static_cast<UINT>(sizeof(uint16_t) * indices.size());
			{
				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_IndexBuffer)),
					L"�C���f�b�N�X�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);

				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_IndexBufferUploadHeap)),
					L"�C���f�b�N�X�A�b�v���[�h�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);

				// Describe the index buffer view.
				Ptr->m_IndexBufferView.BufferLocation = Ptr->m_IndexBuffer->GetGPUVirtualAddress();
				Ptr->m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
				Ptr->m_IndexBufferView.SizeInBytes = indexBufferSize;

			}
			//�C���f�b�N�X���̐ݒ�
			Ptr->m_NumIndicis = static_cast<UINT>(indices.size());
			return Ptr;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief �C���f�b�N�X���܂܂Ȃ����b�V�����\�[�X�̍쐬
		@tparam	T ���_�̌^
		@param[in]	vertices ���_�̔z��
		@param[in]	AccessWrite ���_�ύX�\���ǂ���
		@return	���b�V�����\�[�X��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, bool AccessWrite) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			Ptr->m_AccessWrite = AccessWrite;

			//�f�[�^�̃o�b�N�A�b�v

			auto DataPtr = make_shared< BackupData<T> >();
			DataPtr->m_Vertices.clear();
			for (auto v : vertices) {
				DataPtr->m_Vertices.push_back(v);
			}
			Ptr->m_BackupData = DataPtr;
			UINT vertexBufferSize = (UINT)(sizeof(T) * vertices.size());
			// Create the vertex buffer.
			{
				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_VertexBuffer)),
					L"���_�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);

				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_VertexBufferUploadHeap)),
					L"���_�A�b�v���[�h�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);
				// Initialize the vertex buffer view.
				Ptr->m_VertexBufferView.BufferLocation = Ptr->m_VertexBuffer->GetGPUVirtualAddress();
				Ptr->m_VertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(T));
				Ptr->m_VertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(T) * vertices.size());
			}
			//���_���̐ݒ�
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			return Ptr;
		}

		//�v���~�e�B�u���\�[�X�\�z
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 4�p�`���ʂ̍쐬
		@param[in]	size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateSquare(float Size, bool AccessWrite = false) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSquare(Size, vertices, indices);
			return CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@breaf �L���[�u�i�����́j�̍쐬
		@param[in]	size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCube(float Size, bool AccessWrite = false) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateCube(Size, vertices, indices);
			return CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���̂̍쐬
		@param[in]	Diameter		���a
		@param[in]	Tessellation	������
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite = false) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSphere(Diameter, Tessellation, vertices, indices);
			return CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �J�v�Z���̍쐬
		@param[in]	Diameter		���a
		@param[in]	Height,		����
		@param[in]	Tessellation	������
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			bsm::Vec3 PointA(0, -Height / 2.0f, 0);
			bsm::Vec3 PointB(0, Height / 2.0f, 0);
			//Capsule�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateCapsule(Diameter, PointA, PointB, Tessellation, vertices, indices);
			return CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �V�����_�[�̍쐬
		@param[in]	Height,		����
		@param[in]	Diameter		���a
		@param[in]	Tessellation	������
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cylinder�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateCylinder(Height, Diameter, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �R�[���̍쐬
		@param[in]	Diameter		���a
		@param[in]	Height,		����
		@param[in]	Tessellation	������
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cone�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateCone(Diameter, Height, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �g�[���X�̍쐬
		@param[in]	Diameter		���a
		@param[in]	Thickness		�h�[�i�b�c�̑���
		@param[in]	Tessellation	������
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Torus�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateTorus(Diameter, Thickness, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ��4�ʑ̂̍쐬
		@param[in]	Size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTetrahedron(float Size, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Tetrahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateTetrahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ��8�ʑ̂̍쐬
		@param[in]	Size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateOctahedron(float Size, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Octahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateOctahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ��12�ʑ̂̍쐬
		@param[in]	Size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateDodecahedron(float Size, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Dodecahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateDodecahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ��20�ʑ̂̍쐬
		@param[in]	Size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateIcosahedron(float Size, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Icosahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateIcosahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//������
		static shared_ptr<MeshResource> CreateBoneModelMeshWithTangent(const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false) {
			return nullptr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���_�o�b�t�@�̎擾
		@return	���_�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const ComPtr<ID3D12Resource>& GetVertexBuffer()const {
			return m_VertexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���_�o�b�t�@�r���[�̎擾
		@return	���_�o�b�t�@�r���[�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const {
			return m_VertexBufferView;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���_���̎擾
		@return	���_��
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumVertices() const {
			return m_NumVertices;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �C���f�b�N�X�o�b�t�@�̎擾
		@return	�C���f�b�N�X�o�b�t�@�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const ComPtr<ID3D12Resource>& GetIndexBuffer()const {
			return m_IndexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �C���f�b�N�X�o�b�t�@�r���[�̎擾
		@return	�C���f�b�N�X�o�b�t�@�r���[�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const {
			return m_IndexBufferView;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �C���f�b�N�X���̎擾
		@return	�C���f�b�N�X��
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumIndicis() const {
			return m_NumIndicis;
		}

		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief  ���_�̍X�V
		@tparam	T	���_�̌^
		@param[in] vertices	���_�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void UpdateVirtex(const vector<T>& vertices) {
			if (!m_AccessWrite) {
				ThrowBaseException(
					L"���̃��b�V���̒��_�ύX�͋�����Ă��܂���",
					L"if (!m_AccessWrite)",
					L"MeshResource::UpdateVirtex()"
				);
			}
			auto shptr = dynamic_pointer_cast< BackupData<T> >(m_BackupData);
			if (!shptr) {
				return;
			}
			if (shptr->m_Vertices.size() < vertices.size()) {
				return;
			}
			for (size_t i = 0; i < vertices.size();i++) {
				shptr->m_Vertices[i] = vertices[i];
			}
			m_DataRefresh = true;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief  ���\�[�X�̍X�V
		@tparam	T	���_�̌^
		@param[in] commandList	�R�}���h���X�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void UpdateResources(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			if (!m_DataRefresh) {
				return;
			}
			auto shptr = dynamic_pointer_cast< BackupData<T> >(m_BackupData);
			if (!shptr) {
				return;
			}
			//���_�o�b�t�@�̍X�V
			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData = (void*)&shptr->m_Vertices[0];
			vertexData.RowPitch = shptr->m_Vertices.size() * sizeof(T);
			vertexData.SlicePitch = vertexData.RowPitch;
			UpdateSubresources<1>(commandList.Get(), m_VertexBuffer.Get(), m_VertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
			//�C���f�b�N�X�o�b�t�@�̍X�V
			if (shptr->m_Indices.size() > 0) {
				D3D12_SUBRESOURCE_DATA indexData = {};
				indexData.pData = (void*)&shptr->m_Indices[0];
				indexData.RowPitch = shptr->m_Indices.size() * sizeof(uint16_t);
				indexData.SlicePitch = indexData.RowPitch;
				UpdateSubresources<1>(commandList.Get(), m_IndexBuffer.Get(), m_IndexBufferUploadHeap.Get(), 0, 0, 1, &indexData);
				commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_IndexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));
			}
			m_DataRefresh = false;
		}
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	};


	//--------------------------------------------------------------------------------------
	///	�}���`���b�V�����\�[�X
	//--------------------------------------------------------------------------------------
	class MultiMeshResource : public BaseResource {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�R���X�g���N�^<br />
		�\�z�̓X�^�e�B�b�N�֐��𗘗p����
		*/
		//--------------------------------------------------------------------------------------
		MultiMeshResource() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MultiMeshResource() {}
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	};



	//�ėp�I�Ȑݒ�p��`
	//--------------------------------------------------------------------------------------
	///	�u�����h�X�e�[�g
	//--------------------------------------------------------------------------------------
	enum class BlendState {
		Opaque,
		AlphaBlend,
		Additive,
		NonPremultiplied,
	};

	//--------------------------------------------------------------------------------------
	///	�f�v�X�X�e���V���X�e�[�g
	//--------------------------------------------------------------------------------------
	enum class DepthStencilState {
		None,
		Default,
		Read,
	};

	//--------------------------------------------------------------------------------------
	///	���X�^���C�U�X�e�[�g
	//--------------------------------------------------------------------------------------
	enum class RasterizerState {
		CullNone,
		CullFront,
		CullBack,
		Wireframe,
		DoubleDraw,	//�w�ʕ`��̌�A�O�ʕ`��
	};

	//--------------------------------------------------------------------------------------
	///	�T���v���[�X�e�[�g
	//--------------------------------------------------------------------------------------
	enum class SamplerState {
		SamplerNone,
		PointWrap,
		PointClamp,
		LinearWrap,
		LinearClamp,
		AnisotropicWrap,
		AnisotropicClamp,
		ComparisonLinear,
	};


	class ShadowMapRenderTarget;

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	/*!
	DirectX12�f�o�C�X�N���X
	*/
	//--------------------------------------------------------------------------------------
	class DeviceResources {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �R���X�g���N�^
		@param[in] hWnd	�E�C���h�E�̃n���h��
		@param[in] isFullScreen �t���X�N���[�����ǂ���
		@param[in] Width		��
		@param[in] Height		����
		*/
		//--------------------------------------------------------------------------------------
		DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DeviceResources();
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �t���[�����𓾂�
		@return	�t���[����
		*/
		//--------------------------------------------------------------------------------------
		UINT GetFrameCount() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �t���[���C���f�b�N�X�𓾂�
		@return	�t���[���C���f�b�N�X
		*/
		//--------------------------------------------------------------------------------------
		UINT GetFrameIndex() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �X���b�v�`�F�C���𓾂�
		@return	�X���b�v�`�F�C��
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDXGISwapChain3> GetSwapChain() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �f�o�C�X�𓾂�
		@return	�f�o�C�X
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Device> GetDevice() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g�̍쐬
		@param[in] ShadowMapDimension	�V���h�E�}�b�v�̕��y�э���
		@return	�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ShadowMapRenderTarget> CreateShadowMapRenderTarget(float ShadowMapDimension = 2048.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g�̎擾
		@return	�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ShadowMapRenderTarget> GetShadowMapRenderTarget(float ShadowMapDimension = 2048.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �����_�����O�^�[�Q�b�g�i�̃|�C���^�j�𓾂�
		@return	�����_�����O�^�[�Q�b�g�i�̃|�C���^�j
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Resource>* GetRenderTargets() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �w�肵���C���f�b�N�X�̃����_�����O�^�[�Q�b�g�𓾂�
		@return	�w�肵���C���f�b�N�X�̃����_�����O�^�[�Q�b�g
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Resource> GetRenderTarget(UINT Index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R�}���h�A���P�[�^�𓾂�
		@return	�R�}���h�A���P�[�^
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R�}���h�L���[�𓾂�
		@return	�R�}���h�L���[
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �����_�����O�^�[�Q�b�g�r���[�̃q�[�v�̈�𓾂�
		@return	�����_�����O�^�[�Q�b�g�r���[�̃q�[�v�̈�
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12DescriptorHeap> GetRtvHeap() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�v�X�X�e���V���r���[�̃q�[�v�̈�𓾂�
		@return	�f�v�X�X�e���V���r���[�̃q�[�v�̈�
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12DescriptorHeap> GetDsvHeap() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �����_�����O�^�[�Q�b�g�r���[�̃f�X�N�v���^�T�C�Y�𓾂�
		@return	�����_�����O�^�[�Q�b�g�r���[�̃f�X�N�v���^�T�C�Y
		*/
		//--------------------------------------------------------------------------------------
		UINT GetRtvDescriptorSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �r���[�|�[�g�𓾂�
		@return	�r���[�|�[�g
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_VIEWPORT& GetViewport() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V�U�[�̈�𓾂�
		@return	�V�U�[�̈�
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_RECT& GetScissorRect() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R�}���h���X�g��ǉ�����
		@param[in] Tgt	�R�}���h���X�g�̃|�C���^�i���|�C���^�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void InsertDrawCommandLists(ID3D12CommandList* Tgt);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R�}���h���X�g�����s���t�����g�o�b�t�@�ւ̓]�����s��
		@param[in] SyncInterval	�C���^�[�o��
		@param[in] Flags	�t���O
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Present(unsigned int SyncInterval, unsigned int  Flags);


		//--------------------------------------------------------------------------------------
		/*!
		@brief ���܂��Ă���R�}���h���X�g�����s���ăR�}���h���X�g�̃o�b�t�@���N���A����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ExecuteCommandLists();


		//--------------------------------------------------------------------------------------
		/*!
		@brief 1�O�̃t���[���̏�����҂�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void WaitForPreviousFrame();

		// D2D �A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	D2D1Factory2�C���^�[�t�F�C�X�̎擾
		@return	D2D1Factory2�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		ID2D1Factory2* GetD2DFactory() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	D2D1Device1�C���^�[�t�F�C�X�̎擾
		@return	D2D1Device1�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		ID2D1Device1* GetD2DDevice() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	D2D1DeviceContext1�C���^�[�t�F�C�X�̎擾
		@return	D2D1DeviceContext1�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		ID2D1DeviceContext1* GetD2DDeviceContext() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	DWriteFactory2�C���^�[�t�F�C�X�̎擾
		@return	DWriteFactory2�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		IDWriteFactory2* GetDWriteFactory() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	WICImagingFactory2�C���^�[�t�F�C�X�̎擾
		@return	WICImagingFactory2�C���^�[�t�F�C�X
		*/
		//--------------------------------------------------------------------------------------
		IWICImagingFactory2* GetWicImagingFactory() const;



		//���z�֐�
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V���h�E�}�b�v�̃N���A�i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearShadowmapViews();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V���h�E�}�b�v�`��̊J�n
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartShadowmapDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V���h�E�}�b�v�`��̏I��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndShadowmapDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʏ�`��̃N���A�i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearDefaultViews(const bsm::Col4& col = bsm::Col4(0, 0, 0, 1.0f));
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʏ�`��̊J�n�i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartDefaultDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʏ�`��̏I���i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndDefaultDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �����_�����O�^�[�Q�b�g�ނ̏����i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void InitRenderTargets() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���e���c�̍쐬��̏���
		@param[in]	ShadowActive	�e���L�����ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterInitContents(bool ShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �L�[�Ɋ��蓖�Ă�ꂽ���[�g�V�O�l�`���𓾂�
		@param[in] Key	�L�[
		@return	������΃��[�g�V�O�l�`���B�Ȃ����nurrptr
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12RootSignature> GetRootSignature(const wstring& Key);

		//--------------------------------------------------------------------------------------
		/*!
		@brief �L�[�Ɋ��蓖�Ă�ꂽ���[�g�V�O�l�`����ݒ肷��
		@param[in] Key	�L�[
		@param[in] rootsig	�ݒ肷�郋�[�g�V�O�l�`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetRootSignature(const wstring& Key,const ComPtr<ID3D12RootSignature>& rootsig);

		//--------------------------------------------------------------------------------------
		/*!
		@brief �����_�[�^�[�Q�b�g�r���[�̃n���h���𓾂�
		@return	�����_�[�^�[�Q�b�g�r���[�̃n���h��
		*/
		//--------------------------------------------------------------------------------------
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtvHandle() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�v�X�X�e���V���r���[�̃n���h���𓾂�
		@return	�f�v�X�X�e���V���r���[�̃n���h��
		*/
		//--------------------------------------------------------------------------------------
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() const;


	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	�����_�[�^�[�Q�b�g���N���X
	//--------------------------------------------------------------------------------------
	class RenderTarget {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		RenderTarget();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RenderTarget();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����_�����O�^�[�Q�b�g���N���A���鏃�����z�֐�
		@param[in]	col	�N���A�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearViews(const bsm::Col4& col = bsm::Col4(0, 0, 0, 1.0f)) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����_�����O�^�[�Q�b�g���J�n���鏃�����z�֐�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartRenderTarget() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����_�����O�^�[�Q�b�g���I�����鏃�����z�֐�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndRenderTarget() = 0;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;
		//���[�u�֎~
		RenderTarget(const RenderTarget&&) = delete;
		RenderTarget& operator=(const RenderTarget&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	///	�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g
	//--------------------------------------------------------------------------------------
	class ShadowMapRenderTarget : public RenderTarget {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	ShadowMapDimension	�V���h�E�}�b�v�̑傫��
		*/
		//--------------------------------------------------------------------------------------
		ShadowMapRenderTarget(float ShadowMapDimension);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShadowMapRenderTarget();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�V���h�E�}�b�v�̑傫���𓾂�
		@return	�V���h�E�}�b�v�̑傫��
		*/
		//--------------------------------------------------------------------------------------
		float GetShadowMapDimension() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����_�����O�^�[�Q�b�g���N���A���鉼�z�֐�
		@param[in]	col	�N���A�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearViews(const bsm::Col4& col = bsm::Col4(0, 0, 0, 1.0f)) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����_�����O�^�[�Q�b�g���J�n����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartRenderTarget()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����_�����O�^�[�Q�b�g���I������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndRenderTarget()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�v�X�X�e���V���r���[�̃n���h���𓾂�
		@return	�f�v�X�X�e���V���r���[�̃n���h��
		*/
		//--------------------------------------------------------------------------------------
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�v�X�X�e���V���𓾂�
		@return	�f�v�X�X�e���V��
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Resource> GetDepthStencil() const;

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};






	//--------------------------------------------------------------------------------------
	/// �f�X�N�v���^�q�[�v���[�e�B���e�B
	//--------------------------------------------------------------------------------------
	namespace DescriptorHeap {
		static inline ComPtr<ID3D12DescriptorHeap> CreateDirect(const D3D12_DESCRIPTOR_HEAP_DESC& desc) {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12DescriptorHeap> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&Ret)),
				L"�f�X�N�v���^�q�[�v�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateDescriptorHeap()",
				L"DescriptorHeap::CreateDirect()"
			);
			return Ret;
		}
		static inline ComPtr<ID3D12DescriptorHeap> CreateCbvSrvUavHeap(UINT NumDescriptorHeap) {
			//CbvSrv�f�X�N�v���^�q�[�v
			D3D12_DESCRIPTOR_HEAP_DESC CbvSrvHeapDesc = {};
			CbvSrvHeapDesc.NumDescriptors = NumDescriptorHeap;
			CbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			CbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			return CreateDirect(CbvSrvHeapDesc);
		}
		static inline ComPtr<ID3D12DescriptorHeap> CreateSamplerHeap(UINT NumDescriptorHeap) {
			//�T���v���[�f�X�N�v���^�q�[�v
			D3D12_DESCRIPTOR_HEAP_DESC SamplerHeapDesc = {};
			SamplerHeapDesc.NumDescriptors = NumDescriptorHeap;
			SamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			SamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			return CreateDirect(SamplerHeapDesc);
		}

	}

	//--------------------------------------------------------------------------------------
	/// ���[�g�V�O�l�`�����[�e�B���e�B
	//--------------------------------------------------------------------------------------
	namespace RootSignature {
		static inline ComPtr<ID3D12RootSignature> CreateDirect(const D3D12_ROOT_SIGNATURE_DESC& desc) {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;
			ComPtr<ID3D12RootSignature> Ret;
			ThrowIfFailed(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error),
				L"���[�g�V�O�l�`���̃V���A���C�Y�Ɏ��s���܂���",
				L"D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error)",
				L"RootSignature::Create()"
			);
			ThrowIfFailed(
				Dev->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(),
					signature->GetBufferSize(), IID_PPV_ARGS(&Ret)),
				L"���[�g�V�O�l�`���̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateRootSignature()",
				L"RootSignature::Create()"
			);
			return Ret;
		}
		//��ԃV���v���ȃ��[�g�V�O�l�`��
		static inline ComPtr<ID3D12RootSignature> CreateSimple() {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12RootSignature> Ret = Dev->GetRootSignature(L"Simple");
			if (Ret != nullptr) {
				return Ret;
			}

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			Ret = CreateDirect(rootSignatureDesc);
			Dev->SetRootSignature(L"Simple", Ret);
			return Ret;
		}
		//�R���X�^���g�o�b�t�@�̂�
		static inline ComPtr<ID3D12RootSignature> CreateCbv() {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12RootSignature> Ret = Dev->GetRootSignature(L"Cbv");
			if (Ret != nullptr) {
				return Ret;
			}

			CD3DX12_DESCRIPTOR_RANGE ranges[1];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
			CD3DX12_ROOT_PARAMETER rootParameters[1];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_ALL);

			D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);

			Ret = CreateDirect(rootSignatureDesc);
			Dev->SetRootSignature(L"Cbv", Ret);
			return Ret;
		}
		//�V�F�[�_���\�[�X�ƃT���v���[
		static inline ComPtr<ID3D12RootSignature> CreateSrvSmp() {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12RootSignature> Ret = Dev->GetRootSignature(L"SrvSmp");
			if (Ret != nullptr) {
				return Ret;
			}

			CD3DX12_DESCRIPTOR_RANGE ranges[2];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[2];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			Ret = CreateDirect(rootSignatureDesc);
			Dev->SetRootSignature(L"SrvSmp", Ret);
			return Ret;
		}

		//�V�F�[�_���\�[�X�ƃT���v���[�ƃR���X�^���g�o�b�t�@
		static inline ComPtr<ID3D12RootSignature> CreateSrvSmpCbv() {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12RootSignature> Ret = Dev->GetRootSignature(L"SrvSmpCbv");
			if (Ret != nullptr) {
				return Ret;
			}


			CD3DX12_DESCRIPTOR_RANGE ranges[3];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
			ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[3];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_ALL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			Ret = CreateDirect(rootSignatureDesc);
			Dev->SetRootSignature(L"SrvSmpCbv", Ret);
			return Ret;
		}
		//�V�F�[�_���\�[�X2�ƃT���v���[2�ƃR���X�^���g�o�b�t�@1��
		static inline ComPtr<ID3D12RootSignature> CreateSrv2Smp2Cbv() {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12RootSignature> Ret = Dev->GetRootSignature(L"Srv2Smp2Cbv");
			if (Ret != nullptr) {
				return Ret;
			}


			CD3DX12_DESCRIPTOR_RANGE ranges[5];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
			ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
			ranges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 1);
			ranges[4].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[5];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[3].InitAsDescriptorTable(1, &ranges[3], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[4].InitAsDescriptorTable(1, &ranges[4], D3D12_SHADER_VISIBILITY_ALL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			Ret = CreateDirect(rootSignatureDesc);
			Dev->SetRootSignature(L"Srv2Smp2Cbv", Ret);
			return Ret;
		}


	}

	//--------------------------------------------------------------------------------------
	/// �T���v���[���[�e�B���e�B�i�ύX�\�j
	//--------------------------------------------------------------------------------------
	namespace DynamicSampler {
		static inline void CreateSampler(const SamplerState State, D3D12_CPU_DESCRIPTOR_HANDLE& Handle) {
			D3D12_SAMPLER_DESC samplerDesc = {};
			//�f�t�H���g�����Ă���
			samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

			switch (State) {
			case SamplerState::PointWrap:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::PointClamp:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::LinearWrap:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::LinearClamp:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::AnisotropicWrap:
				samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::AnisotropicClamp:
				samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::ComparisonLinear:
				samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.BorderColor[0] = 1.0f;
				samplerDesc.BorderColor[1] = 1.0f;
				samplerDesc.BorderColor[2] = 1.0f;
				samplerDesc.BorderColor[3] = 1.0f;
				samplerDesc.MinLOD = 0;
				samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
				samplerDesc.MipLODBias = 0.0f;
				samplerDesc.MaxAnisotropy = 0;
				samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

				break;
			}

			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->GetDevice()->CreateSampler(&samplerDesc, Handle);
		}
	}


	//--------------------------------------------------------------------------------------
	/// �p�C�v���C���X�e�[�g���[�e�B���e�B
	//--------------------------------------------------------------------------------------
	namespace PipelineState {
		static inline ComPtr<ID3D12PipelineState> CreateDirect(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12PipelineState> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&Ret)),
				L"�p�C�v���C���X�e�[�g�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateGraphicsPipelineState()",
				L"PipelineState::CreateDirect()"
			);
			return Ret;
		}
		template<typename Vertex, typename VS, typename PS>
		static inline ComPtr<ID3D12PipelineState> CreateDefault2D(const ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc) {

			ZeroMemory(&RetDesc, sizeof(RetDesc));
			RetDesc.InputLayout = { Vertex::GetVertexElement(), Vertex::GetNumElements() };
			RetDesc.pRootSignature = rootSignature.Get();

			RetDesc.VS = { reinterpret_cast<UINT8*>(
				VS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				VS::GetPtr()->GetShaderComPtr()->GetBufferSize() };
			RetDesc.PS = { reinterpret_cast<UINT8*>(
				PS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				PS::GetPtr()->GetShaderComPtr()->GetBufferSize() };
			RetDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			RetDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			RetDesc.DepthStencilState.DepthEnable = FALSE;
			RetDesc.DepthStencilState.StencilEnable = FALSE;
			RetDesc.SampleMask = UINT_MAX;
			RetDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			RetDesc.NumRenderTargets = 1;
			RetDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			RetDesc.SampleDesc.Count = 1;
			return CreateDirect(RetDesc);
		}
		template<typename Vertex, typename VS, typename PS>
		static inline ComPtr<ID3D12PipelineState> CreateDefault3D(const ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc) {

			CD3DX12_RASTERIZER_DESC rasterizerStateDesc(D3D12_DEFAULT);
			//���ʃJ�����O
			rasterizerStateDesc.CullMode = D3D12_CULL_MODE_NONE;

			ZeroMemory(&RetDesc, sizeof(RetDesc));
			RetDesc.InputLayout = { Vertex::GetVertexElement(), Vertex::GetNumElements() };
			RetDesc.pRootSignature = rootSignature.Get();
			RetDesc.VS =
			{
				reinterpret_cast<UINT8*>(VS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				VS::GetPtr()->GetShaderComPtr()->GetBufferSize()
			};
			RetDesc.PS =
			{
				reinterpret_cast<UINT8*>(PS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				PS::GetPtr()->GetShaderComPtr()->GetBufferSize()
			};
			RetDesc.RasterizerState = rasterizerStateDesc;
			RetDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			RetDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			RetDesc.SampleMask = UINT_MAX;
			RetDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			RetDesc.NumRenderTargets = 1;
			RetDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			RetDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			RetDesc.SampleDesc.Count = 1;
			return CreateDirect(RetDesc);
		}

		template<typename Vertex, typename VS>
		static inline ComPtr<ID3D12PipelineState> CreateShadowmap3D(const ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc) {

			CD3DX12_RASTERIZER_DESC rasterizerStateDesc(D3D12_DEFAULT);
			//�\�ʃJ�����O
			rasterizerStateDesc.CullMode = D3D12_CULL_MODE_FRONT;
			rasterizerStateDesc.FillMode = D3D12_FILL_MODE_SOLID;
			rasterizerStateDesc.DepthClipEnable = TRUE;

			ZeroMemory(&RetDesc, sizeof(RetDesc));
			RetDesc.InputLayout = { Vertex::GetVertexElement(), Vertex::GetNumElements() };
			RetDesc.pRootSignature = rootSignature.Get();
			RetDesc.VS =
			{
				reinterpret_cast<UINT8*>(VS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				VS::GetPtr()->GetShaderComPtr()->GetBufferSize()
			};

			RetDesc.PS = CD3DX12_SHADER_BYTECODE(0, 0);
			RetDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
			RetDesc.NumRenderTargets = 0;

			RetDesc.RasterizerState = rasterizerStateDesc;
			RetDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			RetDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			RetDesc.SampleMask = UINT_MAX;
			RetDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			RetDesc.NumRenderTargets = 0;
			RetDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
			RetDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			RetDesc.SampleDesc.Count = 1;
			return CreateDirect(RetDesc);
		}

	}


	//--------------------------------------------------------------------------------------
	/// �R�}���h���X�g���[�e�B���e�B
	//--------------------------------------------------------------------------------------
	namespace CommandList {
		static inline  ComPtr<ID3D12GraphicsCommandList> CreateSimple() {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12GraphicsCommandList> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				Dev->GetCommandAllocator().Get(),
				nullptr,
				IID_PPV_ARGS(&Ret)),
				L"�R�}���h���X�g�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommandList()",
				L"CommandList::CreateSimple()"
			);
			return Ret;
		}

		static inline  ComPtr<ID3D12GraphicsCommandList> CreateDefault(const ComPtr<ID3D12PipelineState>& pipelineState) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12GraphicsCommandList> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				Dev->GetCommandAllocator().Get(),
				pipelineState.Get(),
				IID_PPV_ARGS(&Ret)),
				L"�R�}���h���X�g�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommandList()",
				L"CommandList::CreateDefault()"
			);
			return Ret;
		}
		static inline  ComPtr<ID3D12GraphicsCommandList> CreateCopy(const ComPtr<ID3D12PipelineState>& pipelineState) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12GraphicsCommandList> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_COPY,
				Dev->GetCommandAllocator().Get(),
				pipelineState.Get(),
				IID_PPV_ARGS(&Ret)),
				L"�R�}���h���X�g�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommandList()",
				L"CommandList::CreateCopy()"
			);
			return Ret;
		}

		static inline  void Reset(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ThrowIfFailed(commandList->Reset(Dev->GetCommandAllocator().Get(), nullptr),
				L"�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂���",
				L"commandList->Reset(Dev->GetCommandAllocator().Get(),nullptr)",
				L"CommandList::Reset()"
			);

		}
		static inline  void Reset(const ComPtr<ID3D12PipelineState>& pipelineState, const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ThrowIfFailed(commandList->Reset(Dev->GetCommandAllocator().Get(), pipelineState.Get()),
				L"�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂���",
				L"commandList->Reset(Dev->GetCommandAllocator().Get(),pipelineState.Get())",
				L"CommandList::Reset()"
			);

		}
		static inline  void Close(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			ThrowIfFailed(commandList->Close(),
				L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
				L"commandList->Close()",
				L"CommandList::Close()"
			);

		}


	}

	//--------------------------------------------------------------------------------------
	///	�V�F�[�_�֘A���\�[�X�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class Dx12ShaderResource {
	protected:
		Dx12ShaderResource();
		virtual ~Dx12ShaderResource();
		//�V�F�[�_�A�N�Z�T
		ID3DBlob* GetShaderBlob(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr);
		ComPtr<ID3DBlob>& GetShaderBlobComPtr(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr);
	private:
		//�R�s�[�֎~
		Dx12ShaderResource(const Dx12ShaderResource&) = delete;
		Dx12ShaderResource& operator=(const Dx12ShaderResource&) = delete;
		//���[�u�֎~
		Dx12ShaderResource(const Dx12ShaderResource&&) = delete;
		Dx12ShaderResource& operator=(const Dx12ShaderResource&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	///	�V���O���g���V�F�[�_�̐e�N���X(�V�F�[�_�̎�ނɂ�����炸�����N���X�g�p)
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class Dx12Shader : public Dx12ShaderResource {
		wstring m_Filename;
	protected:
		//�f���[�^�[
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3DBlob> m_ShaderPtr;
		//�\�z�Ɣj��
		Dx12Shader<ShaderType>(const wstring& Filename) : Dx12ShaderResource(), m_Filename(Filename) {}
		virtual ~Dx12Shader() {}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//�V�F�[�_���J�A�N�Z�b�T
		ID3DBlob* GetShader() {
			return GetShaderBlob(m_Filename, m_ShaderPtr);
		}
		ComPtr<ID3DBlob>& GetShaderComPtr() {
			return GetShaderBlobComPtr(m_Filename, m_ShaderPtr);
		}
		//�C���X�^���X�擾
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};


	//������ʃV�F�[�_�[
	//�V�F�[�_�錾�p�}�N��
#define DECLARE_DX12SHADER(ShaderName) class ShaderName : public Dx12Shader<ShaderName>{ \
	public: \
		ShaderName(); \
	};
	//�V�F�[�_���̗p�}�N��
#define IMPLEMENT_DX12SHADER(ShaderName,CsoFilename) unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; \
	ShaderName::ShaderName() : \
	Dx12Shader(CsoFilename){}





}
//end basecross
