/*!
@file DeviceResources.cpp
@brief �e�N�X�`���A���b�V���ADx11�f�o�C�X���\�[�X����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct TextureResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct TextureResource::Impl {
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;	//���\�[�X�r���[
		wstring m_FileName;		//�e�N�X�`���t�@�C���ւ̃p�X

		Impl(const wstring& FileName, const wstring& TexType = L"WIC");
		~Impl() {}
		//�~���[�e�b�N�X
		std::mutex Mutex;

	};

	TextureResource::Impl::Impl(const wstring& FileName, const wstring& TexType) :
		m_FileName(FileName) {
		try {
			if (FileName == L"") {
				throw BaseException(
					L"�t�@�C�����w�肳��Ă��܂���",
					L"if(FileName == L\"\")",
					L"Texture::Impl::Impl()"
				);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(m_FileName.c_str());
			if (RetCode == -1) {
				wstring patherr = m_FileName;
				throw BaseException(
					L"�t�@�C�������݂��܂���",
					patherr.c_str(),
					L"Texture::Impl::Impl()"
				);
			}
			HRESULT hr;
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

				_wsplitpath_s(m_FileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);

				wstring ExtStr = Extbuff;

				if (ExtStr == L".dds" || ExtStr == L".DDS") {
					ThrowIfFailed(
						DirectX::LoadFromDDSFile(m_FileName.c_str(), 0, &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						m_FileName,
						L"Texture::Impl::Impl()"
					);
				}
				else if (ExtStr == L".tga" || ExtStr == L".TGA") {
					ThrowIfFailed(
						DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						m_FileName,
						L"Texture::Impl::Impl()"
					);
				}
				else {
					ThrowIfFailed(
						DirectX::LoadFromWICFile(m_FileName.c_str(), 0, &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						m_FileName,
						L"Texture::Impl::Impl()"
					);
				}
			}
			else if (TexType == L"DDS") {
				ThrowIfFailed(
					DirectX::LoadFromDDSFile(m_FileName.c_str(), 0, &metadata, image),
					L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
					m_FileName,
					L"Texture::Impl::Impl()"
				);
			}
			else if (TexType == L"TGA") {
				ThrowIfFailed(
					DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
					L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
					m_FileName,
					L"Texture::Impl::Impl()"
				);
			}
			else {
				throw BaseException(
					L"���̃^�C�v�͑Ή����Ă��܂���",
					TexType.c_str(),
					L"Texture::Impl::Impl()"
				);
			}

			//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();

			Util::DemandCreate(m_ShaderResView, Mutex, [&](ID3D11ShaderResourceView** pResult) -> HRESULT
			{
				// �摜����V�F�[�_���\�[�XView�̍쐬
				hr = ThrowIfFailed(CreateShaderResourceView(pDx11Device, image.GetImages(), image.GetImageCount(), metadata, pResult),
					L"�V�F�[�_�[���\�[�X�r���[���쐬�ł��܂���",
					L"if( FAILED( CreateShaderResourceView() ) )",
					L"Texture::Impl::Impl()"
				);
				return hr;
			});


		}
		catch (...) {
			throw;
		}
	}



	//--------------------------------------------------------------------------------------
	//	class TextureResource : public BaseResource;
	//	�p�r: �e�N�X�`���̃��b�s���O�N���X
	//--------------------------------------------------------------------------------------

	shared_ptr<TextureResource> TextureResource::CreateTextureResource(const wstring& FileName, const wstring& TexType) {
		try {
			return ObjectFactory::Create<TextureResource>(FileName, TexType);
		}
		catch (...) {
			throw;
		}
	}

	TextureResource::TextureResource(const wstring& FileName, const wstring& TexType) :
		BaseResource(),
		pImpl(new Impl(FileName, TexType))
	{
	}
	TextureResource::~TextureResource() {
	}

	//�A�N�Z�b�T
	ComPtr<ID3D11ShaderResourceView>& TextureResource::GetShaderResourceView() const {
		return pImpl->m_ShaderResView;
	}
	const wstring& TextureResource::GetTextureFileName() const {
		return pImpl->m_FileName;
	}



	//--------------------------------------------------------------------------------------
	///	���b�V�����\�[�X
	//--------------------------------------------------------------------------------------
	//�\�z
	MeshResource::MeshResource() :
		BaseResource()
	{}
	//�j��
	MeshResource::~MeshResource() {}

	//���\�[�X�\�z

	shared_ptr<MeshResource> MeshResource::CreateSquare(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Square�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateSquare(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}
	shared_ptr<MeshResource> MeshResource::CreateCube(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cube�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateCube(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}
	shared_ptr<MeshResource> MeshResource::CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Sphere�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateSphere(Diameter, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			bsm::Vec3 PointA(0, -Height / 2.0f, 0);
			bsm::Vec3 PointB(0, Height / 2.0f, 0);
			//Capsule�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateCapsule(Diameter, PointA, PointB, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}


	}



	shared_ptr<MeshResource> MeshResource::CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cylinder�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateCylinder(Height, Diameter, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cone�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateCone(Diameter, Height, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Torus�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateTorus(Diameter, Thickness, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}

	}

	shared_ptr<MeshResource> MeshResource::CreateTetrahedron(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Tetrahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateTetrahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	//��8�ʑ�
	shared_ptr<MeshResource> MeshResource::CreateOctahedron(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Octahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateOctahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateDodecahedron(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Dodecahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateDodecahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateIcosahedron(float Size, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Icosahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateIcosahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	void MeshResource::ReadBaseData(BinaryReader& Reader, const wstring& BinDataDir, const wstring& BinDataFile,
		vector<VertexPositionNormalTexture>& vertices, vector<VertexPositionNormalTangentTexture>& vertices_withtan,
		vector<uint16_t>& indices, vector<MaterialEx>& materials) {
		vertices.clear();
		vertices_withtan.clear();
		indices.clear();
		materials.clear();
		//���_�̓ǂݍ���
		auto blockHeader = Reader.Read<BlockHeader>();
		if (!(blockHeader.m_Type == BlockType::Vertex || blockHeader.m_Type == BlockType::VertexWithTangent)) {
			throw BaseException(
				L"���_�̃w�b�_���Ⴂ�܂�",
				BinDataFile,
				L"MeshResource::ReadBaseData()"
			);
		}
		if (blockHeader.m_Type == BlockType::Vertex) {
			auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTexturePOD);
			auto pVertex = Reader.ReadArray<VertexPositionNormalTexturePOD>((size_t)VerTexSize);
			for (UINT i = 0; i < VerTexSize; i++) {
				VertexPositionNormalTexture v;
				v.position.x = pVertex[i].position[0];
				v.position.y = pVertex[i].position[1];
				v.position.z = pVertex[i].position[2];
				v.normal.x = pVertex[i].normal[0];
				v.normal.y = pVertex[i].normal[1];
				v.normal.z = pVertex[i].normal[2];
				v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
				v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
				vertices.push_back(v);
			}
		}
		else if (blockHeader.m_Type == BlockType::VertexWithTangent) {
			auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTangentTexturePOD);
			auto pVertex = Reader.ReadArray<VertexPositionNormalTangentTexturePOD>((size_t)VerTexSize);
			for (UINT i = 0; i < VerTexSize; i++) {
				VertexPositionNormalTangentTexture v;
				v.position.x = pVertex[i].position[0];
				v.position.y = pVertex[i].position[1];
				v.position.z = pVertex[i].position[2];
				v.normal.x = pVertex[i].normal[0];
				v.normal.y = pVertex[i].normal[1];
				v.normal.z = pVertex[i].normal[2];
				v.tangent.x = pVertex[i].tangent[0];
				v.tangent.y = pVertex[i].tangent[1];
				v.tangent.z = pVertex[i].tangent[2];
				v.tangent.w = pVertex[i].tangent[3];
				v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
				v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
				vertices_withtan.push_back(v);
			}
		}
		else {
			throw BaseException(
				L"���_�̌^���Ⴂ�܂�",
				BinDataFile,
				L"MeshResource::ReadBaseData()"
			);
		}

		//�C���f�b�N�X�̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::Index) {
			throw BaseException(
				L"�C���f�b�N�X�̃w�b�_���Ⴂ�܂�",
				BinDataFile,
				L"MeshResource::ReadBaseData()"
			);
		}

		auto IndexSize = blockHeader.m_Size / sizeof(uint16_t);
		auto pIndex = Reader.ReadArray<uint16_t>((size_t)IndexSize);
		for (UINT i = 0; i < IndexSize; i++) {
			indices.push_back(pIndex[i]);
		}

		//�}�e���A���̓ǂݍ���
		//�}�e���A�����̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::MaterialCount) {
			throw BaseException(
				L"�}�e���A�����̃w�b�_���Ⴂ�܂�",
				BinDataFile,
				L"MeshResource::ReadBaseData()"
			);
		}
		UINT MaterialCount = blockHeader.m_Size;
		for (UINT i = 0; i < MaterialCount; i++) {
			//�e�N�X�`���t�@�C�������ϒ��Ȃ̂Œ��ӁB
			blockHeader = Reader.Read<BlockHeader>();
			if (blockHeader.m_Type != BlockType::Material) {
				throw BaseException(
					L"�}�e���A���̃w�b�_���Ⴂ�܂�",
					BinDataFile,
					L"MeshResource::ReadBaseData()"
				);
			}
			UINT TextureFileNameSize = blockHeader.m_Size - sizeof(MaterialExPOD);
			auto rMaterial = Reader.Read<MaterialExPOD>();
			MaterialEx ToM;
			//!�J�n�C���f�b�N�X
			ToM.m_StartIndex = rMaterial.m_StartIndex;
			//!�`��C���f�b�N�X�J�E���g
			ToM.m_IndexCount = rMaterial.m_IndexCount;
			//! �f�t�B�[�Y�i���̂̐F�j
			ToM.m_Diffuse.x = rMaterial.m_Diffuse[0];
			ToM.m_Diffuse.y = rMaterial.m_Diffuse[1];
			ToM.m_Diffuse.z = rMaterial.m_Diffuse[2];
			ToM.m_Diffuse.w = rMaterial.m_Diffuse[3];
			//! �X�y�L�����[�i���ˌ��j
			ToM.m_Specular.x = rMaterial.m_Specular[0];
			ToM.m_Specular.y = rMaterial.m_Specular[1];
			ToM.m_Specular.z = rMaterial.m_Specular[2];
			ToM.m_Specular.w = rMaterial.m_Specular[3];
			//! �A���r�G���g�i���F�j
			ToM.m_Ambient.x = rMaterial.m_Ambient[0];
			ToM.m_Ambient.y = rMaterial.m_Ambient[1];
			ToM.m_Ambient.z = rMaterial.m_Ambient[2];
			ToM.m_Ambient.w = rMaterial.m_Ambient[3];
			//! �G�~�b�V�u�i���ˌ��j
			ToM.m_Emissive.x = rMaterial.m_Emissive[0];
			ToM.m_Emissive.y = rMaterial.m_Emissive[1];
			ToM.m_Emissive.z = rMaterial.m_Emissive[2];
			ToM.m_Emissive.w = rMaterial.m_Emissive[3];
			auto pTexture = Reader.ReadArray<wchar_t>(TextureFileNameSize / sizeof(wchar_t));
			wstring TextureFileStr = pTexture;
			if (TextureFileStr != L"") {
				TextureFileStr = BinDataDir + TextureFileStr;
				ToM.m_TextureResource = ObjectFactory::Create<TextureResource>(TextureFileStr);

			}
			else {
				ToM.m_TextureResource = nullptr;
			}
			materials.push_back(ToM);
		}

		//End�̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::End) {
			throw BaseException(
				L"End�w�b�_���Ⴂ�܂�",
				BinDataFile,
				L"MeshResource::ReadBaseData()"
			);
		}

	}
	void MeshResource::ReadBaseBoneData(BinaryReader& Reader, const wstring& BinDataDir, const wstring& BinDataFile,
		vector<VertexPositionNormalTextureSkinning>& vertices, vector<VertexPositionNormalTangentTextureSkinning>& vertices_withtan,
		vector<uint16_t>& indices, vector<MaterialEx>& materials,
		vector<bsm::Mat4x4>& bonematrix, UINT& BoneCount, UINT& SampleCount) {

		vertices.clear();
		vertices_withtan.clear();
		indices.clear();
		materials.clear();
		bonematrix.clear();

		//���_�̓ǂݍ���
		auto blockHeader = Reader.Read<BlockHeader>();
		if (!(blockHeader.m_Type == BlockType::SkinedVertex || blockHeader.m_Type == BlockType::SkinedVertexWithTangent)) {
			throw BaseException(
				L"���_(�X�L������)�̃w�b�_���Ⴂ�܂�",
				BinDataFile,
				L"MeshResource::ReadBaseBoneData()"
			);
		}
		if (blockHeader.m_Type == BlockType::SkinedVertex) {
			auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTextureSkinningPOD);
			auto pVertex = Reader.ReadArray<VertexPositionNormalTextureSkinningPOD>((size_t)VerTexSize);
			for (UINT i = 0; i < VerTexSize; i++) {
				VertexPositionNormalTextureSkinning v;
				v.position.x = pVertex[i].position[0];
				v.position.y = pVertex[i].position[1];
				v.position.z = pVertex[i].position[2];
				v.normal.x = pVertex[i].normal[0];
				v.normal.y = pVertex[i].normal[1];
				v.normal.z = pVertex[i].normal[2];
				v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
				v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
				for (int j = 0; j < 4; j++) {
					v.indices[j] = pVertex[i].indices[j];
					v.weights[j] = pVertex[i].weights[j];
				}
				vertices.push_back(v);
			}
		}
		else if (blockHeader.m_Type == BlockType::SkinedVertexWithTangent) {
			auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTangentTextureSkinningPOD);
			auto pVertex = Reader.ReadArray<VertexPositionNormalTangentTextureSkinningPOD>((size_t)VerTexSize);
			for (UINT i = 0; i < VerTexSize; i++) {
				VertexPositionNormalTangentTextureSkinning v;
				v.position.x = pVertex[i].position[0];
				v.position.y = pVertex[i].position[1];
				v.position.z = pVertex[i].position[2];
				v.normal.x = pVertex[i].normal[0];
				v.normal.y = pVertex[i].normal[1];
				v.normal.z = pVertex[i].normal[2];
				v.tangent.x = pVertex[i].tangent[0];
				v.tangent.y = pVertex[i].tangent[1];
				v.tangent.z = pVertex[i].tangent[2];
				v.tangent.w = pVertex[i].tangent[3];
				v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
				v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
				for (int j = 0; j < 4; j++) {
					v.indices[j] = pVertex[i].indices[j];
					v.weights[j] = pVertex[i].weights[j];
				}
				vertices_withtan.push_back(v);
			}

		}

		//�C���f�b�N�X�̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::Index) {
			throw BaseException(
				L"�C���f�b�N�X�̃w�b�_���Ⴂ�܂�",
				BinDataFile,
				L"MeshResource::ReadBaseBoneData()"
			);
		}

		auto IndexSize = blockHeader.m_Size / sizeof(uint16_t);
		auto pIndex = Reader.ReadArray<uint16_t>((size_t)IndexSize);
		for (UINT i = 0; i < IndexSize; i++) {
			indices.push_back(pIndex[i]);
		}

		//�}�e���A���̓ǂݍ���
		//�}�e���A�����̓ǂݍ���
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::MaterialCount) {
			throw BaseException(
				L"�}�e���A�����̃w�b�_���Ⴂ�܂�",
				BinDataFile,
				L"MeshResource::ReadBaseData()"
			);
		}
		UINT MaterialCount = blockHeader.m_Size;
		for (UINT i = 0; i < MaterialCount; i++) {
			//�e�N�X�`���t�@�C�������ϒ��Ȃ̂Œ��ӁB
			blockHeader = Reader.Read<BlockHeader>();
			if (blockHeader.m_Type != BlockType::Material) {
				throw BaseException(
					L"�}�e���A���̃w�b�_���Ⴂ�܂�",
					BinDataFile,
					L"MeshResource::ReadBaseBoneData()"
				);
			}
			UINT TextureFileNameSize = blockHeader.m_Size - sizeof(MaterialExPOD);
			auto rMaterial = Reader.Read<MaterialExPOD>();
			MaterialEx ToM;
			//!�J�n�C���f�b�N�X
			ToM.m_StartIndex = rMaterial.m_StartIndex;
			//!�`��C���f�b�N�X�J�E���g
			ToM.m_IndexCount = rMaterial.m_IndexCount;
			//! �f�t�B�[�Y�i���̂̐F�j
			ToM.m_Diffuse.x = rMaterial.m_Diffuse[0];
			ToM.m_Diffuse.y = rMaterial.m_Diffuse[1];
			ToM.m_Diffuse.z = rMaterial.m_Diffuse[2];
			ToM.m_Diffuse.w = rMaterial.m_Diffuse[3];
			//! �X�y�L�����[�i���ˌ��j
			ToM.m_Specular.x = rMaterial.m_Specular[0];
			ToM.m_Specular.y = rMaterial.m_Specular[1];
			ToM.m_Specular.z = rMaterial.m_Specular[2];
			ToM.m_Specular.w = rMaterial.m_Specular[3];
			//! �A���r�G���g�i���F�j
			ToM.m_Ambient.x = rMaterial.m_Ambient[0];
			ToM.m_Ambient.y = rMaterial.m_Ambient[1];
			ToM.m_Ambient.z = rMaterial.m_Ambient[2];
			ToM.m_Ambient.w = rMaterial.m_Ambient[3];
			//! �G�~�b�V�u�i���ˌ��j
			ToM.m_Emissive.x = rMaterial.m_Emissive[0];
			ToM.m_Emissive.y = rMaterial.m_Emissive[1];
			ToM.m_Emissive.z = rMaterial.m_Emissive[2];
			ToM.m_Emissive.w = rMaterial.m_Emissive[3];
			auto pTexture = Reader.ReadArray<wchar_t>(TextureFileNameSize / sizeof(wchar_t));
			wstring TextureFileStr = pTexture;
			if (TextureFileStr != L"") {
				TextureFileStr = BinDataDir + TextureFileStr;
				ToM.m_TextureResource = ObjectFactory::Create<TextureResource>(TextureFileStr);
			}
			else {
				ToM.m_TextureResource = nullptr;
			}
			materials.push_back(ToM);
		}

		//�{�[����
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::BoneCount) {
			throw BaseException(
				L"�{�[�����̃w�b�_���Ⴂ�܂�",
				BinDataFile,
				L"MeshResource::ReadBaseBoneData()"
			);
		}
		BoneCount = blockHeader.m_Size;
		//�{�[���A�j���[�V�����s��
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::AnimeMatrix) {
			throw BaseException(
				L"�A�j���[�V�����s��̃w�b�_���Ⴂ�܂�",
				BinDataFile,
				L"MeshResource::ReadBaseBoneData()"
			);
		}
		auto MatrixSize = blockHeader.m_Size / sizeof(MatrixPOD);
		auto pAnimeMatrix = Reader.ReadArray<MatrixPOD>((size_t)MatrixSize);
		for (UINT i = 0; i < MatrixSize; i++) {
			//�{�[���P�ʂł͂Ȃ��s��P�ʂœǂݍ���
			bsm::Mat4x4 mat;
			for (int u = 0; u < 4; u++) {
				for (int v = 0; v < 4; v++) {
					mat(u, v) = pAnimeMatrix->m_Mat[u][v];
				}
			}
			bonematrix.push_back(mat);
			pAnimeMatrix++;
		}
		SampleCount = (UINT)(MatrixSize / BoneCount);

		//End
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::End) {
			throw BaseException(
				L"�I���w�b�_���Ⴂ�܂�",
				BinDataFile,
				L"MeshResource::ReadBaseBoneData()"
			);
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateStaticModelMeshBase(BinaryReader& Reader, const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�^���W�F���g�t���ɃR���o�[�g����z��(�_�~�[)
			vector<VertexPositionNormalTangentTexture> new_pntnt_vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//�}�e���A����ݒ肷��z��
			vector<MaterialEx> Materials;
			ReadBaseData(Reader, BinDataDir, BinDataFile, vertices, new_pntnt_vertices, indices, Materials);
			auto Ptr = MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
			Ptr->m_MeshPrimData.m_MaterialExVec.clear();
			for (auto& v : Materials) {
				Ptr->m_MeshPrimData.m_MaterialExVec.push_back(v);
			}
			return Ptr;
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateStaticModelMeshWithTangentBase(BinaryReader& Reader, const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�^���W�F���g�t���ɃR���o�[�g����z��
			vector<VertexPositionNormalTangentTexture> new_pntnt_vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//�}�e���A����ݒ肷��z��
			vector<MaterialEx> Materials;
			ReadBaseData(Reader, BinDataDir, BinDataFile, vertices, new_pntnt_vertices, indices, Materials);
			if (vertices.size() > 0) {
				//bin�f�[�^�ɂ̓^���W�F���g�͓����ĂȂ�����
				new_pntnt_vertices.clear();
				for (size_t i = 0; i < vertices.size(); i++) {
					VertexPositionNormalTangentTexture new_pntnt_v;

					new_pntnt_v.position = vertices[i].position;
					new_pntnt_v.normal = vertices[i].normal;
					new_pntnt_v.textureCoordinate = vertices[i].textureCoordinate;
					bsm::Vec3 n = bsm::cross((bsm::Vec3)new_pntnt_v.normal, bsm::Vec3(0, 1, 0));
					new_pntnt_v.tangent = XMFLOAT4(0, 0, 0, 0);
					new_pntnt_vertices.push_back(new_pntnt_v);
				}
				MeshUtill::SetNormalTangent(new_pntnt_vertices);
			}
			auto Ptr = MeshResource::CreateMeshResource<VertexPositionNormalTangentTexture>(new_pntnt_vertices, indices, AccessWrite);
			Ptr->m_MeshPrimData.m_MaterialExVec.clear();
			for (auto& v : Materials) {
				Ptr->m_MeshPrimData.m_MaterialExVec.push_back(v);
			}
			return Ptr;
		}
		catch (...) {
			throw;
		}
	}



	shared_ptr<MeshResource> MeshResource::CreateBoneModelMeshBase(BinaryReader& Reader, const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTextureSkinning> vertices;
			//�^���W�F���g�t���ɃR���o�[�g����z��(�_�~�[)
			vector<VertexPositionNormalTangentTextureSkinning> new_pntnt_vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//�}�e���A����ݒ肷��z��
			vector<MaterialEx> Materials;
			//�T���v�����O�s��
			vector<bsm::Mat4x4> SampleMatrices;
			//�{�[����
			UINT BoneCount;
			//�T���v����
			UINT SampleCount;
			ReadBaseBoneData(Reader, BinDataDir, BinDataFile, vertices, new_pntnt_vertices,
				indices, Materials,
				SampleMatrices, BoneCount, SampleCount);
			auto Ptr = MeshResource::CreateMeshResource<VertexPositionNormalTextureSkinning>(vertices, indices, AccessWrite);
			Ptr->GetMaterialExVec().clear();
			for (auto& v : Materials) {
				Ptr->GetMaterialExVec().push_back(v);
			}
			Ptr->m_MeshPrimData.m_SampleMatrixVec.clear();
			for (auto& v : SampleMatrices) {
				Ptr->m_MeshPrimData.m_SampleMatrixVec.push_back(v);
			}
			Ptr->m_MeshPrimData.m_BoneCount = BoneCount;
			Ptr->m_MeshPrimData.m_SampleCount = SampleCount;
			Ptr->m_MeshPrimData.m_IsSkining = true;
			return Ptr;
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateBoneModelMeshWithTangentBase(BinaryReader& Reader, const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite) {
		try {
			//���_�z��
			vector<VertexPositionNormalTextureSkinning> vertices;
			//�^���W�F���g�t���ɃR���o�[�g����z��
			vector<VertexPositionNormalTangentTextureSkinning> new_pntnt_vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//�}�e���A����ݒ肷��z��
			vector<MaterialEx> Materials;
			//�T���v�����O�s��
			vector<bsm::Mat4x4> SampleMatrices;
			//�{�[����
			UINT BoneCount;
			//�T���v����
			UINT SampleCount;
			ReadBaseBoneData(Reader, BinDataDir, BinDataFile, vertices, new_pntnt_vertices,
				indices, Materials,
				SampleMatrices, BoneCount, SampleCount);
			if (vertices.size() > 0) {
				//�v�Z�Ŗ@��������
				new_pntnt_vertices.clear();
				for (size_t i = 0; i < vertices.size(); i++) {
					VertexPositionNormalTangentTextureSkinning new_pntnt_v;
					new_pntnt_v.position = vertices[i].position;
					new_pntnt_v.normal = vertices[i].normal;
					new_pntnt_v.textureCoordinate = vertices[i].textureCoordinate;
					new_pntnt_v.tangent = XMFLOAT4(0, 0, 0, 0);
					for (size_t j = 0; j < 4; j++) {
						new_pntnt_v.indices[j] = vertices[i].indices[j];
						new_pntnt_v.weights[j] = vertices[i].weights[j];
					}
					new_pntnt_vertices.push_back(new_pntnt_v);
				}
				MeshUtill::SetNormalTangent(new_pntnt_vertices);
			}

			auto Ptr = MeshResource::CreateMeshResource<VertexPositionNormalTangentTextureSkinning>(new_pntnt_vertices, indices, AccessWrite);
			Ptr->m_MeshPrimData.m_MaterialExVec.clear();
			for (auto& v : Materials) {
				Ptr->m_MeshPrimData.m_MaterialExVec.push_back(v);
			}
			Ptr->m_MeshPrimData.m_SampleMatrixVec.clear();
			for (auto& v : SampleMatrices) {
				Ptr->m_MeshPrimData.m_SampleMatrixVec.push_back(v);
			}
			Ptr->m_MeshPrimData.m_BoneCount = BoneCount;
			Ptr->m_MeshPrimData.m_SampleCount = SampleCount;
			Ptr->m_MeshPrimData.m_IsSkining = true;
			return Ptr;
		}
		catch (...) {
			throw;
		}
	}



	shared_ptr<MeshResource> MeshResource::CreateStaticModelMesh(const wstring& BinDataDir, const wstring& BinDataFile, bool AccessWrite) {
		try {
			wstring DataFile = BinDataDir + BinDataFile;
			BinaryReader Reader(DataFile);
			//�w�b�_�̓ǂݍ���
			auto pHeader = Reader.ReadArray<char>(16);
			string str = pHeader;
			if (str != "BDV1.0") {
				throw BaseException(
					L"�f�[�^�`�����Ⴂ�܂�",
					DataFile,
					L"MeshResource::CreateStaticModelMesh()"
				);
			}
			return CreateStaticModelMeshBase(Reader,BinDataDir,BinDataFile,AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateStaticModelMeshWithTangent(const wstring& BinDataDir, const wstring& BinDataFile, bool AccessWrite) {
		try {
			wstring DataFile = BinDataDir + BinDataFile;
			BinaryReader Reader(DataFile);
			//�w�b�_�̓ǂݍ���
			auto pHeader = Reader.ReadArray<char>(16);
			string str = pHeader;
			if (str != "BDV1.0") {
				throw BaseException(
					L"�f�[�^�`�����Ⴂ�܂�",
					DataFile,
					L"MeshResource::CreateStaticModelMeshWithTangent()"
				);
			}
			return CreateStaticModelMeshWithTangentBase(Reader,BinDataDir,BinDataFile,AccessWrite);
		}
		catch (...) {
			throw;
		}
	}


	shared_ptr<MeshResource> MeshResource::CreateBoneModelMesh(const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite) {
		try {
			wstring DataFile = BinDataDir + BinDataFile;
			BinaryReader Reader(DataFile);
			//�w�b�_�̓ǂݍ���
			auto pHeader = Reader.ReadArray<char>(16);
			string str = pHeader;
			if (str != "BDV1.0") {
				throw BaseException(
					L"�f�[�^�`�����Ⴂ�܂�",
					DataFile,
					L"MeshResource::CreateBoneModelMesh()"
				);
			}
			return CreateBoneModelMeshBase(Reader,BinDataDir,BinDataFile,AccessWrite);
		}
		catch (...) {
			throw;
		}

	}

	shared_ptr<MeshResource> MeshResource::CreateBoneModelMeshWithTangent(const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite) {
		try {
			wstring DataFile = BinDataDir + BinDataFile;
			BinaryReader Reader(DataFile);
			//�w�b�_�̓ǂݍ���
			auto pHeader = Reader.ReadArray<char>(16);
			string str = pHeader;
			if (str != "BDV1.0") {
				throw BaseException(
					L"�f�[�^�`�����Ⴂ�܂�",
					DataFile,
					L"MeshResource::CreateBoneModelMeshWithTangent()"
				);
			}
			return CreateBoneModelMeshWithTangentBase(Reader,BinDataDir,BinDataFile,AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	///	�}���`���b�V�����\�[�X
	//--------------------------------------------------------------------------------------
	MultiMeshResource::MultiMeshResource() :
		BaseResource()
	{}
	//�j��
	MultiMeshResource::~MultiMeshResource() {}


	shared_ptr<MultiMeshResource> MultiMeshResource::CreateStaticModelMultiMesh(const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite) {
		try {
			wstring DataFile = BinDataDir + BinDataFile;
			BinaryReader Reader(DataFile);
			//�w�b�_�̓ǂݍ���
			auto pHeader = Reader.ReadArray<char>(16);
			string str = pHeader;
			if (str != "BDV1.1") {
				throw BaseException(
					L"�f�[�^�`�����Ⴂ�܂�",
					DataFile,
					L"MultiMeshResource::CreateStaticModelMultiMesh()"
				);
			}
			//���b�V�����̓ǂݍ���
			auto blockHeader = Reader.Read<BlockHeader>();
			if (!(blockHeader.m_Type == BlockType::MashCount)) {
				throw BaseException(
					L"���b�V��y���̃w�b�_���Ⴂ�܂�",
					BinDataFile,
					L"MultiMeshResource::CreateStaticModelMultiMesh()"
				);
			}
			auto MultiMeshPtr = CreateMultiMeshResource();
			for (size_t i = 0; i < blockHeader.m_Size; i++) {
				auto MeshPtr = MeshResource::CreateStaticModelMeshBase(Reader, BinDataDir, BinDataFile, AccessWrite);
				MultiMeshPtr->AddMesh(MeshPtr);
			}
			return MultiMeshPtr;
		}
		catch (...) {
			throw;
		}

	}


	shared_ptr<MultiMeshResource> MultiMeshResource::CreateStaticModelMultiMeshWithTangent(const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite) {
		try {
			wstring DataFile = BinDataDir + BinDataFile;
			BinaryReader Reader(DataFile);
			//�w�b�_�̓ǂݍ���
			auto pHeader = Reader.ReadArray<char>(16);
			string str = pHeader;
			if (str != "BDV1.1") {
				throw BaseException(
					L"�f�[�^�`�����Ⴂ�܂�",
					DataFile,
					L"MultiMeshResource::CreateStaticModelMultiMeshWithTangent()"
				);
			}
			//���b�V�����̓ǂݍ���
			auto blockHeader = Reader.Read<BlockHeader>();
			if (!(blockHeader.m_Type == BlockType::MashCount)) {
				throw BaseException(
					L"���b�V��y���̃w�b�_���Ⴂ�܂�",
					BinDataFile,
					L"MultiMeshResource::CreateStaticModelMultiMeshWithTangent()"
				);
			}
			auto MultiMeshPtr = CreateMultiMeshResource();
			for (size_t i = 0; i < blockHeader.m_Size; i++) {
				auto MeshPtr = MeshResource::CreateStaticModelMeshWithTangentBase(Reader, BinDataDir, BinDataFile, AccessWrite);
				MultiMeshPtr->AddMesh(MeshPtr);
			}
			return MultiMeshPtr;
		}
		catch (...) {
			throw;
		}
	}


	shared_ptr<MultiMeshResource> MultiMeshResource::CreateBoneModelMultiMesh(const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite) {
		try {
			wstring DataFile = BinDataDir + BinDataFile;
			BinaryReader Reader(DataFile);
			//�w�b�_�̓ǂݍ���
			auto pHeader = Reader.ReadArray<char>(16);
			string str = pHeader;
			if (str != "BDV1.1") {
				throw BaseException(
					L"�f�[�^�`�����Ⴂ�܂�",
					DataFile,
					L"MultiMeshResource::CreateBoneModelMultiMesh()"
				);
			}
			//���b�V�����̓ǂݍ���
			auto blockHeader = Reader.Read<BlockHeader>();
			if (!(blockHeader.m_Type == BlockType::MashCount)) {
				throw BaseException(
					L"���b�V��y���̃w�b�_���Ⴂ�܂�",
					BinDataFile,
					L"MultiMeshResource::CreateBoneModelMultiMesh()"
				);
			}
			auto MultiMeshPtr = CreateMultiMeshResource();
			for (size_t i = 0; i < blockHeader.m_Size; i++) {
				auto MeshPtr = MeshResource::CreateBoneModelMeshBase(Reader, BinDataDir, BinDataFile, AccessWrite);
				MultiMeshPtr->AddMesh(MeshPtr);
			}
			return MultiMeshPtr;
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MultiMeshResource> MultiMeshResource::CreateBoneModelMultiMeshWithTangent(const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite){
		try {
			wstring DataFile = BinDataDir + BinDataFile;
			BinaryReader Reader(DataFile);
			//�w�b�_�̓ǂݍ���
			auto pHeader = Reader.ReadArray<char>(16);
			string str = pHeader;
			if (str != "BDV1.1") {
				throw BaseException(
					L"�f�[�^�`�����Ⴂ�܂�",
					DataFile,
					L"MultiMeshResource::CreateBoneModelMultiMeshWithTangent()"
				);
			}
			//���b�V�����̓ǂݍ���
			auto blockHeader = Reader.Read<BlockHeader>();
			if (!(blockHeader.m_Type == BlockType::MashCount)) {
				throw BaseException(
					L"���b�V��y���̃w�b�_���Ⴂ�܂�",
					BinDataFile,
					L"MultiMeshResource::CreateBoneModelMultiMeshWithTangent()"
				);
			}
			auto MultiMeshPtr = CreateMultiMeshResource();
			for (size_t i = 0; i < blockHeader.m_Size; i++) {
				auto MeshPtr = MeshResource::CreateBoneModelMeshWithTangentBase(Reader, BinDataDir, BinDataFile, AccessWrite);
				MultiMeshPtr->AddMesh(MeshPtr);
			}
			return MultiMeshPtr;
		}
		catch (...) {
			throw;
		}

	}


	//--------------------------------------------------------------------------------------
	//	struct DeviceResources::Impl;
	//	�p�r: Direct11�f�o�C�X�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DeviceResources::Impl {
		D3D_DRIVER_TYPE	m_D3DDriverType;			//�h���C�o�̃^�C�v
		D3D_FEATURE_LEVEL m_D3DFeatureLevel;
		// Direct3D �I�u�W�F�N�g�B
		ComPtr<ID3D11Device2> m_D3D11Device;		//�f�o�C�X
		ComPtr<ID3D11DeviceContext2> m_D3D11Context;		//�R���e�L�X�g
		ComPtr<IDXGISwapChain1> m_D3D11SwapChain;	//�X���b�v�`�F�[��


													// Direct2D �`��R���|�[�l���g�B
		ComPtr<ID2D1Factory2>		m_d2dFactory;
		ComPtr<ID2D1Device1>		m_d2dDevice;
		ComPtr<ID2D1DeviceContext1>	m_d2dContext;

		// DirectWrite �`��R���|�[�l���g�B
		ComPtr<IDWriteFactory2>		m_dwriteFactory;
		ComPtr<IWICImagingFactory2>	m_wicFactory;

		float m_dpi;

		shared_ptr<DefaultRenderTarget> m_DefaultRenderTarget;	///<�f�t�H���g�̃����_�����O�^�[�Q�b�g
		shared_ptr<ShadowMapRenderTarget> m_ShadowMapRenderTarget;	///<�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g
		shared_ptr<RenderState> m_RenderState;					///<�����_�����O�X�e�[�g



		void CreateSamplerState(D3D11_FILTER filter,
			D3D11_TEXTURE_ADDRESS_MODE addressMode,
			ID3D11SamplerState** pResult) {
			try {

				auto Dev = App::GetApp()->GetDeviceResources();
				auto pDx11Device = Dev->GetD3DDevice();

				D3D11_SAMPLER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));

				desc.Filter = filter;

				desc.AddressU = addressMode;
				desc.AddressV = addressMode;
				desc.AddressW = addressMode;

				desc.MaxAnisotropy = (pDx11Device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;

				desc.MaxLOD = FLT_MAX;
				desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

				HRESULT hr = pDx11Device->CreateSamplerState(&desc, pResult);

				if (FAILED(hr)) {
					// ���������s
					throw BaseException(
						L"�T���v���[�X�e�[�g�쐬�Ɏ��s���܂����B",
						L"if(FAILED(pDx11Device->CreateSamplerState()))",
						L"DeviceResources::Impl::CreateSamplerState()"
					);
				}
			}
			catch (...) {
				throw;
			}

		}
		void CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
			ID3D11BlendState** pResult) {
			try {
				auto Dev = App::GetApp()->GetDeviceResources();
				auto pDx11Device = Dev->GetD3DDevice();

				D3D11_BLEND_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) ||
					(destBlend != D3D11_BLEND_ZERO);

				desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
				desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
				desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

				HRESULT hr = pDx11Device->CreateBlendState(&desc, pResult);
				if (FAILED(hr)) {
					// ���������s
					throw BaseException(
						L"�u�����h�X�e�[�g�쐬�Ɏ��s���܂����B",
						L"if(FAILED(pDx11Device->CreateBlendState()))",
						L"DeviceResources::Impl::CreateBlendState()"
					);
				}
			}
			catch (...) {
				throw;
			}
		}
		void CreateDepthStencilState(bool enable, bool writeEnable,
			ID3D11DepthStencilState** pResult) {
			try {
				auto Dev = App::GetApp()->GetDeviceResources();
				auto pDx11Device = Dev->GetD3DDevice();

				D3D11_DEPTH_STENCIL_DESC desc;
				ZeroMemory(&desc, sizeof(desc));

				desc.DepthEnable = enable;
				desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
				desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

				desc.StencilEnable = false;
				desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
				desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

				desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
				desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

				desc.BackFace = desc.FrontFace;

				HRESULT hr = pDx11Device->CreateDepthStencilState(&desc, pResult);
				if (FAILED(hr)) {
					// ���������s
					throw BaseException(
						L"�f�v�X�X�e���V���X�e�[�g�쐬�Ɏ��s���܂����B",
						L"if(FAILED(pDx11Device->CreateDepthStencilState()))",
						L"DeviceResources::Impl::CreateDepthStencilState()"
					);
				}
			}
			catch (...) {
				throw;
			}
		}
		void CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
			ID3D11RasterizerState** pResult, bool Scissor = false) {
			try {
				auto Dev = App::GetApp()->GetDeviceResources();
				auto pDx11Device = Dev->GetD3DDevice();

				D3D11_RASTERIZER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));

				desc.CullMode = cullMode;
				desc.FillMode = fillMode;
				desc.DepthClipEnable = true;
				desc.MultisampleEnable = true;
				desc.ScissorEnable = Scissor;

				HRESULT hr = pDx11Device->CreateRasterizerState(&desc, pResult);
				if (FAILED(hr)) {
					// ���������s
					throw BaseException(
						L"���X�^���C�U�X�e�[�g�쐬�Ɏ��s���܂����B",
						L"if(FAILED(pDx11Device->CreateRasterizerState()))",
						L"DeviceResources::Impl::CreateRasterizerState()"
					);
				}
			}
			catch (...) {
				throw;
			}
		}

		ComPtr<ID3D11BlendState> m_OpaquePtr;
		ComPtr<ID3D11SamplerState> m_LinearClampPtr;
		ComPtr<ID3D11DepthStencilState> m_DepthDefaultPtr;
		ComPtr<ID3D11RasterizerState> m_CullBackPtr;
		//�~���[�e�b�N�X
		std::mutex Mutex;

		//�\�z�Ɣj��
		Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		~Impl() {}
		//���\�[�X�̍\�z
		void CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);

		ID3D11SamplerState* GetLinearClampSampler() {
			return Util::DemandCreate(m_LinearClampPtr, Mutex, [&](ID3D11SamplerState** pResult)
			{
				CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
			});
		}
		ID3D11BlendState* GetOpaqueBlend() {
			return Util::DemandCreate(m_OpaquePtr, Mutex, [&](ID3D11BlendState** pResult)
			{
				CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, pResult);
			});

		}

		ID3D11RasterizerState* GetCullBackRasterizer() {
			return Util::DemandCreate(m_CullBackPtr, Mutex, [&](ID3D11RasterizerState** pResult)
			{
				CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, pResult);
			});
		}

		ID3D11DepthStencilState* GetDefaultDepthStencil() {
			return Util::DemandCreate(m_DepthDefaultPtr, Mutex, [&](ID3D11DepthStencilState** pResult)
			{
				CreateDepthStencilState(true, true, pResult);
			});
		}



	};
	//�\�z
	DeviceResources::Impl::Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) :
		m_dpi(96.0f)
	{
		try {
			//���\�[�X�̍\�z
			CreateDeviceResources(hWnd, isFullScreen, Width, Height);
		}
		catch (...) {
			throw;
		}
	}
	//���\�[�X�̍\�z
	void DeviceResources::Impl::CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) {

		// Direct2D ���\�[�X�����������܂��B
		D2D1_FACTORY_OPTIONS options;
		ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
		// �v���W�F�N�g���f�o�b�O �r���h�Ɋ܂܂�Ă���ꍇ�́ADirect2D �f�o�b�O�� SDK ���C���[����ėL���ɂ��܂��B
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
		// Direct2D �t�@�N�g�������������܂��B
		ThrowIfFailed(
			D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED,
				__uuidof(ID2D1Factory2),
				&options,
				&m_d2dFactory
			),
			L"Factory�쐬�Ɏ��s���܂����B",
			L"D2D1CreateFactory()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);


		// DirectWrite �t�@�N�g�������������܂��B
		ThrowIfFailed(
			DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory2),
				&m_dwriteFactory
			),
			L"DirectWrite �t�@�N�g���쐬�Ɏ��s���܂����B",
			L"DWriteCreateFactory()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		ThrowIfFailed(
			CoCreateInstance(
				CLSID_WICImagingFactory2,
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&m_wicFactory)
			),
			L"(WIC) �t�@�N�g���쐬�Ɏ��s���܂����B",
			L"CoCreateInstance()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);


		HRESULT hr;	//API�̖߂�l�p
					//�f�o�C�X�쐬�p�t���O
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		//�f�o�b�O��
#ifdef _DEBUG
		//		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		//�h���C�o�̃^�C�v��z��
		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		//�I���ł���h���C�o�[�̐�
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		//�@�\�Z�b�g�̔z��
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,	//Dx11�@�\�I�����[
		};
		//�I���ł���@�\�̐�
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		//�X���b�v�`�F�[���̃f�X�N�v���^�̐ݒ�
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;					//�o�b�t�@��(2�_�u���o�b�t�@)
		sd.BufferDesc.Width = Width;		//�o�b�t�@�̕�
		sd.BufferDesc.Height = Height;	//�o�b�t�@�̍���
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//8 �r�b�g �A���t�@���T�|�[�g���� 4 �����A16 �r�b�g�����Ȃ����K�������t�H�[�}�b�g
		sd.BufferDesc.RefreshRate.Numerator = 60;	//���t���b�V�����[�g�ő�l
		sd.BufferDesc.RefreshRate.Denominator = 1;	//���t���b�V�����[�g�ŏ��l
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�͗p�Ƃ��Ē�`
		sd.OutputWindow = hWnd;		//�E�C���h�E�̃n���h��
		sd.SampleDesc.Count = 1;		//�}���`�T���v�����O ����1
		sd.SampleDesc.Quality = 0;		//�}���`�T���v�����O�N�I���e�B��0�i�ő�j

		sd.Windowed = TRUE;			//�E�C���h�E���[�h�ō쐬���Ă��Ƃ���t��s�N���[���ɂ���

		ComPtr<ID3D11Device> temp_device;
		ComPtr<ID3D11DeviceContext>	temp_context;
		ComPtr<IDXGISwapChain>	temp_swapChain;


		//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
			//�h���C�o�^�C�v��z�񂩂�擾
			m_D3DDriverType = driverTypes[driverTypeIndex];
			//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬
			hr = D3D11CreateDeviceAndSwapChain(
				nullptr,
				m_D3DDriverType,
				nullptr,
				createDeviceFlags,
				featureLevels,
				numFeatureLevels,
				D3D11_SDK_VERSION,
				&sd,
				&temp_swapChain,
				&temp_device,
				&m_D3DFeatureLevel,
				&temp_context
			);
			//���������炻�̃h���C�o���g��
			if (SUCCEEDED(hr))
				break;
		}
		ThrowIfFailed(
			hr,
			L"DX11�f�o�C�X�ƃX���b�v�`�F�[���̍쐬�Ɏ��s���܂����B",
			L"D3D11CreateDeviceAndSwapChain()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		//���\�[�X���o�[�W�����A�b�v����
		ThrowIfFailed(
			temp_device.As(&m_D3D11Device),
			L"DX11�f�o�C�X�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"temp_device.As(&m_D3D11Device)",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(
			temp_context.As(&m_D3D11Context),
			L"DX11�R���e�N�X�g�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"temp_context.As(&m_D3D11Context)",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(
			temp_swapChain.As(&m_D3D11SwapChain),
			L"DX11�X���b�v�`�F�[���̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"temp_swapChain.As(&m_D3D11SwapChain)",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		// Direct2D �f�o�C�X �I�u�W�F�N�g�ƁA�Ή�����R���e�L�X�g���쐬���܂��B

		ComPtr<IDXGIDevice3> dxgiDevice;

		ThrowIfFailed(
			m_D3D11Device.As(&dxgiDevice),
			L"dxgiDevice�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"m_d3dDevice.As(&dxgiDevice)",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		ThrowIfFailed(
			m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice),
			L"dxgiDevice�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"m_d2dFactory->CreateDevice()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		ThrowIfFailed(
			m_d2dDevice->CreateDeviceContext(
				D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
				&m_d2dContext
			),
			L"2d�f�o�C�X�R���e�L�X�g�쐬�Ɏ��s���܂����B",
			L"m_d2dDevice->CreateDeviceContext()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);
		if (isFullScreen) {
			ThrowIfFailed(
				m_D3D11SwapChain->SetFullscreenState(true, NULL),
				L"�t���X�N���[���ڍs�Ɏ��s���܂����B",
				L"m_D3D11SwapChain->SetFullscreenState(true, NULL)",
				L"DeviceResources::Impl::CreateDeviceResources()"
			);

		}

	}

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	//	�p�r: Direct11�f�o�C�X�ȂǃN���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	DeviceResources::DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) :
		pImpl(new Impl(hWnd, isFullScreen, Width, Height))
	{
		//�����_�����O�X�e�[�g�̍쐬
		pImpl->m_RenderState = make_shared<RenderState>();
	}
	DeviceResources::~DeviceResources() {}
	//�A�N�Z�T
	ID3D11Device2*	DeviceResources::GetD3DDevice() const { return pImpl->m_D3D11Device.Get(); }
	ID3D11DeviceContext2* DeviceResources::GetD3DDeviceContext() const { return pImpl->m_D3D11Context.Get(); }
	IDXGISwapChain1* DeviceResources::GetSwapChain() const { return pImpl->m_D3D11SwapChain.Get(); }
	D3D_FEATURE_LEVEL DeviceResources::GetFeatureLevel() const { return pImpl->m_D3DFeatureLevel; }

	// D2D �A�N�Z�T�[�B
	ID2D1Factory2*			DeviceResources::GetD2DFactory() const { return pImpl->m_d2dFactory.Get(); }
	ID2D1Device1*			DeviceResources::GetD2DDevice() const { return pImpl->m_d2dDevice.Get(); }
	ID2D1DeviceContext1*	DeviceResources::GetD2DDeviceContext() const { return pImpl->m_d2dContext.Get(); }
	IDWriteFactory2*		DeviceResources::GetDWriteFactory() const { return pImpl->m_dwriteFactory.Get(); }
	IWICImagingFactory2*	DeviceResources::GetWicImagingFactory() const { return pImpl->m_wicFactory.Get(); }

	void DeviceResources::InitializeStates() {
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		ID3D11Buffer* pNullConstantBuffer[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = { nullptr };

		auto pID3D11DeviceContext = GetD3DDeviceContext();
		//��n��
		//�R���X�^���g�o�b�t�@�N���A
		pID3D11DeviceContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		//�V�F�[�_�[���\�[�X���N���A
		pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
		//�T���v���[���N���A
		pID3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
		//�V�F�[�_�̃N���A
		pID3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->CSSetShader(nullptr, nullptr, 0);
		//�C���v�b�g���C�A�E�g�̃N���A
		pID3D11DeviceContext->IASetInputLayout(nullptr);
		//�f�t�H���g�̐ݒ�
		//���j�A�T���v���[��ݒ肵�Ă���
		ID3D11SamplerState* samplerState = pImpl->GetLinearClampSampler();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//�u�����h�h��Ԃ�
		pID3D11DeviceContext->OMSetBlendState(pImpl->GetOpaqueBlend(), nullptr, 0xffffffff);
		//�����_�[�X�e�[�g�O�ʂ̂ݕ`��
		pID3D11DeviceContext->RSSetState(pImpl->GetCullBackRasterizer());
		//�f�v�X�X�e���V���g�p
		pID3D11DeviceContext->OMSetDepthStencilState(pImpl->GetDefaultDepthStencil(), 0);

	}

	shared_ptr<DefaultRenderTarget> DeviceResources::GetDefaultRenderTarget() {
		if (!pImpl->m_DefaultRenderTarget) {
			//�f�t�H���g�̃����_�����O�^�[�Q�b�g���쐬
			pImpl->m_DefaultRenderTarget = make_shared<DefaultRenderTarget>();
		}
		return pImpl->m_DefaultRenderTarget;
	}
	shared_ptr<ShadowMapRenderTarget> DeviceResources::GetShadowMapRenderTarget(float ShadowMapDimension ) {
		if (!pImpl->m_ShadowMapRenderTarget) {
			//�V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g���쐬
			pImpl->m_ShadowMapRenderTarget = make_shared<ShadowMapRenderTarget>(ShadowMapDimension);
		}
		return pImpl->m_ShadowMapRenderTarget;
	}
	shared_ptr<RenderState> DeviceResources::GetRenderState()const {
		return pImpl->m_RenderState;
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
	void DeviceResources::ClearDefaultViews(const bsm::Col4& col) {
		auto DefaultTarget = GetDefaultRenderTarget();
		DefaultTarget->ClearViews(col);
	}
	void DeviceResources::StartDefaultDraw() {
		auto DefaultTarget = GetDefaultRenderTarget();
		DefaultTarget->StartRenderTarget();
	}
	void DeviceResources::EndDefaultDraw() {
		auto DefaultTarget = GetDefaultRenderTarget();
		DefaultTarget->EndRenderTarget();
	}

	void DeviceResources::Present(unsigned int SyncInterval, unsigned int  Flags) {
		// �o�b�N�o�b�t�@����t�����g�o�b�t�@�ɓ]��
		GetSwapChain()->Present(SyncInterval, Flags);
	}


	//--------------------------------------------------------------------------------------
	//	struct RenderState;
	//	�p�r: �����_�����O�X�e�[�g�N���X
	//--------------------------------------------------------------------------------------
	struct RenderState::Impl {
		//�u�����h�X�e�[�g
		ComPtr<ID3D11BlendState> m_OpaquePtr;
		ComPtr<ID3D11BlendState> m_AlphaBlendPtr;
		ComPtr<ID3D11BlendState> m_AlphaBlendExPtr;

		ComPtr<ID3D11BlendState> m_AdditivePtr;
		ComPtr<ID3D11BlendState> m_NonPremultipliedPtr;
		//�f�v�X�X�e���V���X�e�[�g
		ComPtr<ID3D11DepthStencilState> m_DepthNonePtr;
		ComPtr<ID3D11DepthStencilState> m_DepthDefaultPtr;
		ComPtr<ID3D11DepthStencilState> m_DepthReadPtr;
		//���X�^���C�U�X�e�[�g
		ComPtr<ID3D11RasterizerState> m_CullNonePtr;
		ComPtr<ID3D11RasterizerState> m_CullNoneScissorPtr;
		ComPtr<ID3D11RasterizerState> m_CullFrontPtr;
		ComPtr<ID3D11RasterizerState> m_CullFrontScissorPtr;
		ComPtr<ID3D11RasterizerState> m_CullBackPtr;
		ComPtr<ID3D11RasterizerState> m_CullBackScissorPtr;
		ComPtr<ID3D11RasterizerState> m_WireframePtr;
		ComPtr<ID3D11RasterizerState> m_WireframeScissorPtr;
		//�T���v���[�X�e�[�g
		ComPtr<ID3D11SamplerState> m_PointWrapPtr;
		ComPtr<ID3D11SamplerState> m_PointClampPtr;
		ComPtr<ID3D11SamplerState> m_LinearWrapPtr;
		ComPtr<ID3D11SamplerState> m_LinearClampPtr;
		ComPtr<ID3D11SamplerState> m_AnisotropicWrapPtr;
		ComPtr<ID3D11SamplerState> m_AnisotropicClampPtr;
		ComPtr<ID3D11SamplerState> m_ComparisonLinearPtr;
		//�~���[�e�b�N�X
		std::mutex Mutex;

		//--------------------------------------------------------------------------------------
		//	Impl(};
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		Impl() {}
		~Impl() {}
		//--------------------------------------------------------------------------------------
		//	void CreateBlendState(
		//	D3D11_BLEND srcBlend,		//�\�[�X�u�����h
		//	D3D11_BLEND destBlend,		//�f�X�g�u�����h
		//	ID3D11BlendState** pResult	//�󂯎��C���^�[�t�F�C�X
		//	);
		//	�p�r: �u�����h�X�e�[�g���쐬����w���p�[�֐�
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
			ID3D11BlendState** pResult);
		//--------------------------------------------------------------------------------------
		//	void CreateDepthStencilState(
		//	bool enable,
		//	bool writeEnable,
		//	ID3D11DepthStencilState** pResult
		//	);
		//	�p�r: �f�v�X�X�e���V���X�e�[�g���쐬����w���p�[�֐�
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void CreateDepthStencilState(bool enable, bool writeEnable,
			ID3D11DepthStencilState** pResult);
		//--------------------------------------------------------------------------------------
		//	void CreateRasterizerState(
		//	D3D11_CULL_MODE cullMode,			//�J�����O
		//	D3D11_FILL_MODE fillMode,			//�h��
		//	ID3D11RasterizerState** pResult,		//�󂯎��C���^�[�t�F�C�X
		//	bool Scissor = false				//��ʕ������邩�ǂ���
		//	);
		//	�p�r: ���X�^���C�U�X�e�[�g���쐬����w���p�[�֐�
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
			ID3D11RasterizerState** pResult, bool Scissor = false);
		//--------------------------------------------------------------------------------------
		//	void CreateSamplerState(
		//	D3D11_FILTER filter,					//�t�B���^�[
		//	D3D11_TEXTURE_ADDRESS_MODE addressMode	//�A�h���X���[�h
		//	ID3D11SamplerState** pResult			//�󂯎��C���^�[�t�F�C�X
		//	);
		//	�p�r: �T���v���[�X�e�[�g���쐬����w���p�[�֐�
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void CreateSamplerState(D3D11_FILTER filter,
			D3D11_TEXTURE_ADDRESS_MODE addressMode,
			ID3D11SamplerState** pResult);


	};

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateBlendState(
	//	D3D11_BLEND srcBlend,		//�\�[�X�u�����h
	//	D3D11_BLEND destBlend,		//�f�X�g�u�����h
	//	ID3D11BlendState** pResult	//�󂯎��C���^�[�t�F�C�X
	//	);
	//	�p�r: �u�����h�X�e�[�g���쐬����w���p�[�֐�
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
		ID3D11BlendState** pResult) {
		try {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) ||
				(destBlend != D3D11_BLEND_ZERO);

			desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
			desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
			desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			HRESULT hr = pDx11Device->CreateBlendState(&desc, pResult);
			if (FAILED(hr)) {
				// ���������s
				throw BaseException(
					L"�u�����h�X�e�[�g�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx11Device->CreateBlendState()))",
					L"RenderState::Impl::CreateBlendState()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateDepthStencilState(
	//	bool enable,
	//	bool writeEnable,
	//	ID3D11DepthStencilState** pResult
	//	);
	//	�p�r: �f�v�X�X�e���V���X�e�[�g���쐬����w���p�[�֐�
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateDepthStencilState(bool enable, bool writeEnable,
		ID3D11DepthStencilState** pResult) {
		try {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.DepthEnable = enable;
			desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

			desc.StencilEnable = false;
			desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

			desc.BackFace = desc.FrontFace;

			HRESULT hr = pDx11Device->CreateDepthStencilState(&desc, pResult);
			if (FAILED(hr)) {
				// ���������s
				throw BaseException(
					L"�f�v�X�X�e���V���X�e�[�g�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx11Device->CreateDepthStencilState()))",
					L"RenderState::Impl::CreateDepthStencilState()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateRasterizerState(
	//	D3D11_CULL_MODE cullMode,			//�J�����O
	//	D3D11_FILL_MODE fillMode,			//�h��
	//	ID3D11RasterizerState** pResult,		//�󂯎��C���^�[�t�F�C�X
	//	bool Scissor = false				//��ʕ������邩�ǂ���
	//	);
	//	�p�r: ���X�^���C�U�X�e�[�g���쐬����w���p�[�֐�
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
		ID3D11RasterizerState** pResult, bool Scissor) {
		try {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_RASTERIZER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.CullMode = cullMode;
			desc.FillMode = fillMode;
			desc.DepthClipEnable = true;
			desc.MultisampleEnable = true;
			desc.ScissorEnable = Scissor;

			HRESULT hr = pDx11Device->CreateRasterizerState(&desc, pResult);
			if (FAILED(hr)) {
				// ���������s
				throw BaseException(
					L"���X�^���C�U�X�e�[�g�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx11Device->CreateRasterizerState()))",
					L"RenderState::Impl::CreateRasterizerState()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateSamplerState(
	//	D3D11_FILTER filter,					//�t�B���^�[
	//	D3D11_TEXTURE_ADDRESS_MODE addressMode	//�A�h���X���[�h
	//	ID3D11SamplerState** pResult			//�󂯎��C���^�[�t�F�C�X
	//	);
	//	�p�r: �T���v���[�X�e�[�g���쐬����w���p�[�֐�
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateSamplerState(D3D11_FILTER filter,
		D3D11_TEXTURE_ADDRESS_MODE addressMode,
		ID3D11SamplerState** pResult) {
		try {

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_SAMPLER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.Filter = filter;

			desc.AddressU = addressMode;
			desc.AddressV = addressMode;
			desc.AddressW = addressMode;

			desc.MaxAnisotropy = (pDx11Device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;

			desc.MaxLOD = FLT_MAX;
			desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

			HRESULT hr = pDx11Device->CreateSamplerState(&desc, pResult);

			if (FAILED(hr)) {
				// ���������s
				throw BaseException(
					L"�T���v���[�X�e�[�g�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx11Device->CreateSamplerState()))",
					L"RenderState::Impl::CreateSamplerState()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class RenderState;
	//	�p�r: �����_�����O�X�e�[�g�N���X
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	RenderState::RenderState();
	//	�p�r: �R���X�g���N�^
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	RenderState::RenderState() :
		pImpl(new Impl())
	{
	}

	RenderState::~RenderState() {
	}

	//--------------------------------------------------------------------------------------
	//	�p�r: �u�����h�X�e�[�g�A�N�Z�b�T
	//	�߂�l: �u�����h�X�e�[�g�̃|�C���^
	//--------------------------------------------------------------------------------------
	ID3D11BlendState* RenderState::GetOpaque()const {
		return Util::DemandCreate(pImpl->m_OpaquePtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, pResult);
		});

	}

	ID3D11BlendState* RenderState::GetAlphaBlend()const {
		return Util::DemandCreate(pImpl->m_AlphaBlendPtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, pResult);
		});
	}

	ID3D11BlendState* RenderState::GetAlphaBlendEx()const {
		if (!pImpl->m_AlphaBlendExPtr) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			HRESULT hr = pDx11Device->CreateBlendState(&desc, &pImpl->m_AlphaBlendExPtr);
			if (FAILED(hr)) {
				// ���������s
				throw BaseException(
					L"�u�����h�X�e�[�g�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx11Device->CreateBlendState()))",
					L"BasicState::GetAlphaBlendEx()"
				);
			}
		}
		return pImpl->m_AlphaBlendExPtr.Get();
	}


	ID3D11BlendState* RenderState::GetAdditive()const {
		return Util::DemandCreate(pImpl->m_AdditivePtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, pResult);
		});
	}

	ID3D11BlendState*  RenderState::GetNonPremultiplied()const {
		return Util::DemandCreate(pImpl->m_NonPremultipliedPtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, pResult);
		});
	}

	//--------------------------------------------------------------------------------------
	//	�p�r: �f�v�X�X�e���V���X�e�[�g�A�N�Z�b�T
	//	�߂�l: �f�v�X�X�e���V���X�e�[�g�̃|�C���^
	//--------------------------------------------------------------------------------------
	ID3D11DepthStencilState* RenderState::GetDepthNone()const {
		return Util::DemandCreate(pImpl->m_DepthNonePtr, pImpl->Mutex, [&](ID3D11DepthStencilState** pResult)
		{
			pImpl->CreateDepthStencilState(false, false, pResult);
		});
	}

	ID3D11DepthStencilState* RenderState::GetDepthDefault()const {
		return Util::DemandCreate(pImpl->m_DepthDefaultPtr, pImpl->Mutex, [&](ID3D11DepthStencilState** pResult)
		{
			pImpl->CreateDepthStencilState(true, true, pResult);
		});
	}

	ID3D11DepthStencilState* RenderState::GetDepthRead()const {
		return Util::DemandCreate(pImpl->m_DepthReadPtr, pImpl->Mutex, [&](ID3D11DepthStencilState** pResult)
		{
			pImpl->CreateDepthStencilState(true, false, pResult);
		});
	}

	//--------------------------------------------------------------------------------------
	//	�p�r: ���X�^���C�U�X�e�[�g�A�N�Z�b�T
	//	�߂�l: ���X�^���C�U�X�e�[�g�̃|�C���^
	//--------------------------------------------------------------------------------------
	ID3D11RasterizerState* RenderState::GetCullNone()const {
		return Util::DemandCreate(pImpl->m_CullNonePtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullNoneScissor()const {
		return Util::DemandCreate(pImpl->m_CullNoneScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, pResult, true);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullFront()const {
		return Util::DemandCreate(pImpl->m_CullFrontPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullFrontScissor()const {
		return Util::DemandCreate(pImpl->m_CullFrontScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, pResult, true);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullBack()const {
		return Util::DemandCreate(pImpl->m_CullBackPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullBackScissor()const {
		return Util::DemandCreate(pImpl->m_CullBackScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, pResult, true);
		});
	}

	ID3D11RasterizerState* RenderState::GetWireframe()const {
		return Util::DemandCreate(pImpl->m_WireframePtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetWireframeScissor()const {
		return Util::DemandCreate(pImpl->m_WireframeScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME, pResult, true);
		});
	}

	//--------------------------------------------------------------------------------------
	//	�p�r: �T���v���[�X�e�[�g�A�N�Z�b�T
	//	�߂�l: �T���v���[�X�e�[�g�̃|�C���^
	//--------------------------------------------------------------------------------------
	ID3D11SamplerState* RenderState::GetPointWrap()const {
		return Util::DemandCreate(pImpl->m_PointWrapPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetPointClamp()const {
		return Util::DemandCreate(pImpl->m_PointClampPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetLinearWrap()const {
		return Util::DemandCreate(pImpl->m_LinearWrapPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetLinearClamp()const {
		return Util::DemandCreate(pImpl->m_LinearClampPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetAnisotropicWrap()const {
		return Util::DemandCreate(pImpl->m_AnisotropicWrapPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetAnisotropicClamp()const {
		return Util::DemandCreate(pImpl->m_AnisotropicClampPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetComparisonLinear()const {
		if (!pImpl->m_ComparisonLinearPtr) {
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			D3D11_SAMPLER_DESC comparisonSamplerDesc;
			ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
			comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.BorderColor[0] = 1.0f;
			comparisonSamplerDesc.BorderColor[1] = 1.0f;
			comparisonSamplerDesc.BorderColor[2] = 1.0f;
			comparisonSamplerDesc.BorderColor[3] = 1.0f;
			comparisonSamplerDesc.MinLOD = 0.f;
			comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			comparisonSamplerDesc.MipLODBias = 0.f;
			comparisonSamplerDesc.MaxAnisotropy = 0;
			comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
			comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
			ThrowIfFailed(
				pDx11Device->CreateSamplerState(&comparisonSamplerDesc, &pImpl->m_ComparisonLinearPtr),
				L"��r�p�T���v���[�̍쐬�Ɏ��s���܂���",
				L"pDx11Device->CreateSamplerState(&comparisonSamplerDesc, &pImpl->m_ComparisonSampler_linear)",
				L"RenderState::GetComparisonLinear()"
			);
		}
		return pImpl->m_ComparisonLinearPtr.Get();
	}

	//--------------------------------------------------------------------------------------
	//	struct RenderTarget::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct RenderTarget::Impl {
		D3D11_VIEWPORT m_ViewPort;	//�r���[�|�[�g
		Impl() {
			ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));
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


	const D3D11_VIEWPORT& RenderTarget::GetViewport() const {
		return pImpl->m_ViewPort;
	}

	void RenderTarget::SetViewport(const D3D11_VIEWPORT& Viewport) {
		pImpl->m_ViewPort = Viewport;
	}


	//--------------------------------------------------------------------------------------
	//	struct ShadowMapRenderTarget::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct ShadowMapRenderTarget::Impl {
		const float m_ShadowMapDimension;	//�V���h�E�}�b�v�̑傫��
		ComPtr<ID3D11ShaderResourceView>	m_ShaderResourceView;	//�V�F�[�_���\�[�X�r���[
		ComPtr<ID3D11RasterizerState> m_ShadowRenderState;	//�V���h�E�}�b�v�̃��X�^���C�U�X�e�[�g
		ComPtr<ID3D11Texture2D>		m_DepthStencil;		//�[�x�X�e���V���o�b�t�@
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;	//�[�x�X�e���V���r���[
		Impl(float ShadowMapDimension) :
			m_ShadowMapDimension(ShadowMapDimension)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class ShadowMapRenderTarget: public RenderTarget;
	//	�p�r: �V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ShadowMapRenderTarget::ShadowMapRenderTarget(float ShadowMapDimension) :
		pImpl(new Impl(ShadowMapDimension))
	{
		try {
			//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11Device = Dev->GetD3DDevice();
			auto pSwapChain = Dev->GetSwapChain();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

			//�V���h�E�}�b�v�e�N�X�`���̍쐬
			D3D11_TEXTURE2D_DESC shadowMapDesc;
			ZeroMemory(&shadowMapDesc, sizeof(D3D11_TEXTURE2D_DESC));
			shadowMapDesc.Width = static_cast<UINT>(pImpl->m_ShadowMapDimension);
			shadowMapDesc.Height = static_cast<UINT>(pImpl->m_ShadowMapDimension);
			shadowMapDesc.MipLevels = 1;
			shadowMapDesc.ArraySize = 1;
			shadowMapDesc.SampleDesc.Count = 1;
			shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
			shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			shadowMapDesc.Format = DXGI_FORMAT_R32_TYPELESS;

			ThrowIfFailed(
				pD3D11Device->CreateTexture2D(&shadowMapDesc, nullptr, &pImpl->m_DepthStencil),
				L"�V���h�E�}�b�v�e�N�X�`���̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateTexture2D(&shadowMapDesc, nullptr, &m_DepthStencil)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);

			//�[�x�X�e���V���r���[�쐬
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
			ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;

			ThrowIfFailed(
				pD3D11Device->CreateDepthStencilView(pImpl->m_DepthStencil.Get(), &depthStencilViewDesc, &pImpl->m_DepthStencilView),
				L"�[�x�X�e���V���r���[�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateDepthStencilView(&m_DepthStencil, &depthStencilViewDesc, &m_DepthStencilView)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);

			//�V�F�[�_�[���\�[�X�r���[�쐬
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			ThrowIfFailed(
				pD3D11Device->CreateShaderResourceView(pImpl->m_DepthStencil.Get(), &shaderResourceViewDesc, &pImpl->m_ShaderResourceView),
				L"�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateShaderResourceView(m_DepthStencil.Get(), &shaderResourceViewDesc, &m_ShaderResourceView)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);


			D3D11_RASTERIZER_DESC shadowRenderStateDesc;
			ZeroMemory(&shadowRenderStateDesc, sizeof(D3D11_RASTERIZER_DESC));
			shadowRenderStateDesc.CullMode = D3D11_CULL_FRONT;
			shadowRenderStateDesc.FillMode = D3D11_FILL_SOLID;
			shadowRenderStateDesc.DepthClipEnable = true;

			ThrowIfFailed(pD3D11Device->CreateRasterizerState(&shadowRenderStateDesc, &pImpl->m_ShadowRenderState),
				L"�V���h�E�}�b�v�̃��X�^���C�U�X�e�[�g�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateRasterizerState(&shadowRenderStateDesc,&pImpl->m_ShadowRenderState)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);


			//�r���[�|�[�g�̍쐬
			//�V���h�E�����_�����O�r���[�|�[�g
			D3D11_VIEWPORT ViewPort;
			ZeroMemory(&ViewPort, sizeof(D3D11_VIEWPORT));
			ViewPort.Height = pImpl->m_ShadowMapDimension;
			ViewPort.Width = pImpl->m_ShadowMapDimension;
			ViewPort.MinDepth = 0.f;
			ViewPort.MaxDepth = 1.f;
			SetViewport(ViewPort);
		}
		catch (...) {
			throw;
		}
	}

	ShadowMapRenderTarget::~ShadowMapRenderTarget() {}

	//�A�N�Z�T
	float ShadowMapRenderTarget::GetShadowMapDimension() const { return pImpl->m_ShadowMapDimension; }
	ID3D11ShaderResourceView* ShadowMapRenderTarget::GetShaderResourceView() const { return pImpl->m_ShaderResourceView.Get(); }
	ID3D11Texture2D* ShadowMapRenderTarget::GetDepthStencil() const { return pImpl->m_DepthStencil.Get(); }
	ID3D11DepthStencilView*	ShadowMapRenderTarget::GetDepthStencilView() const { return pImpl->m_DepthStencilView.Get(); }


	//�����_�����O�^�[�Q�b�g���N���A����
	void ShadowMapRenderTarget::ClearViews(const bsm::Col4& col) {
		//�V���h�E�}�b�v��col�͖��g�p
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�V���h�E�}�b�v�N���A
		pD3D11DeviceContext->ClearDepthStencilView(pImpl->m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);
	}
	//�����_�����O�^�[�Q�b�g���J�n����
	void ShadowMapRenderTarget::StartRenderTarget() {
		//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�����_�����O�^�[�Q�b�g�͐[�x�X�e���V���r���[�̂ݎw��
		ID3D11RenderTargetView* pnullView = nullptr;

		pD3D11DeviceContext->OMSetRenderTargets(1, &pnullView, pImpl->m_DepthStencilView.Get());
		//�r���[�|�[�g�̐ݒ�
		pD3D11DeviceContext->RSSetViewports(1, &GetViewport());
		//���X�^���C�U�X�e�[�g�̐ݒ�(�ݒ����)
		pD3D11DeviceContext->RSSetState(nullptr);
		//�V�F�[�_�[�͎w�肵�Ȃ�
		//���_�V�F�[�_�̐ݒ�(�����ł͎w�肵�Ȃ�)
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		//�s�N�Z���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		//�W�I���g���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);


	}
	//�����_�����O�^�[�Q�b�g���I������
	void ShadowMapRenderTarget::EndRenderTarget() {
		//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�����_�����O�^�[�Q�b�g�͐[�x�X�e���V���r���[�̂ݎw��
		ID3D11RenderTargetView* pnullView = nullptr;
		pD3D11DeviceContext->OMSetRenderTargets(1, &pnullView, nullptr);
		//�r���[�|�[�g�̐ݒ�
		pD3D11DeviceContext->RSSetViewports(1, &GetViewport());
		//���X�^���C�U�X�e�[�g�̐ݒ�(�ݒ����)
		pD3D11DeviceContext->RSSetState(nullptr);
		//�V�F�[�_�[�͎w�肵�Ȃ�
		//���_�V�F�[�_�̐ݒ�(�����ł͎w�肵�Ȃ�)
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		//�s�N�Z���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		//�W�I���g���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
	}

	//--------------------------------------------------------------------------------------
	//	struct DefaultRenderTarget::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DefaultRenderTarget::Impl {
		//�r���[�֘A
		ComPtr<ID3D11RenderTargetView> m_D3D11RenderTargetView;	//�����_�����O�^�[�Q�b�g���r���[
		ComPtr<ID3D11Texture2D>		m_DepthStencil;		//�[�x�X�e���V���o�b�t�@
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;	//�[�x�X�e���V���r���[

		ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;

		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class DefaultRenderTarget : public RenderTarget;
	//	�p�r: �f�t�H���g�̃����_�[�^�[�Q�b�g
	//	���f�t�H���g�̃����_���[
	//--------------------------------------------------------------------------------------
	//�\�z
	DefaultRenderTarget::DefaultRenderTarget() :
		pImpl(new Impl())
	{
		try {

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11Device = Dev->GetD3DDevice();
			auto pSwapChain = Dev->GetSwapChain();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto pD2D11DeviceContext = Dev->GetD2DDeviceContext();


			//�����_�����O�^�[�Q�b�g�r���[�̍쐬
			ComPtr<ID3D11Texture2D> pBackBuffer;
			//�܂��o�b�N�o�b�t�@�̃|�C���^�𓾂�
			ThrowIfFailed(
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer),
				L"�X���b�v�`�F�[������o�b�N�o�b�t�@�̎擾�Ɏ��s���܂����B",
				L"pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);
			//�o�b�N�o�b�t�@���烌���_�����O�^�[�Q�b�g�̃r���[���쐬����
			ThrowIfFailed(
				pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pImpl->m_D3D11RenderTargetView),
				L"DX11�o�b�N�o�b�t�@����̃����_�����O�^�[�Q�b�g�r���[���쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_D3D11RenderTargetView)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			//�[�x�e�N�X�`���̍쐬
			D3D11_TEXTURE2D_DESC descDepth;
			ZeroMemory(&descDepth, sizeof(descDepth));
			descDepth.Width = App::GetApp()->GetGameWidth();
			descDepth.Height = App::GetApp()->GetGameHeight();
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = 0;

			ThrowIfFailed(
				pD3D11Device->CreateTexture2D(&descDepth, nullptr, &pImpl->m_DepthStencil),
				L"DX11�[�x�e�N�X�`���̍쐬���s�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencil)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			//�[�x�X�e���V���r���[�̍쐬
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;

			ThrowIfFailed(
				pD3D11Device->CreateDepthStencilView(pImpl->m_DepthStencil.Get(), &descDSV, &pImpl->m_DepthStencilView),
				L"DX11�[�x�X�e���V���r���[�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateDepthStencilView(m_DepthStencil.Get(), &descDSV, &m_DepthStencilView)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			ComPtr<IDXGISurface2> dxgiBackBuffer;
			ThrowIfFailed(
				pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)),
				L"2d�f�o�C�X�R���e�L�X�g�쐬�Ɏ��s���܂����B",
				L"m_d2dDevice->CreateDeviceContext()",
				L"DeviceResources::Impl::CreateDeviceResources()"
			);


			ThrowIfFailed(
				pD2D11DeviceContext->CreateBitmapFromDxgiSurface(
					dxgiBackBuffer.Get(),
					nullptr,	//�f�t�H���g�ݒ�
					&pImpl->m_d2dTargetBitmap
				),
				L"2d�r�b�g�}�b�v�쐬�Ɏ��s���܂����B",
				L"pD2D11DeviceContext->CreateBitmapFromDxgiSurface()",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			pD2D11DeviceContext->SetTarget(pImpl->m_d2dTargetBitmap.Get());
			//�O���[�X�P�[�� �e�L�X�g�̃A���`�G�C���A�V���O
			pD2D11DeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

			//�f�t�H���g�r���[�|�[�g�̃Z�b�g
			D3D11_VIEWPORT ViewPort;
			ZeroMemory(&ViewPort, sizeof(ViewPort));
			ViewPort.Width = (float)App::GetApp()->GetGameWidth();
			ViewPort.Height = (float)App::GetApp()->GetGameHeight();
			ViewPort.MinDepth = 0.0f;
			ViewPort.MaxDepth = 1.0f;
			ViewPort.TopLeftX = 0;
			ViewPort.TopLeftY = 0;
			SetViewport(ViewPort);


		}
		catch (...) {
			throw;
		}
	}
	DefaultRenderTarget::~DefaultRenderTarget() {}


	//�A�N�Z�T
	ID3D11RenderTargetView* DefaultRenderTarget::GetRenderTargetView() const { return pImpl->m_D3D11RenderTargetView.Get(); }
	ID3D11Texture2D* DefaultRenderTarget::GetDepthStencil() const { return pImpl->m_DepthStencil.Get(); }
	ID3D11DepthStencilView*	DefaultRenderTarget::GetDepthStencilView() const { return pImpl->m_DepthStencilView.Get(); }
	ID2D1Bitmap1*			DefaultRenderTarget::GetD2DTargetBitmap() const { return pImpl->m_d2dTargetBitmap.Get(); }


	//����
	//�X�N���[���S�̂��w��̐F�ŃN���A����
	void DefaultRenderTarget::ClearViews(const bsm::Col4& col) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�o�b�t�@�̃N���A
		float Color[4] = { col.x, col.y, col.z, col.w };
		D3D11_VIEWPORT ViewPort;
		//�r���[�|�[�g�̃Z�b�g�A�b�v
		ZeroMemory(&ViewPort, sizeof(ViewPort));
		ViewPort.Width = (float)App::GetApp()->GetGameWidth();
		ViewPort.Height = (float)App::GetApp()->GetGameHeight();
		ViewPort.MinDepth = 0.0f;
		ViewPort.MaxDepth = 1.0f;
		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		pD3D11DeviceContext->RSSetViewports(1, &ViewPort);
		//�����_�����O�^�[�Q�b�g�̃N���A
		pD3D11DeviceContext->ClearRenderTargetView(pImpl->m_D3D11RenderTargetView.Get(), Color);
		//�ʏ�̐[�x�o�b�t�@�ƃX�e���V���o�b�t�@�̃N���A
		pD3D11DeviceContext->ClearDepthStencilView(pImpl->m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//�����_�����O�^�[�Q�b�g���J�n����
	void DefaultRenderTarget::StartRenderTarget() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		ID3D11RenderTargetView* pV = pImpl->m_D3D11RenderTargetView.Get();
		//�����_�����O�^�[�Q�b�g�ƃX�e���V����ݒ�
		pD3D11DeviceContext->OMSetRenderTargets(1, &pV, pImpl->m_DepthStencilView.Get());
		//�r���[�|�[�g�̐ݒ�
		auto ViewPort = GetViewport();
		pD3D11DeviceContext->RSSetViewports(1, &ViewPort);

		D3D11_RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (LONG)ViewPort.Width;
		rect.bottom = (LONG)ViewPort.Height;
		pD3D11DeviceContext->RSSetScissorRects(1, &rect);

		//�V�F�[�_�[���\�[�X�r���[�̃N���A
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//�V�F�[�_�[�͎w�肵�Ȃ�
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//�u�����h�͎w�肵�Ȃ�
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	}
	//�����_�����O�^�[�Q�b�g���I������
	void DefaultRenderTarget::EndRenderTarget() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�V�F�[�_�[���\�[�X�r���[�̃N���A
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//�V�F�[�_�[�͎w�肵�Ȃ�
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//�u�����h�͎w�肵�Ȃ�
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	//--------------------------------------------------------------------------------------
	//	struct ShaderResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct ShaderResource::Impl {
		Impl() {}
		~Impl() {}
	};
	ShaderResource::ShaderResource() :pImpl(new Impl()) {}
	ShaderResource::~ShaderResource() {}
	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::ReadBinaryFile(
	//		const wstring& fileName,		//�t�@�C����
	//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^�̖߂�
	//		size_t& CsoSz					//�T�C�Y�̖߂�
	//	);
	//	�p�r: �o�C�i���t�@�C����ǂݍ���
	//--------------------------------------------------------------------------------------
	void ShaderResource::ReadBinaryFile(const wstring& fileName, unique_ptr<uint8_t[]>& Data, size_t& CsoSz) {
		try {
			if (fileName == L"") {
				throw BaseException(
					L"�t�@�C�����w�肳��Ă��܂���",
					L"if(fileName == L\"\")",
					L"ShaderResource::ReadBinaryFile()"
				);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(fileName.c_str());
			if (RetCode == -1) {
				throw BaseException(
					L"�t�@�C�������݂��܂���",
					fileName,
					L"ShaderResource::ReadBinaryFile()"
				);
			}

			ThrowIfFailed(
				BinaryReader::ReadEntireFile(fileName, Data, &CsoSz),
				L"�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B",
				fileName,
				L"ShaderResource::ReadBinaryFile()"
			);

		}
		catch (...) {
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateVertexShader(
	//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
	//		size_t CsoSz,					//�T�C�Y
	//		ID3D11VertexShader** pResult			//�󂯎��V�F�[�_
	//	);
	//	�p�r: �o�C�i���f�[�^���璸�_�V�F�[�_���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateVertexShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11VertexShader** pResult) {
		try {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//���_�V�F�[�_�[�̍쐬
			ThrowIfFailed(
				pDx11Device->CreateVertexShader(Data.get(), CsoSz, nullptr, pResult),
				L"���_�V�F�[�_�̍쐬�Ɏ��s���܂����B",
				L"if( FAILED( pDx11Device->CreateVertexShader() ) )",
				L"ShaderResource::CreateVertexShader()"
			);

		}
		catch (...) {
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateInputLayout(
	//		unique_ptr<uint8_t[]>& Data,	//���_�V�F�[�_�̃o�C�i���f�[�^
	//		size_t CsoSz,					//�T�C�Y
	//		const D3D11_INPUT_ELEMENT_DESC* pElement,	//���_��`
	//		UINT NumElement,							//���_��`�̐�
	//		ID3D11InputLayout** pResult			//�󂯎�郌�C�A�E�g
	//	);
	//	�p�r: �o�C�i���f�[�^����C���v�b�g���C�A�E�g���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateInputLayout(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
		const D3D11_INPUT_ELEMENT_DESC* pElement, UINT NumElement, ID3D11InputLayout** pResult) {
		try {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�C���v�b�g���C�A�E�g�̍쐬
			HRESULT hr = pDx11Device->CreateInputLayout(
				pElement,	//���_��`
				NumElement,		//���_��`�̐�
				Data.get(),
				CsoSz,
				pResult
			);
			//���s������
			if (FAILED(hr))
			{
				throw basecross::BaseException(
					L"���_�V�F�[�_�̃C���v�b�g���C�A�E�g�̍쐬�Ɏ��s���܂����B",
					L"if( FAILED( pDx11Device->CreateInputLayout() ) )",
					L"ShaderResource::CreateInputLayout()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreatePixelShader(
	//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
	//		size_t CsoSz,					//�T�C�Y
	//		ID3D11PixelShader** pResult			//�󂯎��V�F�[�_
	//	);
	//	�p�r: �o�C�i���f�[�^����s�N�Z���V�F�[�_���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreatePixelShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11PixelShader** pResult) {
		try {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�s�N�Z���V�F�[�_�쐬
			HRESULT hr = pDx11Device->CreatePixelShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)) {
				throw basecross::BaseException(
					L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂����B",
					L"f( FAILED( pDx11Device->CreatePixelShader() ) )",
					L"ShaderResource::CreatePixelShader()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateGeometryShader(
	//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
	//		size_t CsoSz,					//�T�C�Y
	//		ID3D11GeometryShader** pResult			//�󂯎��V�F�[�_
	//	);
	//	�p�r: �o�C�i���f�[�^����W�I���g���V�F�[�_���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11GeometryShader** pResult) {
		try {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�W�I���g���V�F�[�_�쐬
			HRESULT hr = pDx11Device->CreateGeometryShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)) {
				throw basecross::BaseException(
					L"�W�I���g���V�F�[�_�̍쐬�Ɏ��s���܂����B",
					L"f( FAILED( pDx11Device->CreateGeometryShader() ) )",
					L"ShaderResource::CreateGeometryShader()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	void ShaderResource::CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
		const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries, UINT Stride, UINT NumStride, ID3D11GeometryShader** pResult) {
		try {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			//�W�I���g���V�F�[�_�쐬
			UINT StridBuff[1] = { Stride };
			HRESULT hr = pDx11Device->CreateGeometryShaderWithStreamOutput(
				Data.get(),
				CsoSz,
				SOEntries.data(),
				(INT)SOEntries.size(),
				StridBuff,
				NumStride,
				0,
				nullptr,
				pResult);
			if (FAILED(hr)) {
				throw basecross::BaseException(
					L"�W�I���g���V�F�[�_�̍쐬�Ɏ��s���܂����B",
					L"f( FAILED( pDx11Device->CreateGeometryShader() ) )",
					L"ShaderResource::CreateGeometryShader()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateComputeShader(
	//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
	//		size_t CsoSz,					//�T�C�Y
	//		ID3D11ComputeShader** pResult			//�󂯎��V�F�[�_
	//	);
	//	�p�r: �o�C�i���f�[�^����R���s���[�g�V�F�[�_���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateComputeShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11ComputeShader** pResult) {
		try {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�s�N�Z���V�F�[�_�쐬
			HRESULT hr = pDx11Device->CreateComputeShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)) {
				throw basecross::BaseException(
					L"�R���s���[�g�V�F�[�_�̍쐬�Ɏ��s���܂����B",
					L"f( FAILED( pDx11Device->CreateComputeShader() ) )",
					L"ShaderResource::CreateComputeShader()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateConstantBuffer(
	//		UINT BuffSize,					//�T�C�Y
	//		ID3D11Buffer** pResult			//�󂯎��V�F�[�_
	//	);
	//	�p�r: �R���X�^���g�o�b�t�@���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateConstantBuffer(UINT BuffSize, ID3D11Buffer** pResult) {
		try {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�R���X�^���g�o�b�t�@�̍쐬
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = BuffSize;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			HRESULT hr = pDx11Device->CreateBuffer(&bd, NULL, pResult);
			if (FAILED(hr)) {
				// ���������s
				throw BaseException(
					L"�R���X�^���g�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if( FAILED( hr ) )",
					L"ShaderResource::CreateConstantBuffer()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
