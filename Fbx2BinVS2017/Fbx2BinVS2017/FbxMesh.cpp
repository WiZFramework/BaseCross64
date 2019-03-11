/*!
@file FbxMesh.cpp
@brief FbxMesh�֘A����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	struct VertexParams {
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		UINT m_NumVertices;				//���_�̐�
		UINT m_NumIndicis;				//�C���f�b�N�X�̐�
	};

	//--------------------------------------------------------------------------------------
	//	Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct FbxMeshResource2::Impl {
		unique_ptr<FbxMesh, FbxMeshDeleter> m_FbxMesh;
		//FBX�V�[��
		weak_ptr<FbxSceneResource> m_FbxSceneResource;
		//FBX���i�󔒂̏ꍇ����j
		string m_FbxName;
		//�ȉ��A�X�L�����b�V���p
		//�X�L�����b�V���̏ꍇnullptr�ȊO�ɂȂ�
		unique_ptr<FbxSkin, FbxSkinDeleter> m_FbxSkin;
		//�P�t���[���̊�ƂȂ鎞��
		FbxTime m_timePeriod;
		//�X�L���A�j���[�V�����Ɏg�p����{�[��
		//�{�[���̔z��
		vector< Bone > m_vecBones;
		//�{�[���𖼑O�ŏƉ�邷��ۂɎg�p����C���f�b�N�X�e�[�u��
		map< string, UINT > m_mapBoneList;
		//�X�L�����b�V���ł������I�ɃX�^�e�B�b�N�œǂނ��ǂ���
		bool m_NeedStatic;
		bool m_WithTangent;

		Impl(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic, bool WithTangent) :
			m_FbxSceneResource(FbxSceneResourcePtr),
			m_FbxSkin(nullptr),
			m_timePeriod(0),
			m_FbxMesh(pFbxMesh),
			m_NeedStatic(NeedStatic),
			m_WithTangent(WithTangent)
		{}
		~Impl() {
		}
	};
	//--------------------------------------------------------------------------------------
	///	FBX���b�V�����\�[�X(MeshResource�h����)
	//--------------------------------------------------------------------------------------
	FbxMeshResource2::FbxMeshResource2(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic,
		bool WithTangent) :
		MeshResource(),
		pImpl(new Impl(FbxSceneResourcePtr, pFbxMesh, NeedStatic, WithTangent))
	{}
	FbxMeshResource2::~FbxMeshResource2() {}

	void FbxMeshResource2::OnCreate() {
		try {
			//�X�L�����b�V�����ǂ����𔻒f
			pImpl->m_FbxSkin.reset(FbxCast< FbxSkin >(pImpl->m_FbxMesh->GetDeformer(0, FbxDeformer::eSkin)));
			//�X�L�����b�V���̏ꍇ
			if (pImpl->m_FbxSkin && !pImpl->m_NeedStatic) {
				CreateInstanceFromSkinFbx();
			}
			//�X�^�e�B�b�N���b�V���������͋����I�ɃX�^�e�B�b�N�œǂޏꍇ
			else {
				pImpl->m_FbxSkin = nullptr;
				CreateInstanceFromStaticFbx();
			}
		}
		catch (...) {
			throw;
		}
	}

	void FbxMeshResource2::GetMaterialVec(vector<MaterialEx>& materials) {
		DWORD MaterialCount = pImpl->m_FbxMesh->GetNode()->GetMaterialCount();
		//�}�e���A���̐ݒ�
		//�e�N�X�`���t�@�C�����쐬�̂��߂̃��[�N�z��
		wchar_t Buff[MAX_PATH];
		setlocale(LC_CTYPE, "jpn");
		if (pImpl->m_FbxSceneResource.expired()) {
			//���s����
			throw BaseException(
				L"FbxSceneResource�������ł�",
				L"if (pImpl->m_FbxSceneResource.expired())",
				L"FbxMeshResource::GetMaterialVec()");

		}
		auto FbxSceneResourcePtr = pImpl->m_FbxSceneResource.lock();
		for (DWORD i = 0; i < MaterialCount; i++) {
			//�}�e���A���擾
			MaterialEx material;
			::ZeroMemory(&material, sizeof(MaterialEx));

			FbxSurfaceMaterial*			pMaterial = pImpl->m_FbxMesh->GetNode()->GetMaterial(i);
			FbxSurfacePhong*			pPhong = (FbxSurfacePhong*)pMaterial;
			FbxPropertyT<FbxDouble3>	color;

			auto impl = GetImplementation(pMaterial, FBXSDK_IMPLEMENTATION_CGFX);
			if (!impl) {
				impl = GetImplementation(pMaterial, FBXSDK_IMPLEMENTATION_HLSL);
			}
			//impl���擾�ł��Ȃ�
			if (!impl) {
				if (pMaterial->GetClassId().Is(FbxSurfacePhong::ClassId)) {
					auto p_phong = (FbxSurfacePhong*)pMaterial;
					//�f�B�t�[�Y
					{
						auto v = p_phong->Diffuse.Get();
						material.m_Diffuse = Col4((float)v[0], (float)v[1], (float)v[2], 1.0f);
					}
					//�A���r�G���g
					{
						auto v = p_phong->Ambient.Get();
						material.m_Ambient = Col4((float)v[0], (float)v[1], (float)v[2], 0);
					}
					//�X�y�L�����[
					{
						auto v = p_phong->Specular.Get();
						material.m_Specular = Col4((float)v[0], (float)v[1], (float)v[2], 0);
					}
					//����
					{
						auto v = p_phong->Shininess.Get();
						//����
					}
					//�X�y�L�����t�@�N�^
					{
						auto v = p_phong->SpecularFactor.Get();
						material.m_Specular.w = (float)v;
					}
					//�G�~�b�V�u
					{
						auto v = p_phong->Emissive.Get();
						material.m_Emissive = Col4((float)v[0], (float)v[1], (float)v[2], 0);
					}
					//���ߓx�i�f�t�B�[�Y�ɐݒ�j
					{
						auto v = p_phong->TransparencyFactor.Get();
						material.m_Diffuse.w = (float)v;
					}
					//���˓x
					{
						auto v = p_phong->ReflectionFactor.Get();
						//����
					}
				}
				else if (pMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
					auto p_lam = (FbxSurfaceLambert*)pMaterial;
					//�f�B�t�[�Y
					{
						auto v = p_lam->Diffuse.Get();
						material.m_Diffuse = Col4((float)v[0], (float)v[1], (float)v[2], 1.0f);
					}
					//�A���r�G���g
					{
						auto v = p_lam->Ambient.Get();
						material.m_Ambient = Col4((float)v[0], (float)v[1], (float)v[2], 0);
					}
					//�G�~�b�V�u
					{
						auto v = p_lam->Emissive.Get();
						material.m_Emissive = Col4((float)v[0], (float)v[1], (float)v[2], 0);
					}
					//���ߓx�i�f�t�B�[�Y�ɐݒ�j
					{
						auto v = p_lam->TransparencyFactor.Get();
						material.m_Diffuse.w = (float)v;
					}
				}
				//�}�e���A���Ɋ֘A�t�����Ă���e�N�X�`����ǂݍ���
				const FbxProperty	fbxProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
				//�e�N�X�`������t�@�C�����𔲂��o��
				FbxFileTexture*	pFbxFileTexture = fbxProperty.GetSrcObject< FbxFileTexture >(i);
				TextureResource* pTexture = 0;
				if (pFbxFileTexture) {
					//�e�N�X�`���t�@�C��������p�X��r�����t�@�C����+�g���q�Ƃ��č�������
					char szTextureFilename[256], szFileExt[8];
					_splitpath_s(pFbxFileTexture->GetFileName(), nullptr, 0, nullptr, 0, szTextureFilename, 256, szFileExt, 8);
					//�t�@�C�����̍���
					string sTextureFilename(szTextureFilename);
					sTextureFilename += szFileExt;
					//UNI�R�[�h�ɕϊ�
					size_t size = 0;
					mbstowcs_s(&size, Buff, sTextureFilename.c_str(), MAX_PATH - 1);
					wstring strWork = FbxSceneResourcePtr->GetDataDir() + Buff;
					//pFbxFileTexture���烉�b�v���[�h���擾���ăe�N�X�`�����쐬
					auto PtrTexture = TextureResource::CreateTextureResource(strWork.c_str());
					material.m_TextureResource = PtrTexture;
				}
				else {
					//�e�N�X�`�����Ȃ��ꍇ��nullptr��ݒ�
					material.m_TextureResource = nullptr;
				}
			}
			//impl���擾�ł���
			else {
				auto p_root_table = impl->GetRootTable();
				auto entry_count = p_root_table->GetEntryCount();
				for (size_t table = 0; table < entry_count; ++table) {
					auto entry_table = p_root_table->GetEntry(table);
					auto entry_type = entry_table.GetEntryType(true);
					FbxProperty prop;
					//�v���p�e�B���擾����
					if (strcmp(FbxPropertyEntryView::sEntryType, entry_type) == 0)
					{
						prop = pMaterial->FindPropertyHierarchical(entry_table.GetSource());
						if (!prop.IsValid())
						{
							prop = pMaterial->RootProperty.FindHierarchical(entry_table.GetSource());
						}
					}
					else if (strcmp(FbxConstantEntryView::sEntryType, entry_type) == 0)
					{
						prop = impl->GetConstants().FindHierarchical(entry_table.GetSource());
					}
					//�v���p�e�B���擾�ł��Ȃ���Ώ������L�����Z��
					if (!prop.IsValid()) {
						continue;
					}
					auto str = entry_table.GetSource();
					//�Ƃ肠�����\�[�X�̖��O�擾
					//�e�N�X�`������t�@�C�����𔲂��o��
					FbxFileTexture*	pFbxFileTexture = prop.GetSrcObject< FbxFileTexture >(i);
					TextureResource* pTexture = 0;
					if (pFbxFileTexture) {
						//�e�N�X�`���t�@�C��������p�X��r�����t�@�C����+�g���q�Ƃ��č�������
						char szTextureFilename[256], szFileExt[8];
						_splitpath_s(pFbxFileTexture->GetFileName(), nullptr, 0, nullptr, 0, szTextureFilename, 256, szFileExt, 8);
						//�t�@�C�����̍���
						string sTextureFilename(szTextureFilename);
						sTextureFilename += szFileExt;
						//UNI�R�[�h�ɕϊ�
						size_t size = 0;
						mbstowcs_s(&size, Buff, sTextureFilename.c_str(), MAX_PATH - 1);
						wstring strWork = FbxSceneResourcePtr->GetDataDir() + Buff;
						//pFbxFileTexture���烉�b�v���[�h���擾���ăe�N�X�`�����쐬
						auto PtrTexture = TextureResource::CreateTextureResource(strWork.c_str());
						material.m_TextureResource = PtrTexture;
					}
					else {
						//�e�N�X�`�����Ȃ��ꍇ��nullptr��ݒ�
						material.m_TextureResource = nullptr;
						//�e�N�X�`������Ȃ���΂Ȃ񂩂Ȃ̂łȂ񂩕K�v�Ȃ��̂�o�^����
						auto source_type_name = std::string(str);
						//���O����Ȃ񂩌�������
						auto Find = [&](const char* a) {
							if (source_type_name.find(a) == std::string::npos) return false;
							return true;
						};
						//�����炭maya��ł̃G�~�b�V�u�l
						if (Find("LitColor")) {
							auto val = prop.Get<FbxDouble3>();
							material.m_Emissive = Col4((float)val[0], (float)val[1], (float)val[2], 0);
						}
						else if (Find("FalloffPower")) {
							auto val = prop.Get<FbxDouble>();
							//����
						}
						else if (Find("SpecularPower")) {
							auto val = prop.Get<FbxDouble>();
							material.m_Specular.w = (float)val;
						}
					}

				}
			}
			//�}�e���A���z��ɒǉ�
			materials.push_back(material);
		}
	}

	void FbxMeshResource2::GetStaticVerticesIndicesMaterials(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices,
		vector<MaterialEx>& materials) {
		vertices.clear();
		indices.clear();
		materials.clear();
		GetMaterialVec(materials);
		auto MaterialCount = materials.size();
		UINT NumVertices = 0;
		//���b�V���P�̂̓ǂݍ���
		DWORD dwNumPolygons = 0;	//�|���S����
									//���_���Ȃ�	
		if ((NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//���s����
			throw BaseException(L"Fbx�ɒ��_������܂���",
				L"m_FbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::GetStaticVerticesIndicesMaterials()");
		}
		//�|���S�����̎擾
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();
		//���_���쐬���邽�߂̔z��
		vertices.resize(NumVertices);
		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//���_���W�E�@���E�e�N�X�`�����W�̎擾
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//�|���S���̃T�C�Y�𓾂�i�ʏ�R�j
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbx���璸�_�𓾂�
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//�@���𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV�l�𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				vertices[iIndex] =
					VertexPositionNormalTexture(
						//���_�̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
						Vec3(static_cast<float>(vPos[0]), static_cast<float>(vPos[1]), -static_cast<float>(vPos[2])),
						//�@���̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
						Vec3(static_cast<float>(vNormal[0]), static_cast<float>(vNormal[1]), -static_cast<float>(vNormal[2])),
						//UV�l�̐ݒ�
						//V�̒l���A1.0����������l�ɂȂ�
						Vec2(static_cast<float>(vUV[0]), 1.0f - static_cast<float>(vUV[1]))
					);
				int tangentCount = pImpl->m_FbxMesh->GetElementTangentCount();
				int binormalCount = pImpl->m_FbxMesh->GetElementBinormalCount();
			}
		}
		//�C���f�b�N�X
		//�}�e���A���̃|�C���^���擾����
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//���_�C���f�b�N�X���œK������(�����}�e���A�����g�p����|���S�����܂Ƃ߂ĕ`��ł���悤�ɕ��ׁA
			//�`�掞�Ƀ}�e���A���̐؂�ւ��񐔂����炷)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast<uint16_t>(pImpl->m_FbxMesh->GetPolygonVertex(j, 2 - k)));
						materials[i].m_IndexCount++;
					}
				}
			}
		}
		//�}�e���A���z��ɃX�^�[�g�n�_��ݒ�
		UINT StarIndex = 0;
		for (DWORD i = 0; i < materials.size(); i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}
	}


	void FbxMeshResource2::GetStaticVerticesIndicesMaterialsWithTangent(vector<VertexPositionNormalTangentTexture>& vertices,
		vector<uint16_t>& indices, vector<MaterialEx>& materials) {
		vertices.clear();
		indices.clear();
		materials.clear();
		GetMaterialVec(materials);
		auto MaterialCount = materials.size();
		UINT NumVertices = 0;
		//���b�V���P�̂̓ǂݍ���
		DWORD dwNumPolygons = 0;	//�|���S����
		//���_���Ȃ�	
		if ((NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//���s����
			throw BaseException(L"Fbx�ɒ��_������܂���",
				L"m_FbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::GetStaticVerticesIndicesMaterialsWithTangent()");
		}
		//�|���S�����̎擾
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();
		//���_���쐬���邽�߂̔z��
		vertices.resize(NumVertices);
		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		auto TanPtr = pImpl->m_FbxMesh->GetLayer(0)->GetTangents();
		//���_���W�E�@���E�e�N�X�`�����W�̎擾
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//�|���S���̃T�C�Y�𓾂�i�ʏ�R�j
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				FbxVector4 fbxTangent;
				//Fbx���璸�_�𓾂�
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//�@���𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV�l�𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				Vec4 Tan(0, 0, 0, 0);
				if (TanPtr) {
					Tan.x = (float)TanPtr->GetDirectArray().GetAt(iIndex)[0];
					Tan.y = (float)TanPtr->GetDirectArray().GetAt(iIndex)[1];
					Tan.z = (float)TanPtr->GetDirectArray().GetAt(iIndex)[2];
					Tan.w = (float)TanPtr->GetDirectArray().GetAt(iIndex)[3];
				}
				vertices[iIndex] =
					VertexPositionNormalTangentTexture(
						//���_�̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
						Vec3(static_cast<float>(vPos[0]), static_cast<float>(vPos[1]), -static_cast<float>(vPos[2])),
						//�@���̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
						//XMFLOAT3(static_cast< float >(-vNormal[0]), -static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
						Vec3(static_cast<float>(vNormal[0]), static_cast<float>(vNormal[1]), -static_cast<float>(vNormal[2])),
						//�^���W�F���g�̐ݒ�
						Tan,
						//UV�l�̐ݒ�
						//V�̒l���A1.0����������l�ɂȂ�
						Vec2(static_cast<float>(vUV[0]), 1.0f - static_cast<float>(vUV[1]))
					);

				int tangentCount = pImpl->m_FbxMesh->GetElementTangentCount();
				int binormalCount = pImpl->m_FbxMesh->GetElementBinormalCount();

			}
		}
		//�C���f�b�N�X
		//�}�e���A���̃|�C���^���擾����
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//���_�C���f�b�N�X���œK������(�����}�e���A�����g�p����|���S�����܂Ƃ߂ĕ`��ł���悤�ɕ��ׁA
			//�`�掞�Ƀ}�e���A���̐؂�ւ��񐔂����炷)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast<uint16_t>(pImpl->m_FbxMesh->GetPolygonVertex(j, 2 - k)));
						materials[i].m_IndexCount++;
					}
				}
			}
		}
		//�}�e���A���z��ɃX�^�[�g�n�_��ݒ�
		UINT StarIndex = 0;
		for (DWORD i = 0; i < materials.size(); i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}
	}


	void FbxMeshResource2::CreateInstanceFromStaticFbx() {
		//����FBX�ɖ��O������΂����ێ�
		if (pImpl->m_FbxMesh->GetName()) {
			pImpl->m_FbxName = pImpl->m_FbxMesh->GetName();
		}
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionNormalTangentTexture> vertices_withtan;
		vector<uint16_t> indices;
		if (pImpl->m_WithTangent) {
			GetStaticVerticesIndicesMaterialsWithTangent(vertices_withtan, indices, GetMaterialExVec());
		}
		else {
			GetStaticVerticesIndicesMaterials(vertices, indices, GetMaterialExVec());
		}
		//�z������Ƃɒ��_�ƃC���f�b�N�X���쐬
		ComPtr<ID3D11Buffer> VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D11Buffer> IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		if (pImpl->m_WithTangent) {
			MeshResource::CreateVertexBuffer(VertexBuffer, vertices_withtan);
		}
		else {
			MeshResource::CreateVertexBuffer(VertexBuffer, vertices);
		}
		MeshResource::CreateIndexBuffer(IndexBuffer, indices);
		SetVertexBuffer(VertexBuffer);
		SetIndexBuffer(IndexBuffer);
		SetNumVertices((UINT)vertices.size());
		SetNumIndicis((UINT)indices.size());
		if (pImpl->m_WithTangent) {
			SetVertexType<VertexPositionNormalTangentTexture>();
		}
		else {
			SetVertexType<VertexPositionNormalTexture>();
		}
	}

	void FbxMeshResource2::GetSkinVerticesIndicesMaterials(vector<VertexPositionNormalTextureSkinning>& vertices,
		vector<uint16_t>& indices, vector<MaterialEx>& materials,
		vector<Bone>& Bones, map< string, UINT >& mapBoneList) {
		vertices.clear();
		indices.clear();
		materials.clear();
		Bones.clear();
		mapBoneList.clear();
		GetMaterialVec(materials);
		auto MaterialCount = materials.size();
		UINT NumVertices = 0;
		//���b�V���P�̂̓ǂݍ���
		DWORD dwNumPolygons = 0;	//�|���S����
		//���_���Ȃ�	
		if ((NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//���s����
			throw BaseException(L"Fbx�ɒ��_������܂���",
				L"m_pFbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::GetSkinVerticesIndicesMaterials()");
		}
		//�|���S�����̎擾
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();

		//���_���쐬���邽�߂̔z��
		vertices.resize(NumVertices);
		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//���_���W�E�@���E�e�N�X�`�����W�̎擾
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//�|���S���̃T�C�Y�𓾂�i�ʏ�R�j
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbx���璸�_�𓾂�
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//�@���𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV�l�𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				uint32_t temp[4] = { 0, 0, 0, 0 };
				float tempf[4] = { 0, 0, 0, 0 };
				vertices[iIndex] =
					VertexPositionNormalTextureSkinning(
						//���_�̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
						Vec3(static_cast<float>(vPos[0]), static_cast<float>(vPos[1]), -static_cast<float>(vPos[2])),
						//�@���̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
						Vec3(static_cast<float>(vNormal[0]), static_cast<float>(vNormal[1]), -static_cast<float>(vNormal[2])),
						//UV�l�̐ݒ�
						//V�̒l���A1.0����������l�ɂȂ�
						Vec2(static_cast<float>(vUV[0]), 1.0f - static_cast<float>(vUV[1])),
						//�u�����h�C���f�b�N�X�͂Ƃ肠����0
						temp,
						//�u�����h�E�G�C�g�͂Ƃ肠����0
						tempf
					);
			}
		}

		//�u�����h�E�F�C�g�ƃu�����h�C���f�b�N�X�̓ǂݍ���
		const int	iNumCluster = pImpl->m_FbxSkin->GetClusterCount();
		// �ϊ����� FbxSkin ���� �N���X�^�[(�{�[��)�̐����擾����

		for (int i = 0; i < iNumCluster; i++) {
			int		iNumBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndicesCount();	// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�̐����擾����
			int*	piBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndices();		// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�̔ԍ���z��Ŏ擾����
			double*	pdBlendWeights = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointWeights();		// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�ɑΉ������d�݃f�[�^��z��Ŏ擾����

			for (int j = 0; j < iNumBlendIndices; j++) {
				int	idx = piBlendIndices[j];

				//���_�ɓo�^����S�̃u�����h�E�F�C�g�̂����A�ŏ��̒l�����v�f����������iDirectX9�̌Œ�V�F�[�_�ł͂S�̃{�[�����炵���u�����h�ł��Ȃ��j
				int	iMinIndex = 0;
				for (int k = 0; k < 4 - 1; k++) {
					for (int l = k + 1; l < 4; l++) {
						if (vertices[idx].weights[k] < vertices[idx].weights[l]) {
							iMinIndex = k;
						}
						else {
							iMinIndex = l;
							k = l;
							break;
						}
					}
				}
				//���łɓo�^����Ă��钆�ōŏ��̃u�����h�E�F�C�g�����傫���l�����f�[�^��o�^����
				if (static_cast<float>(pdBlendWeights[j]) > vertices[idx].weights[iMinIndex]) {
					vertices[idx].indices[iMinIndex] = static_cast<BYTE>(i);
					vertices[idx].weights[iMinIndex] = static_cast<float>(pdBlendWeights[j]);
				}
			}
		}
		//�E�G�C�g�̃`�F�b�N
		//�e���_���ƂɃE�F�C�g�𑫂���1.0f�ɂȂ�Ȃ��ƃX�L�������܂��ł��Ȃ�
		for (size_t i = 0; i < vertices.size(); i++) {
			float f = vertices[i].weights[0] + vertices[i].weights[1] + vertices[i].weights[2] + vertices[i].weights[3];
			if (f > 0 && f < 1.0f) {
				float k = 1.0f / f;
				vertices[i].weights[0] *= k;
				vertices[i].weights[1] *= k;
				vertices[i].weights[2] *= k;
				vertices[i].weights[3] *= k;
			}
		}

		//��^�C�}�[�̐ݒ�
		FbxGlobalSettings&	globalTimeSettings = pImpl->m_FbxMesh->GetScene()->GetGlobalSettings();
		FbxTime::EMode timeMode = globalTimeSettings.GetTimeMode();
		pImpl->m_timePeriod.SetTime(0, 0, 0, 1, 0, timeMode);

		//�C���f�b�N�X�o�b�t�@�̍쐬
		//�}�e���A���̃|�C���^���擾����
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//���_�C���f�b�N�X���œK������(�����}�e���A�����g�p����|���S�����܂Ƃ߂ĕ`��ł���悤�ɕ��ׁA
			//�`�掞�Ƀ}�e���A���̐؂�ւ��񐔂����炷)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast<uint16_t>(pImpl->m_FbxMesh->GetPolygonVertex(j, 2 - k)));
						materials[i].m_IndexCount++;
					}
				}
			}
		}
		//�}�e���A���z��ɃX�^�[�g�n�_��ݒ�
		UINT StarIndex = 0;
		for (DWORD i = 0; i < materials.size(); i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}

		//�{�[�����𓾂�
		auto NumBones = (UINT)pImpl->m_FbxSkin->GetClusterCount();
		for (UINT i = 0; i < NumBones; i++) {
			Bone	bone;

			FbxAMatrix	mBindPose, mCurrentPose;
			pImpl->m_FbxSkin->GetCluster(i)->GetTransformLinkMatrix(mBindPose);
			mCurrentPose = pImpl->m_FbxSkin->GetCluster(i)->GetLink()->EvaluateGlobalTransform(pImpl->m_timePeriod * 0);
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					bone.m_BindPose(r, c) = static_cast<float>(mBindPose.Get(r, c));
					bone.m_CurrentPose(r, c) = static_cast<float>(mCurrentPose.Get(r, c));
				}
			}
			Mat4x4	mMirror, mBindInverse;
			mMirror.identity();
			mMirror(2, 2) = -1.0f;
			bone.m_BindPose *= mMirror;
			bone.m_CurrentPose *= mMirror;
			mBindInverse = inverse(bone.m_BindPose);
			bone.m_ConbinedPose = mBindInverse * bone.m_CurrentPose;

			Bones.push_back(bone);
			//�}�b�v�̓o�^
			mapBoneList[pImpl->m_FbxSkin->GetCluster(i)->GetName()] = i;
		}
	}

	void FbxMeshResource2::GetSkinVerticesIndicesMaterialsWithTangent(vector<VertexPositionNormalTangentTextureSkinning>& vertices,
		vector<uint16_t>& indices, vector<MaterialEx>& materials,
		vector<Bone>& Bones, map< string, UINT >& mapBoneList) {
		vertices.clear();
		indices.clear();
		materials.clear();
		Bones.clear();
		mapBoneList.clear();
		GetMaterialVec(materials);
		auto MaterialCount = materials.size();
		UINT NumVertices = 0;
		//���b�V���P�̂̓ǂݍ���
		DWORD dwNumPolygons = 0;	//�|���S����
									//���_���Ȃ�	
		if ((NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//���s����
			throw BaseException(L"Fbx�ɒ��_������܂���",
				L"m_pFbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::GetSkinVerticesIndicesMaterials()");
		}
		//�|���S�����̎擾
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();

		//���_���쐬���邽�߂̔z��
		vertices.resize(NumVertices);
		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		auto TanPtr = pImpl->m_FbxMesh->GetLayer(0)->GetTangents();
		//���_���W�E�@���E�e�N�X�`�����W�̎擾
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//�|���S���̃T�C�Y�𓾂�i�ʏ�R�j
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbx���璸�_�𓾂�
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//�@���𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV�l�𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				uint32_t temp[4] = { 0, 0, 0, 0 };
				float tempf[4] = { 0, 0, 0, 0 };

				Vec4 Tan(0, 0, 0, 0);
				if (TanPtr) {
					Tan.x = (float)TanPtr->GetDirectArray().GetAt(iIndex)[0];
					Tan.y = (float)TanPtr->GetDirectArray().GetAt(iIndex)[1];
					Tan.z = (float)TanPtr->GetDirectArray().GetAt(iIndex)[2];
					Tan.w = (float)TanPtr->GetDirectArray().GetAt(iIndex)[3];
				}

				vertices[iIndex] =
					VertexPositionNormalTangentTextureSkinning(
						//���_�̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
						Vec3(static_cast<float>(vPos[0]), static_cast<float>(vPos[1]), -static_cast<float>(vPos[2])),
						//�@���̐ݒ�
						//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
						Vec3(static_cast<float>(vNormal[0]), static_cast<float>(vNormal[1]), -static_cast<float>(vNormal[2])),
						//�^���W�F���g�̐ݒ�
						Tan,
						//UV�l�̐ݒ�
						//V�̒l���A1.0����������l�ɂȂ�
						Vec2(static_cast<float>(vUV[0]), 1.0f - static_cast<float>(vUV[1])),
						//�u�����h�C���f�b�N�X�͂Ƃ肠����0
						temp,
						//�u�����h�E�G�C�g�͂Ƃ肠����0
						tempf
					);
			}
		}

		//�u�����h�E�F�C�g�ƃu�����h�C���f�b�N�X�̓ǂݍ���
		const int	iNumCluster = pImpl->m_FbxSkin->GetClusterCount();
		// �ϊ����� FbxSkin ���� �N���X�^�[(�{�[��)�̐����擾����

		for (int i = 0; i < iNumCluster; i++) {
			int		iNumBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndicesCount();	// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�̐����擾����
			int*	piBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndices();		// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�̔ԍ���z��Ŏ擾����
			double*	pdBlendWeights = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointWeights();		// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�ɑΉ������d�݃f�[�^��z��Ŏ擾����

			for (int j = 0; j < iNumBlendIndices; j++) {
				int	idx = piBlendIndices[j];

				//���_�ɓo�^����S�̃u�����h�E�F�C�g�̂����A�ŏ��̒l�����v�f����������iDirectX9�̌Œ�V�F�[�_�ł͂S�̃{�[�����炵���u�����h�ł��Ȃ��j
				int	iMinIndex = 0;
				for (int k = 0; k < 4 - 1; k++) {
					for (int l = k + 1; l < 4; l++) {
						if (vertices[idx].weights[k] < vertices[idx].weights[l]) {
							iMinIndex = k;
						}
						else {
							iMinIndex = l;
							k = l;
							break;
						}
					}
				}
				//���łɓo�^����Ă��钆�ōŏ��̃u�����h�E�F�C�g�����傫���l�����f�[�^��o�^����
				if (static_cast<float>(pdBlendWeights[j]) > vertices[idx].weights[iMinIndex]) {
					vertices[idx].indices[iMinIndex] = static_cast<BYTE>(i);
					vertices[idx].weights[iMinIndex] = static_cast<float>(pdBlendWeights[j]);
				}
			}
		}
		//�E�G�C�g�̃`�F�b�N
		//�e���_���ƂɃE�F�C�g�𑫂���1.0f�ɂȂ�Ȃ��ƃX�L�������܂��ł��Ȃ�
		for (size_t i = 0; i < vertices.size(); i++) {
			float f = vertices[i].weights[0] + vertices[i].weights[1] + vertices[i].weights[2] + vertices[i].weights[3];
			if (f > 0 && f < 1.0f) {
				float k = 1.0f / f;
				vertices[i].weights[0] *= k;
				vertices[i].weights[1] *= k;
				vertices[i].weights[2] *= k;
				vertices[i].weights[3] *= k;
			}
		}

		//��^�C�}�[�̐ݒ�
		FbxGlobalSettings&	globalTimeSettings = pImpl->m_FbxMesh->GetScene()->GetGlobalSettings();
		FbxTime::EMode timeMode = globalTimeSettings.GetTimeMode();
		pImpl->m_timePeriod.SetTime(0, 0, 0, 1, 0, timeMode);

		//�C���f�b�N�X�o�b�t�@�̍쐬
		//�}�e���A���̃|�C���^���擾����
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//���_�C���f�b�N�X���œK������(�����}�e���A�����g�p����|���S�����܂Ƃ߂ĕ`��ł���悤�ɕ��ׁA
			//�`�掞�Ƀ}�e���A���̐؂�ւ��񐔂����炷)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast<uint16_t>(pImpl->m_FbxMesh->GetPolygonVertex(j, 2 - k)));
						materials[i].m_IndexCount++;
					}
				}
			}
		}
		//�}�e���A���z��ɃX�^�[�g�n�_��ݒ�
		UINT StarIndex = 0;
		for (DWORD i = 0; i < materials.size(); i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}

		//�{�[�����𓾂�
		auto NumBones = (UINT)pImpl->m_FbxSkin->GetClusterCount();
		for (UINT i = 0; i < NumBones; i++) {
			Bone	bone;

			FbxAMatrix	mBindPose, mCurrentPose;
			pImpl->m_FbxSkin->GetCluster(i)->GetTransformLinkMatrix(mBindPose);
			mCurrentPose = pImpl->m_FbxSkin->GetCluster(i)->GetLink()->EvaluateGlobalTransform(pImpl->m_timePeriod * 0);

			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					bone.m_BindPose(r, c) = static_cast<float>(mBindPose.Get(r, c));
					bone.m_CurrentPose(r, c) = static_cast<float>(mCurrentPose.Get(r, c));
				}
			}

			Mat4x4	mMirror, mBindInverse;
			mMirror.identity();
			mMirror(2, 2) = -1.0f;

			bone.m_BindPose *= mMirror;
			bone.m_CurrentPose *= mMirror;
			mBindInverse = inverse(bone.m_BindPose);
			bone.m_ConbinedPose = mBindInverse * bone.m_CurrentPose;

			Bones.push_back(bone);
			//�}�b�v�̓o�^
			mapBoneList[pImpl->m_FbxSkin->GetCluster(i)->GetName()] = i;
		}
	}



	void FbxMeshResource2::CreateInstanceFromSkinFbx() {
		//����FBX�ɖ��O������΂����ێ�
		if (pImpl->m_FbxMesh->GetName()) {
			pImpl->m_FbxName = pImpl->m_FbxMesh->GetName();
		}
		vector<VertexPositionNormalTextureSkinning> vertices;
		vector<VertexPositionNormalTangentTextureSkinning> vertices_withtan;
		vector<uint16_t> indices;
		if (pImpl->m_WithTangent) {
			GetSkinVerticesIndicesMaterialsWithTangent(vertices_withtan, indices, GetMaterialExVec(),
				pImpl->m_vecBones, pImpl->m_mapBoneList);

		}
		else {
			GetSkinVerticesIndicesMaterials(vertices, indices, GetMaterialExVec(),
				pImpl->m_vecBones, pImpl->m_mapBoneList);
		}

		//�z������Ƃɒ��_�ƃC���f�b�N�X���쐬
		ComPtr<ID3D11Buffer> VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D11Buffer> IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		if (pImpl->m_WithTangent) {
			MeshResource::CreateVertexBuffer(VertexBuffer, vertices_withtan);
		}
		else {
			MeshResource::CreateVertexBuffer(VertexBuffer, vertices);
		}

		MeshResource::CreateIndexBuffer(IndexBuffer, indices);
		SetVertexBuffer(VertexBuffer);
		SetIndexBuffer(IndexBuffer);
		SetNumVertices((UINT)vertices.size());
		SetNumIndicis((UINT)indices.size());
		if (pImpl->m_WithTangent) {
			SetVertexType<VertexPositionNormalTangentTextureSkinning>();
		}
		else {
			SetVertexType<VertexPositionNormalTextureSkinning>();
		}


	}
	shared_ptr<FbxMeshResource2> FbxMeshResource2::CreateFbxMeshResource(shared_ptr<FbxSceneResource> FbxSceneResourcePtr,
		FbxMesh* pFbxMesh, bool NeedStatic, bool WithTangent) {
		return ObjectFactory::Create<FbxMeshResource2>(FbxSceneResourcePtr, pFbxMesh, NeedStatic, WithTangent);
	}

	bool FbxMeshResource2::IsSkining() const {
		return (pImpl->m_FbxSkin && !pImpl->m_NeedStatic);
	}

	const unique_ptr<FbxMesh, FbxMeshResource2::FbxMeshDeleter>& FbxMeshResource2::GetFbxMesh()const {
		return pImpl->m_FbxMesh;
	}
	const unique_ptr<FbxSkin, FbxMeshResource2::FbxSkinDeleter>& FbxMeshResource2::GetFbxSkin() const {
		return pImpl->m_FbxSkin;
	}

	const vector<Bone>& FbxMeshResource2::GetBonesVec() const {
		return pImpl->m_vecBones;
	}


	//
	bool FbxMeshResource2::GenerateCurrentPose(vector<Bone>& tgtBones, AnimationData& rAnimationData, float CurrentTime) {
		//�o�ߎ��Ԃ���t���[�������߂�
		float fCurrentFrame = CurrentTime * rAnimationData.m_SamplesParSecond;
		//�A�j���[�V�������Ō�܂ł����Ă邩�ǂ������m�F
		bool ret = false;
		if (!rAnimationData.m_IsLoop && ((UINT)fCurrentFrame >= rAnimationData.m_SampleLength)) {
			//���[�v���Ȃ��ŁA�A�j�����Ō�ɓ��B������
			ret = true;
		}
		int iAnimationFrame = static_cast<int>(fCurrentFrame * 10000.0f);

		if (rAnimationData.m_SampleLength > 0) {
			if (rAnimationData.m_IsLoop) {
				//���[�v����
				iAnimationFrame %= rAnimationData.m_SampleLength * 10000;
				iAnimationFrame += rAnimationData.m_StartSample * 10000;
			}
			else {
				//���[�v���Ȃ�
				if (ret) {
					//�Ō�̃t���[���Ƀ|�[�Y��ݒ肷��
					iAnimationFrame = (rAnimationData.m_StartSample + rAnimationData.m_SampleLength) * 10000;
				}
				else {
					iAnimationFrame += rAnimationData.m_StartSample * 10000;
				}
			}
		}
		FbxAMatrix	mCurrentPose;
		Mat4x4	mMirror, mBindInverse;
		mMirror.identity();
		mMirror(2, 2) = -1.0f;	//���W�n�̊֌W�Ń{�[�������]���Ă���̂ŁA�����DirectX�ɍ��킹�邽�߂Ɏg�p����s��

								//�J�����g�s��ƍ����s����Čv�Z����
		for (UINT i = 0; i < GetNumBones(); i++) {
			mCurrentPose = pImpl->m_FbxSkin->GetCluster(i)->GetLink()->EvaluateGlobalTransform((pImpl->m_timePeriod * iAnimationFrame) / 10000);
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					tgtBones[i].m_CurrentPose(r, c) = static_cast<float>(mCurrentPose.Get(r, c));
				}
			}
			tgtBones[i].m_CurrentPose *= mMirror;

			mBindInverse = inverse(tgtBones[i].m_BindPose);

			tgtBones[i].m_ConbinedPose = mBindInverse * tgtBones[i].m_CurrentPose;
		}
		rAnimationData.m_IsAnimeEnd = ret;
		return ret;
	}


	//--------------------------------------------------------------------------------------
	//	Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct FbxSceneResource::Impl {
		//�f�[�^�f�B���N�g����
		wstring m_DataDir;
		//FBX�t�@�C����
		wstring m_FileName;
		//FBX�V�[����
		string m_FbxSceneName;
		//FBX�V�[��
		shared_ptr<FbxScene> m_FbxScene;
		//�����I��static�œǂނ��ǂ���
		bool m_NeedStatic;
		//�^���W�F���g��ǂނ�
		bool m_WithTangent;
		//�P���FBX���b�V���̔z��
		vector< shared_ptr<FbxMeshResource2> > m_FbxMeshResourceVec;
		Impl(const wstring& DataDir,
			const wstring& FileName, const string& SceneName, bool  NeedStatic, bool WithTangent) :
			m_DataDir(DataDir),
			m_FileName(FileName),
			m_FbxSceneName(SceneName),
			m_NeedStatic(NeedStatic),
			m_WithTangent(WithTangent)
		{}
		~Impl() {}

	};

	//--------------------------------------------------------------------------------------
	///	FBX�V�[�����\�[�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FbxSceneResource::FbxSceneResource(const wstring& DataDir,
		const wstring& FileName, const string& SceneName, bool NeedStatic, bool WithTangent) :
		BaseResource(),
		pImpl(new Impl(DataDir, FileName, SceneName, NeedStatic, WithTangent))
	{
		try {
			if (FileName == L"") {
				//���s����
				throw BaseException(
					L"Fbx�t�@�C�����������ł�",
					L"if(FileName == \"\")",
					L"FbxSceneResource::FbxSceneResource()");
			}

			if (pImpl->m_FbxSceneName == "") {
				if (FileName == L"") {
					//���s����
					throw BaseException(
						L"�t�@�C�����������ł�",
						L"if(FileName == \"\")",
						L"FbxSceneResource::FbxSceneResource()");
				}
				wstring strWork;
				strWork = pImpl->m_FileName;
				//�t�@�C�������}���`�o�C�g�ɕϊ����V�[�������쐬
				Util::WStoMB(strWork, pImpl->m_FbxSceneName);
				pImpl->m_FbxSceneName += ".scene";
			}
		}
		catch (...) {
			throw;
		}
	}
	FbxSceneResource::~FbxSceneResource() {}

	//static�\�z
	shared_ptr<FbxSceneResource> FbxSceneResource::CreateFbxScene(const wstring& DataDir,
		const wstring& FileName, const string& SceneName, bool NeedStatic, bool WithTangent) {
		try {
			return ObjectFactory::Create<FbxSceneResource>(DataDir, FileName, SceneName, NeedStatic, WithTangent);
		}
		catch (...) {
			throw;
		}
	}

	//�ċA�I�ɃV�[����ǂݍ���
	void FbxSceneResource::CreateFbxMeshVector(FbxNode* pFbxNode) {
		//�m�[�h���L���łȂ���΃��^�[��
		if (!pFbxNode) {
			return;
		}
		//FBX�V�[���̎擾
		FbxScene* pScene = pFbxNode->GetScene();
		//�m�[�h�̑������擾
		FbxNodeAttribute* pFbxNodeAttribute = pFbxNode->GetNodeAttribute();
		//�m�[�h�̑������L���ł���΁A�����̃^�C�v�𔻒肵�A�^�C�v�����b�V���ł����vector�ɓo�^����
		if (pFbxNodeAttribute) {
			if (pFbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
				//�O�p����
				FbxGeometryConverter converter(pFbxNode->GetFbxManager());
				converter.Triangulate(pScene, true);
				//���b�V�������o��
				FbxMesh* pFbxMesh = pFbxNode->GetMesh();
				//�V�[���̃O���[�o���s����擾
				FbxAnimEvaluator* pFbxAnimEvaluator = pScene->GetAnimationEvaluator();
				FbxMatrix mGlobal = pFbxAnimEvaluator->GetNodeGlobalTransform(pFbxNode);
				//�O���[�o���s������ׂĂ̒��_�ɓK�p
				int	iNumControlPoints = pFbxMesh->GetControlPointsCount();
				FbxVector4*	v = pFbxMesh->GetControlPoints();
				for (int i = 0; i < iNumControlPoints; i++) {
					v[i] = mGlobal.MultNormalize(v[i]);
				}
				//Fbx���b�V���̔z��ɓo�^
				auto FbxMeshPtr = FbxMeshResource2::CreateFbxMeshResource(GetThis<FbxSceneResource>(), pFbxMesh, pImpl->m_NeedStatic,
					pImpl->m_WithTangent);
				pImpl->m_FbxMeshResourceVec.push_back(
					FbxMeshPtr
				);
			}
		}
		//�q�m�[�h�̐����擾����
		int iNumChild = pFbxNode->GetChildCount();
		for (int i = 0; i < iNumChild; i++) {
			//�q�m�[�h���ׂĂ��ċA�I�Ƀ��b�V����o�^����
			CreateFbxMeshVector(pFbxNode->GetChild(i));
		}
	}

	void FbxSceneResource::OnCreate() {
		try {

			//�C���|�[�^�[���쐬����
			FbxImporter* lImporter = FbxImporter::Create(App::GetApp()->GetScene<Scene>()->GetFbxManager().get(), "");
			//�t�@�C�������}���`�o�C�g�ɂ���
			wstring strWork;
			strWork = pImpl->m_DataDir + pImpl->m_FileName;
			//�}���`�o�C�g�ɕϊ�
			string sFileName;
			Util::WStoMB(strWork, sFileName);
			//�C���|�[�^�[�̏�����
			if (!lImporter->Initialize(sFileName.c_str(), -1, App::GetApp()->GetScene<Scene>()->GetFbxManager().get()->GetIOSettings())) {
				//���s����
				throw BaseMBException(
					"�C���|�[�^�̏������Ɏ��s���܂���",
					lImporter->GetStatus().GetErrorString(),
					"FbxSceneResource::FbxSceneResource()");
			}
			//�V�[���̍쐬
			pImpl->m_FbxScene = shared_ptr<FbxScene>(
				FbxScene::Create(
					App::GetApp()->GetScene<Scene>()->GetFbxManager().get(),
					pImpl->m_FbxSceneName.c_str()),	//���|�C���^
				FbxSceneDeleter()	//�J�X�^���f���[�^
				);
			//�C���|�[�^�ɂ��V�[���ւ̓ǂݍ���
			lImporter->Import(pImpl->m_FbxScene.get());
			//�C���|�[�^�͂����K�v�Ȃ�
			lImporter->Destroy();


			//�ċA�I�ɓǂݍ���
			CreateFbxMeshVector(pImpl->m_FbxScene->GetRootNode());


		}
		catch (...) {
			throw;
		}
	}

	//�A�N�Z�T
	wstring FbxSceneResource::GetDataDir() const {
		return pImpl->m_DataDir;
	}
	wstring FbxSceneResource::GetFileName() const {
		return pImpl->m_FileName;
	}
	string FbxSceneResource::GetFbxSceneName() const {
		return pImpl->m_FbxSceneName;
	}
	shared_ptr<FbxMeshResource2> FbxSceneResource::GetFbxMeshResource(size_t Index) const {
		if (Index >= pImpl->m_FbxMeshResourceVec.size()) {
			auto str = Util::UintToWStr((UINT)Index);
			str += L" >= m_FbxMeshResourceVec.size()";
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				str,
				L"FbxSceneResource::GetMeshResource()"
			);
		}
		return pImpl->m_FbxMeshResourceVec[Index];
	}
	size_t FbxSceneResource::GetFbxMeshResourceSize() const {
		return pImpl->m_FbxMeshResourceVec.size();
	}
	vector< shared_ptr<FbxMeshResource2> >& FbxSceneResource::GetFbxMeshResourceVec() const {
		return pImpl->m_FbxMeshResourceVec;
	}

	//--------------------------------------------------------------------------------------
	//	BcFbxPNTBoneModelDraw::Impl
	//--------------------------------------------------------------------------------------
	struct BcFbxPNTBoneModelDraw::Impl {
		string m_sCurrentAnimationName;	//���ݏ����ΏۂɂȂ��Ă���A�j���[�V�����̖��O
		float m_CurrentTime;			//�A�j���[�V�����̌��݂̌o�ߎ��ԁi�b�j
		bool m_IsAnimeEnd;				//���݂̃A�j���[�V�������I���������ǂ���
		vector< Bone > m_vecLocalBones;	//�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă���
										//�V�F�[�_�ɓn���{�[���s��
		vector<Mat4x4> m_LocalBonesMatrix;	//�V���h�E�}�b�v�ɓn���|�C���^

												//�A�j���[�V�����𖼑O�ŏƉ�邷��ۂɎg�p����C���f�b�N�X�e�[�u��
		map< string, AnimationData > m_AnimationMap;
		Impl() :
			m_sCurrentAnimationName(""),
			m_CurrentTime(0),
			m_IsAnimeEnd(false)
		{}
		~Impl() {}
	};
	//--------------------------------------------------------------------------------------
	///	BcFbxPNTBoneModelDraw�`��R���|�[�l���g�i�{�[�����f���`��p�j
	//--------------------------------------------------------------------------------------

	BcFbxPNTBoneModelDraw::BcFbxPNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		BcPNTStaticModelDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g�̂RD
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	BcFbxPNTBoneModelDraw::~BcFbxPNTBoneModelDraw() {}

	//�{�[���s������\�[�X����ǂݍ���
	void BcFbxPNTBoneModelDraw::SetBoneVec() {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->GetNumBones() > 0) {
			//���[�J���{�[���̃I���W�i������̃R�s�[
			pImpl->m_vecLocalBones.resize(PtrMeshResource->GetBonesVec().size());
			pImpl->m_vecLocalBones = PtrMeshResource->GetBonesVec();

			pImpl->m_LocalBonesMatrix.resize(PtrMeshResource->GetNumBones());
		}
		else {
			throw BaseException(
				L"�{�[����������܂���",
				L"if (PtrMeshResource->GetNumBones() <= 0)",
				L"BcFbxPNTBoneModelDraw::SetBoneVec()"
			);
		}
	}
	void BcFbxPNTBoneModelDraw::GetBoneVec(vector< Bone >& Bones) {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->GetNumBones() > 0) {
			//���[�J���{�[���̃I���W�i������̃R�s�[
			Bones.resize(PtrMeshResource->GetBonesVec().size());
			Bones = PtrMeshResource->GetBonesVec();
		}
		else {
			throw BaseException(
				L"�{�[����������܂���",
				L"if (PtrMeshResource->GetNumBones() <= 0)",
				L"BcFbxPNTBoneModelDraw::GetBoneVec()"
			);
		}
	}


	//�A�N�Z�T
	//�e�I�u�W�F�N�g���Ƃ̃{�[��
	const vector< Bone >& BcFbxPNTBoneModelDraw::GetVecLocalFbxBones() const {
		return pImpl->m_vecLocalBones;
	}
	const vector< Bone >* BcFbxPNTBoneModelDraw::GetVecLocalFbxBonesPtr() const {
		return &pImpl->m_vecLocalBones;
	}

	const vector< Mat4x4 >* BcFbxPNTBoneModelDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_LocalBonesMatrix;
	}

	void BcFbxPNTBoneModelDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		if (!MeshRes->IsSkining()) {
			throw BaseException(
				L"���b�V�����{�[�����b�V���ł͂���܂���",
				L"if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0)",
				L"BcPNTBoneModelDraw::SetMeshResource()"
			);

		}
		BcPNTStaticModelDraw::SetMeshResource(MeshRes);
		//�{�[��������Γǂݍ���
		SetBoneVec();
	}

	void BcFbxPNTBoneModelDraw::SetMeshResource(const wstring& MeshKey) {
		BcFbxPNTBoneModelDraw::SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}



	void BcFbxPNTBoneModelDraw::AddAnimation(const char* Name, UINT StartFrame, UINT FrameLength, bool Loop,
		float FramesParSecond) {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->IsSkining()) {
			map< string, AnimationData >::iterator it
				= pImpl->m_AnimationMap.find(Name);
			if (it != pImpl->m_AnimationMap.end()) {
				//�w��̖��O����������
				//���̃f�[�^�ɍ����ւ�
				it->second.m_StartSample = StartFrame;
				it->second.m_SampleLength = FrameLength;
				it->second.m_SamplesParSecond = FramesParSecond;
				it->second.m_IsLoop = Loop;
			}
			else {
				//������Ȃ�
				//�A�j���[�V������`�̒ǉ�
				pImpl->m_AnimationMap[Name] = AnimationData(StartFrame, FrameLength,
					Loop, FramesParSecond);
			}
		}
	}
	const AnimationData& BcFbxPNTBoneModelDraw::GetAnimationData(const string& AnimeName) const {
		auto it = pImpl->m_AnimationMap.find(AnimeName);
		if (it != pImpl->m_AnimationMap.end()) {
			return it->second;
		}
		else {
			throw BaseMBException(
				"�w��̃A�j���[�V������������܂���",
				AnimeName,
				"AnimationData& BcFbxPNTBoneModelDraw::GetAnimationData()");
		}
	}


	const string& BcFbxPNTBoneModelDraw::GetCurrentAnimation() const {
		return pImpl->m_sCurrentAnimationName;
	}
	void BcFbxPNTBoneModelDraw::SetCurrentAnimation(const string& AnemationName, float StartTime) {
		//�w��̃A�j���[�V���������邩�ǂ����`�F�b�N
		//������Η�O���o��
		AnimationData animData = GetAnimationData(AnemationName);
		pImpl->m_sCurrentAnimationName = AnemationName;
		pImpl->m_CurrentTime = StartTime;
		pImpl->m_IsAnimeEnd = false;
	}

	float BcFbxPNTBoneModelDraw::GetCurrentTime() const {
		return pImpl->m_CurrentTime;
	}


	//���݂̃A�j���[�V�������I�����Ă��邩�ǂ���
	bool BcFbxPNTBoneModelDraw::IsTargetAnimeEnd() const {
		return pImpl->m_IsAnimeEnd;
	}

	//�w�肵��ID�̃{�[���̌��݂̍s����擾����
	void BcFbxPNTBoneModelDraw::GetBoneMatrix(UINT BoneId, Mat4x4& Matrix) const {
		auto PtrGameObject = GetGameObject();
		auto PtrT = PtrGameObject->GetComponent<Transform>();
		Mat4x4 MeshMatrix = GetMeshToTransformMatrix() * PtrT->GetWorldMatrix();
		if (pImpl->m_vecLocalBones.size() <= BoneId) {
			throw BaseException(
				L"�{�[��ID���͈͊O�ł�",
				L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
				L"BcFbxPNTBoneModelDraw::GetBoneMatrix()"
			);
		}
		Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
	}

	//�w�肵��ID�̃{�[���̌��݂̃��[�J���s����擾����i�e�q�֌W���\�z����ȂǗp�j
	void BcFbxPNTBoneModelDraw::GetLocalBoneMatrix(UINT BoneId, Mat4x4& Matrix) const {
		auto PtrGameObject = GetGameObject();
		auto PtrT = PtrGameObject->GetComponent<Transform>();
		Mat4x4 MeshMatrix = GetMeshToTransformMatrix();
		if (pImpl->m_vecLocalBones.size() <= BoneId) {
			throw BaseException(
				L"�{�[��ID���͈͊O�ł�",
				L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
				L"BcFbxPNTBoneModelDraw::GetLocalBoneMatrix()"
			);
		}
		Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
	}

	void BcFbxPNTBoneModelDraw::OnCreate() {
		SetLightingEnabled(true);
		//�}���`���C�g�̐ݒ�
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}


	bool BcFbxPNTBoneModelDraw::UpdateAnimation(float ElapsedTime) {
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		//�{�[�����o�^����Ă�����{�[����T��
		if (PtrMeshResource->IsSkining()) {
			if (!pImpl->m_IsAnimeEnd) {
				pImpl->m_CurrentTime += ElapsedTime;
			}
			//�A�j���[�V�����f�[�^�̎擾
			//�`�F�b�N���邾���Ȃ̂Ńe���|�����ł悢
			AnimationData animData = GetAnimationData(pImpl->m_sCurrentAnimationName);
			//�{�[�������݂̎��ԂɍX�V����
			//�߂�l�͏I�����Ă邩�ǂ���
			pImpl->m_IsAnimeEnd = PtrMeshResource->GenerateCurrentPose(pImpl->m_vecLocalBones, animData, pImpl->m_CurrentTime);
			if (animData.m_IsLoop) {
				float TotalTime = (float)animData.m_SampleLength / (float)animData.m_SamplesParSecond;
				if (TotalTime <= pImpl->m_CurrentTime) {
					pImpl->m_CurrentTime = 0.0f;
				}
			}
			//�V���h�E�}�b�v�p�̍s��ɃR�s�[
			size_t BoneSz = pImpl->m_vecLocalBones.size();
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				pImpl->m_LocalBonesMatrix[b] = pImpl->m_vecLocalBones[b].m_ConbinedPose;
			}
			return pImpl->m_IsAnimeEnd;
		}
		return false;
	}

	void BcFbxPNTBoneModelDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//���b�V�����Ȃ���Ε`�悵�Ȃ�
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!MeshRes)",
				L"BcFbxPNTBoneModelDraw::OnDraw()"
			);
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };


		//���C�g�𓾂�
		auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
		auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
		if (PtrMultiLight) {
			//�}���`���C�g������
			for (int i = 0; i < GetMaxDirectionalLights(); i++) {
				if (IsLightEnabled(i)) {
					SetLightDirection(i, PtrMultiLight->GetLight(i).m_Directional);
					SetLightDiffuseColor(i, PtrMultiLight->GetLight(i).m_DiffuseColor);
					SetLightSpecularColor(i, PtrMultiLight->GetLight(i).m_SpecularColor);
				}
			}
		}
		else {
			//�����ł͂Ȃ�
			auto LightPtr = GetGameObject()->OnGetDrawLight();
			SetLightEnabled(0, true);
			SetLightDirection(0, LightPtr.m_Directional);
			SetLightDiffuseColor(0, LightPtr.m_DiffuseColor);
			SetLightSpecularColor(0, LightPtr.m_SpecularColor);
			for (int i = 1; i < GetMaxDirectionalLights(); i++) {
				SetLightEnabled(i, false);
			}
		}
		SetAmbientLightColor(PtrLightObj->GetAmbientLightColor());

		//�V�F�[�_�̐ݒ�
		if (IsPerPixelLighting()) {
			//�s�N�Z�����C�e�B���O
			if (IsOwnShadowActive()) {
				//�e�t��
				if (IsBiasedNormals()) {
					//�o�C�A�X�t��
					pD3D11DeviceContext->VSSetShader(BcVSPNTBonePLBnShadow::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̐ݒ�
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBonePLBnShadow::GetPtr()->GetInputLayout());
				}
				else {
					//�o�C�A�X����
					pD3D11DeviceContext->VSSetShader(BcVSPNTBonePLShadow::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̐ݒ�
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBonePLShadow::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTPLShadow::GetPtr()->GetShader(), nullptr, 0);
			}
			else {
				//�e����
				if (IsBiasedNormals()) {
					//�o�C�A�X�t��
					pD3D11DeviceContext->VSSetShader(BcVSPNTBonePLBn::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̐ݒ�
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBonePLBn::GetPtr()->GetInputLayout());
				}
				else {
					//�o�C�A�X����
					pD3D11DeviceContext->VSSetShader(BcVSPNTBonePL::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̐ݒ�
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBonePL::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTPL::GetPtr()->GetShader(), nullptr, 0);
			}
		}
		else {
			//���_���C�e�B���O
			if (IsOwnShadowActive()) {
				//�e�t��
				if (IsBiasedNormals()) {
					//�o�C�A�X�t��
					pD3D11DeviceContext->VSSetShader(BcVSPNTBoneVLBnShadow::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̐ݒ�
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBoneVLBnShadow::GetPtr()->GetInputLayout());
				}
				else {
					//�o�C�A�X����
					pD3D11DeviceContext->VSSetShader(BcVSPNTBoneVLShadow::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̐ݒ�
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBoneVLShadow::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTVLShadow::GetPtr()->GetShader(), nullptr, 0);
			}
			else {
				//�e����
				if (IsBiasedNormals()) {
					//�o�C�A�X�t��
					pD3D11DeviceContext->VSSetShader(BcVSPNTBoneVLBn::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̐ݒ�
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBoneVLBn::GetPtr()->GetInputLayout());
				}
				else {
					//�o�C�A�X����
					pD3D11DeviceContext->VSSetShader(BcVSPNTBoneVL::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̐ݒ�
					pD3D11DeviceContext->IASetInputLayout(BcVSPNTBoneVL::GetPtr()->GetInputLayout());
				}
				pD3D11DeviceContext->PSSetShader(BcPSPNTVL::GetPtr()->GetShader(), nullptr, 0);
			}
		}
		//�e�ƃT���v���[
		if (IsOwnShadowActive()) {
			//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//�V���h�E�}�b�v�T���v���[
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
		}
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�f�v�X�X�e���V���X�e�[�g�͐ݒ�ɔC����
		SetDeviceDepthStencilState();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		BasicConstants BcCb;
		ZeroMemory(&BcCb, sizeof(BcCb));
		if (IsOwnShadowActive()) {
			SetConstants(BcCb, MeshRes->GetMashData());
		}
		else {
			SetConstants(BcCb, MeshRes->GetMashData());
		}

		//������}�e���A�����Ƃ̕`��
		auto& MatVec = MeshRes->GetMaterialExVec();
		for (auto& m : MatVec) {
			if (m.m_TextureResource && IsModelTextureEnabled()) {
				//�e�N�X�`��������e�N�X�`�����L���ł���
				BcCb.activeFlg.y = 1;
			}
			else {
				//�e�N�X�`�����Ȃ�
				BcCb.activeFlg.y = 0;
			}
			if (IsModelDiffusePriority()) {
				BcCb.diffuseColor = m.m_Diffuse;
			}
			if (IsModelEmissivePriority()) {
				Col4 Em4 = m.m_Emissive;
				Em4.w = 0.0f;
				BcCb.emissiveColor = Em4;
			}
			//�{�[���̐ݒ�
			size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				Mat4x4 mat = pImpl->m_LocalBonesMatrix[b];
				mat.transpose();
				BcCb.bones[cb_count] = ((XMMATRIX)mat).r[0];
				BcCb.bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				BcCb.bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}

			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &BcCb, 0, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�e�N�X�`���ƃT���v���[
			if (m.m_TextureResource) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
				//�T���v���[�͐ݒ�ɔC����
				SetDeviceSamplerState();
			}
			else {
				//�V�F�[�_�[���\�[�X���N���A
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				//�T���v���[���N���A
				pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
			}
			//���������Ȃ�
			if (GetGameObject()->GetAlphaActive()) {
				//�u�����h�X�e�[�g
				//��������
				if (GetBlendState() == BlendState::Additive) {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
				}
				else {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
				//���X�^���C�U�X�e�[�g(���`��)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//�`��
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				//���X�^���C�U�X�e�[�g�i�\�`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				//�����������Ȃ�
				//�u�����h�X�e�[�g�͐ݒ�ɔC����
				SetDeviceBlendState();
				//���X�^���C�U�X�e�[�g�͐ݒ�ɔC����
				SetDeviceRasterizerState();
				//�`��
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
		}
		//��n��
		Dev->InitializeStates();
	}


	//--------------------------------------------------------------------------------------
	//	BcFbxPNTnTBoneModelDraw::Impl
	//--------------------------------------------------------------------------------------
	struct BcFbxPNTnTBoneModelDraw::Impl {
		string m_sCurrentAnimationName;	//���ݏ����ΏۂɂȂ��Ă���A�j���[�V�����̖��O
		float m_CurrentTime;			//�A�j���[�V�����̌��݂̌o�ߎ��ԁi�b�j
		bool m_IsAnimeEnd;				//���݂̃A�j���[�V�������I���������ǂ���
		vector< Bone > m_vecLocalBones;	//�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă���
										//�V�F�[�_�ɓn���{�[���s��
		vector<Mat4x4> m_LocalBonesMatrix;	//�V���h�E�}�b�v�ɓn���|�C���^

												//�A�j���[�V�����𖼑O�ŏƉ�邷��ۂɎg�p����C���f�b�N�X�e�[�u��
		map< string, AnimationData > m_AnimationMap;
		Impl() :
			m_sCurrentAnimationName(""),
			m_CurrentTime(0),
			m_IsAnimeEnd(false)
		{}
		~Impl() {}
	};
	//--------------------------------------------------------------------------------------
	///	BcFbxPNTnTBoneModelDraw�`��R���|�[�l���g�i�{�[�����f���`��p�j
	//--------------------------------------------------------------------------------------

	BcFbxPNTnTBoneModelDraw::BcFbxPNTnTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		BcPNTnTStaticModelDraw(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g�̂RD
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}
	BcFbxPNTnTBoneModelDraw::~BcFbxPNTnTBoneModelDraw() {}

	//�{�[���s������\�[�X����ǂݍ���
	void BcFbxPNTnTBoneModelDraw::SetBoneVec() {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->GetNumBones() > 0) {
			//���[�J���{�[���̃I���W�i������̃R�s�[
			pImpl->m_vecLocalBones.resize(PtrMeshResource->GetBonesVec().size());
			pImpl->m_vecLocalBones = PtrMeshResource->GetBonesVec();

			pImpl->m_LocalBonesMatrix.resize(PtrMeshResource->GetNumBones());
		}
		else {
			throw BaseException(
				L"�{�[����������܂���",
				L"if (PtrMeshResource->GetNumBones() <= 0)",
				L"BcFbxPNTnTBoneModelDraw::SetBoneVec()"
			);
		}
	}
	void BcFbxPNTnTBoneModelDraw::GetBoneVec(vector< Bone >& Bones) {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->GetNumBones() > 0) {
			//���[�J���{�[���̃I���W�i������̃R�s�[
			Bones.resize(PtrMeshResource->GetBonesVec().size());
			Bones = PtrMeshResource->GetBonesVec();
		}
		else {
			throw BaseException(
				L"�{�[����������܂���",
				L"if (PtrMeshResource->GetNumBones() <= 0)",
				L"BcFbxPNTnTBoneModelDraw::GetBoneVec()"
			);
		}
	}


	//�A�N�Z�T
	//�e�I�u�W�F�N�g���Ƃ̃{�[��
	const vector< Bone >& BcFbxPNTnTBoneModelDraw::GetVecLocalFbxBones() const {
		return pImpl->m_vecLocalBones;
	}
	const vector< Bone >* BcFbxPNTnTBoneModelDraw::GetVecLocalFbxBonesPtr() const {
		return &pImpl->m_vecLocalBones;
	}

	const vector< Mat4x4 >* BcFbxPNTnTBoneModelDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_LocalBonesMatrix;
	}

	void BcFbxPNTnTBoneModelDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		if (!MeshRes->IsSkining()) {
			throw BaseException(
				L"���b�V�����{�[�����b�V���ł͂���܂���",
				L"if (!MeshRes->IsSkining() || MeshRes->GetBoneCount() == 0 || MeshRes->GetSampleCount() == 0)",
				L"BcFbxPNTnTBoneModelDraw::SetMeshResource()"
			);

		}
		BcPNTnTStaticModelDraw::SetMeshResource(MeshRes);
		//�{�[��������Γǂݍ���
		SetBoneVec();
	}

	void BcFbxPNTnTBoneModelDraw::SetMeshResource(const wstring& MeshKey) {
		BcFbxPNTnTBoneModelDraw::SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}



	void BcFbxPNTnTBoneModelDraw::AddAnimation(const char* Name, UINT StartFrame, UINT FrameLength, bool Loop,
		float FramesParSecond) {
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		if (PtrMeshResource->IsSkining()) {
			map< string, AnimationData >::iterator it
				= pImpl->m_AnimationMap.find(Name);
			if (it != pImpl->m_AnimationMap.end()) {
				//�w��̖��O����������
				//���̃f�[�^�ɍ����ւ�
				it->second.m_StartSample = StartFrame;
				it->second.m_SampleLength = FrameLength;
				it->second.m_SamplesParSecond = FramesParSecond;
				it->second.m_IsLoop = Loop;
			}
			else {
				//������Ȃ�
				//�A�j���[�V������`�̒ǉ�
				pImpl->m_AnimationMap[Name] = AnimationData(StartFrame, FrameLength,
					Loop, FramesParSecond);
			}
		}
	}
	const AnimationData& BcFbxPNTnTBoneModelDraw::GetAnimationData(const string& AnimeName) const {
		auto it = pImpl->m_AnimationMap.find(AnimeName);
		if (it != pImpl->m_AnimationMap.end()) {
			return it->second;
		}
		else {
			throw BaseMBException(
				"�w��̃A�j���[�V������������܂���",
				AnimeName,
				"AnimationData& BcFbxPNTBoneModelDraw::GetAnimationData()");
		}
	}


	const string& BcFbxPNTnTBoneModelDraw::GetCurrentAnimation() const {
		return pImpl->m_sCurrentAnimationName;
	}
	void BcFbxPNTnTBoneModelDraw::SetCurrentAnimation(const string& AnemationName, float StartTime) {
		//�w��̃A�j���[�V���������邩�ǂ����`�F�b�N
		//������Η�O���o��
		AnimationData animData = GetAnimationData(AnemationName);
		pImpl->m_sCurrentAnimationName = AnemationName;
		pImpl->m_CurrentTime = StartTime;
		pImpl->m_IsAnimeEnd = false;
	}

	float BcFbxPNTnTBoneModelDraw::GetCurrentTime() const {
		return pImpl->m_CurrentTime;
	}


	//���݂̃A�j���[�V�������I�����Ă��邩�ǂ���
	bool BcFbxPNTnTBoneModelDraw::IsTargetAnimeEnd() const {
		return pImpl->m_IsAnimeEnd;
	}

	//�w�肵��ID�̃{�[���̌��݂̍s����擾����
	void BcFbxPNTnTBoneModelDraw::GetBoneMatrix(UINT BoneId, Mat4x4& Matrix) const {
		auto PtrGameObject = GetGameObject();
		auto PtrT = PtrGameObject->GetComponent<Transform>();
		Mat4x4 MeshMatrix = GetMeshToTransformMatrix() * PtrT->GetWorldMatrix();
		if (pImpl->m_vecLocalBones.size() <= BoneId) {
			throw BaseException(
				L"�{�[��ID���͈͊O�ł�",
				L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
				L"BcFbxPNTnTBoneModelDraw::GetBoneMatrix()"
			);
		}
		Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
	}

	//�w�肵��ID�̃{�[���̌��݂̃��[�J���s����擾����i�e�q�֌W���\�z����ȂǗp�j
	void BcFbxPNTnTBoneModelDraw::GetLocalBoneMatrix(UINT BoneId, Mat4x4& Matrix) const {
		auto PtrGameObject = GetGameObject();
		auto PtrT = PtrGameObject->GetComponent<Transform>();
		Mat4x4 MeshMatrix = GetMeshToTransformMatrix();
		if (pImpl->m_vecLocalBones.size() <= BoneId) {
			throw BaseException(
				L"�{�[��ID���͈͊O�ł�",
				L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
				L"BcFbxPNTBoneModelDraw::GetLocalBoneMatrix()"
			);
		}
		Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
	}

	void BcFbxPNTnTBoneModelDraw::OnCreate() {
		SetLightingEnabled(true);
		//�}���`���C�g�̐ݒ�
		for (int i = 0; i < GetMaxDirectionalLights(); i++) {
			SetLightEnabled(i, true);
		}
	}


	bool BcFbxPNTnTBoneModelDraw::UpdateAnimation(float ElapsedTime) {
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = dynamic_pointer_cast<FbxMeshResource2>(GetMeshResource());
		//�{�[�����o�^����Ă�����{�[����T��
		if (PtrMeshResource->IsSkining()) {
			if (!pImpl->m_IsAnimeEnd) {
				pImpl->m_CurrentTime += ElapsedTime;
			}
			//�A�j���[�V�����f�[�^�̎擾
			//�`�F�b�N���邾���Ȃ̂Ńe���|�����ł悢
			AnimationData animData = GetAnimationData(pImpl->m_sCurrentAnimationName);
			//�{�[�������݂̎��ԂɍX�V����
			//�߂�l�͏I�����Ă邩�ǂ���
			pImpl->m_IsAnimeEnd = PtrMeshResource->GenerateCurrentPose(pImpl->m_vecLocalBones, animData, pImpl->m_CurrentTime);
			if (animData.m_IsLoop) {
				float TotalTime = (float)animData.m_SampleLength / (float)animData.m_SamplesParSecond;
				if (TotalTime <= pImpl->m_CurrentTime) {
					pImpl->m_CurrentTime = 0.0f;
				}
			}
			//�V���h�E�}�b�v�p�̍s��ɃR�s�[
			size_t BoneSz = pImpl->m_vecLocalBones.size();
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				pImpl->m_LocalBonesMatrix[b] = pImpl->m_vecLocalBones[b].m_ConbinedPose;
			}
			return pImpl->m_IsAnimeEnd;
		}
		return false;
	}

	void BcFbxPNTnTBoneModelDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		//���b�V�����Ȃ���Ε`�悵�Ȃ�
		auto MeshRes = GetMeshResource();
		if (!MeshRes) {
			throw BaseException(
				L"���b�V�����쐬����Ă��܂���",
				L"if (!MeshRes)",
				L"BcPNTnTBoneModelDraw::OnDraw()"
			);
		}
		auto shNormalTex = GetNormalMapTextureResource();
		if (!shNormalTex) {
			throw BaseException(
				L"�@���}�b�v�̃e�N�X�`��������܂���",
				L"if (!shNormalTex)",
				L"BcPNTnTBoneModelDraw::OnDraw()"
			);
		}


		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };


		//���C�g�𓾂�
		auto PtrLightObj = GetGameObject()->GetStage()->GetLight();
		auto PtrMultiLight = dynamic_pointer_cast<MultiLight>(PtrLightObj);
		if (PtrMultiLight) {
			//�}���`���C�g������
			for (int i = 0; i < GetMaxDirectionalLights(); i++) {
				if (IsLightEnabled(i)) {
					SetLightDirection(i, PtrMultiLight->GetLight(i).m_Directional);
					SetLightDiffuseColor(i, PtrMultiLight->GetLight(i).m_DiffuseColor);
					SetLightSpecularColor(i, PtrMultiLight->GetLight(i).m_SpecularColor);
				}
			}
		}
		else {
			//�����ł͂Ȃ�
			auto LightPtr = GetGameObject()->OnGetDrawLight();
			SetLightEnabled(0, true);
			SetLightDirection(0, LightPtr.m_Directional);
			SetLightDiffuseColor(0, LightPtr.m_DiffuseColor);
			SetLightSpecularColor(0, LightPtr.m_SpecularColor);
			for (int i = 1; i < GetMaxDirectionalLights(); i++) {
				SetLightEnabled(i, false);
			}
		}
		SetAmbientLightColor(PtrLightObj->GetAmbientLightColor());

		//�V�F�[�_�̐ݒ�
		//�s�N�Z�����C�e�B���O�̂�
		if (IsOwnShadowActive()) {
			//�e�t��
			//�o�C�A�X�����̂�
			pD3D11DeviceContext->VSSetShader(BcVSPNTnTBonePLShadow::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(BcVSPNTnTBonePLShadow::GetPtr()->GetInputLayout());
			pD3D11DeviceContext->PSSetShader(BcPSPNTnTPLShadow::GetPtr()->GetShader(), nullptr, 0);
		}
		else {
			//�e����
			//�o�C�A�X�����̂�
			pD3D11DeviceContext->VSSetShader(BcVSPNTnTBonePL::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(BcVSPNTnTBonePL::GetPtr()->GetInputLayout());
			pD3D11DeviceContext->PSSetShader(BcPSPNTnTPL::GetPtr()->GetShader(), nullptr, 0);
		}

		//�e�ƃT���v���[
		if (IsOwnShadowActive()) {
			//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
			auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
			ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
			pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
			//�V���h�E�}�b�v�T���v���[
			ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
			pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
		}

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = MeshRes->GetNumStride();
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�f�v�X�X�e���V���X�e�[�g�͐ݒ�ɔC����
		SetDeviceDepthStencilState();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		BasicConstants BcCb;
		ZeroMemory(&BcCb, sizeof(BcCb));
		if (IsOwnShadowActive()) {
			SetConstants(BcCb, MeshRes->GetMashData());
		}
		else {
			SetConstants(BcCb, MeshRes->GetMashData());
		}

		//������}�e���A�����Ƃ̕`��
		auto& MatVec = MeshRes->GetMaterialExVec();
		for (auto& m : MatVec) {
			if (m.m_TextureResource && IsModelTextureEnabled()) {
				//�e�N�X�`��������e�N�X�`�����L���ł���
				BcCb.activeFlg.y = 1;
			}
			else {
				//�e�N�X�`�����Ȃ�
				BcCb.activeFlg.y = 0;
			}
			if (IsModelDiffusePriority()) {
				BcCb.diffuseColor = m.m_Diffuse;
			}
			if (IsModelEmissivePriority()) {
				Col4 Em4 = m.m_Emissive;
				Em4.w = 0.0f;
				BcCb.emissiveColor = Em4;
			}
			//�{�[���̐ݒ�
			size_t BoneSz = pImpl->m_LocalBonesMatrix.size();
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				Mat4x4 mat = pImpl->m_LocalBonesMatrix[b];
				mat.transpose();
				BcCb.bones[cb_count] = ((XMMATRIX)mat).r[0];
				BcCb.bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				BcCb.bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}

			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &BcCb, 0, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);


			//�e�N�X�`���ƃT���v���[
			//�e�t�������ŃZ�b�g����ʒu���Ⴄ
			if (IsOwnShadowActive()) {
				if (m.m_TextureResource) {
					pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
					//�T���v���[�͐ݒ�ɔC����
					SetDeviceSamplerState();
				}
				else {
					//�V�F�[�_�[���\�[�X���N���A
					//�e�͕`�悷��̂őS���̓N���A���Ȃ�
					pD3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
					//�T���v���[���N���A
					pD3D11DeviceContext->PSSetSamplers(0, 1, pNullSR);
				}
				//�@���}�b�v
				//�Q�Ԗڂɓ����
				pD3D11DeviceContext->PSSetShaderResources(2, 1, shNormalTex->GetShaderResourceView().GetAddressOf());
			}
			else {
				if (m.m_TextureResource) {
					pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
					//�T���v���[�͐ݒ�ɔC����
					SetDeviceSamplerState();
				}
				else {
					//�V�F�[�_�[���\�[�X���N���A
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//�T���v���[���N���A
					pD3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
				}
				//�@���}�b�v
				//1�Ԗڂɓ����
				pD3D11DeviceContext->PSSetShaderResources(1, 1, shNormalTex->GetShaderResourceView().GetAddressOf());
			}
			//���������Ȃ�
			if (GetGameObject()->GetAlphaActive()) {
				//�u�����h�X�e�[�g
				//��������
				if (GetBlendState() == BlendState::Additive) {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
				}
				else {
					pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
				//���X�^���C�U�X�e�[�g(���`��)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//�`��
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				//���X�^���C�U�X�e�[�g�i�\�`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
			else {
				//�����������Ȃ�
				//�u�����h�X�e�[�g�͐ݒ�ɔC����
				SetDeviceBlendState();
				//���X�^���C�U�X�e�[�g�͐ݒ�ɔC����
				SetDeviceRasterizerState();
				//�`��
				pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			}
		}
		//��n��
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	//	class FbxMeshObject : public GameObject;
	//	�p�r: FBX���b�V���̃I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	FbxMeshObject::FbxMeshObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_DataDir(L""),
		m_MeshIndex(0),
		m_CharaLocalScale(1.0f),
		m_CharaLocalPosition(0, 0, 0),
		m_IsReadStaticMesh(false),
		m_WithTangent(false),
		m_IsAnimeRun(false),
		m_TextureWrap(false)
	{}
	void FbxMeshObject::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.0f, 0);

		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));



	}

	void FbxMeshObject::OnUpdate() {
		auto PtrBoneDraw = GetComponent<BcFbxPNTBoneModelDraw>(false);
		auto PtrTanBoneDraw = GetComponent<BcFbxPNTnTBoneModelDraw>(false);
		if (PtrBoneDraw && m_IsAnimeRun) {
			if (PtrBoneDraw->IsTargetAnimeEnd()) {
				m_IsAnimeRun = false;
			}
			else {
				PtrBoneDraw->UpdateAnimation(App::GetApp()->GetElapsedTime());
			}
		}
		else if (PtrTanBoneDraw && m_IsAnimeRun) {
			if (PtrTanBoneDraw->IsTargetAnimeEnd()) {
				m_IsAnimeRun = false;
			}
			else {
				PtrTanBoneDraw->UpdateAnimation(App::GetApp()->GetElapsedTime());
			}
		}
	}

	void FbxMeshObject::OnUpdate2() {
		if (m_MeshIndex == 0) {
			//���b�V���C���f�b�N�X��0�̏ꍇ�̂ݕ�����\��
			auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
			wstring FPS(L"FPS: ");
			FPS += Util::UintToWStr(fps);
			FPS += L"\n";
			wstring CurrentTime = L"";
			wstring AnimeRun = L"";
			auto PtrBoneDraw = GetComponent<BcFbxPNTBoneModelDraw>(false);
			auto PtrTanBoneDraw = GetComponent<BcFbxPNTnTBoneModelDraw>(false);
			if (PtrBoneDraw) {
				CurrentTime += Util::FloatToWStr(PtrBoneDraw->GetCurrentTime(), 6, Util::FloatModify::Fixed) + L"\n";
				if (m_IsAnimeRun) {
					AnimeRun = L"ANIME: RUN\n";
				}
				else {
					AnimeRun = L"ANIME: STOP\n";
				}
			}
			else if (PtrTanBoneDraw) {
				CurrentTime += Util::FloatToWStr(PtrTanBoneDraw->GetCurrentTime(), 6, Util::FloatModify::Fixed) + L"\n";
				if (m_IsAnimeRun) {
					AnimeRun = L"ANIME: RUN\n";
				}
				else {
					AnimeRun = L"ANIME: STOP\n";
				}
			}

			wstring str = FPS + CurrentTime + AnimeRun;
			//�����������
			auto PtrString = GetComponent<StringSprite>();
			PtrString->SetText(str);
		}
	}




	void FbxMeshObject::ResetFbxMesh(const wstring& DirName, const shared_ptr<FbxSceneResource>& SceneRes,
		size_t MeshIndex, float Scale, const Vec3& Position,
		bool IsReadStatic, bool WithTangent, const wstring& NormalFileName, bool TextureWrap) {
		try {
			m_DataDir = DirName;
			m_MeshIndex = MeshIndex;
			m_CharaLocalScale = Scale;
			m_CharaLocalPosition = Position;
			m_IsReadStaticMesh = IsReadStatic;
			m_WithTangent = WithTangent;
			m_TextureWrap = TextureWrap;
			m_PtrFbxMesh = SceneRes->GetFbxMeshResource(m_MeshIndex);
			//���f���̍s��
			auto Ptr = GetComponent<Transform>();
			Ptr->SetScale(m_CharaLocalScale, m_CharaLocalScale, m_CharaLocalScale);
			Ptr->SetRotation(0.0f, 0.0f, 0.0f);
			Ptr->SetPosition(m_CharaLocalPosition);

			auto ShadowPtr = AddComponent<Shadowmap>();
			ShadowPtr->SetMeshResource(m_PtrFbxMesh);

			RemoveComponent<BcPNTStaticModelDraw>();
			RemoveComponent<BcPNTnTStaticModelDraw>();
			RemoveComponent<BcFbxPNTBoneModelDraw>();
			RemoveComponent<BcFbxPNTnTBoneModelDraw>();

			if (m_PtrFbxMesh->IsSkining() && !m_IsReadStaticMesh) {
				if (WithTangent) {
					if (App::GetApp()->CheckResource<TextureResource>(L"NORMAL_TX")) {
						App::GetApp()->UnRegisterResource<TextureResource>(L"NORMAL_TX");
					}
					App::GetApp()->RegisterTexture(L"NORMAL_TX", NormalFileName);
					auto PtrDraw = AddComponent<BcFbxPNTnTBoneModelDraw>();
					PtrDraw->SetFogEnabled(true);
					PtrDraw->SetMeshResource(m_PtrFbxMesh);
					PtrDraw->SetNormalMapTextureResource(L"NORMAL_TX");
					PtrDraw->AddAnimation("start", 0, 1, true, 30);
					PtrDraw->SetCurrentAnimation("start");
					if (m_TextureWrap) {
						PtrDraw->SetSamplerState(SamplerState::LinearWrap);
					}
					else {
						PtrDraw->SetSamplerState(SamplerState::LinearClamp);
					}
				}
				else {
					auto PtrDraw = AddComponent<BcFbxPNTBoneModelDraw>();
					PtrDraw->SetFogEnabled(true);
					PtrDraw->SetMeshResource(m_PtrFbxMesh);
					PtrDraw->AddAnimation("start", 0, 1, true, 30);
					PtrDraw->SetCurrentAnimation("start");
					if (m_TextureWrap) {
						PtrDraw->SetSamplerState(SamplerState::LinearWrap);
					}
					else {
						PtrDraw->SetSamplerState(SamplerState::LinearClamp);
					}
				}
			}
			else {
				if (WithTangent) {
					if (App::GetApp()->CheckResource<TextureResource>(L"NORMAL_TX")) {
						App::GetApp()->UnRegisterResource<TextureResource>(L"NORMAL_TX");
					}
					App::GetApp()->RegisterTexture(L"NORMAL_TX", NormalFileName);
					auto PtrDraw = AddComponent<BcPNTnTStaticModelDraw>();
					PtrDraw->SetFogEnabled(true);
					PtrDraw->SetMeshResource(m_PtrFbxMesh);
					PtrDraw->SetNormalMapTextureResource(L"NORMAL_TX");
					if (m_TextureWrap) {
						PtrDraw->SetSamplerState(SamplerState::LinearWrap);
					}
					else {
						PtrDraw->SetSamplerState(SamplerState::LinearClamp);
					}
				}
				else {
					auto PtrDraw = AddComponent<BcPNTStaticModelDraw>();
					PtrDraw->SetFogEnabled(true);
					PtrDraw->SetMeshResource(m_PtrFbxMesh);
					if (m_TextureWrap) {
						PtrDraw->SetSamplerState(SamplerState::LinearWrap);
					}
					else {
						PtrDraw->SetSamplerState(SamplerState::LinearClamp);
					}
				}
			}
		}
		catch (...) {
			throw;
		}

	}



	bool FbxMeshObject::CheckSkinMesh() {
		auto PtrBoneDraw = GetComponent<BcFbxPNTBoneModelDraw>(false);
		if (PtrBoneDraw) {
			auto FbxMeshPtr = PtrBoneDraw->GetMeshResource();
			if (FbxMeshPtr->IsSkining()) {
				return true;
			}
		}
		auto PtrTanBoneDraw = GetComponent<BcFbxPNTnTBoneModelDraw>(false);
		if (PtrTanBoneDraw) {
			auto FbxMeshPtr = PtrTanBoneDraw->GetMeshResource();
			if (FbxMeshPtr->IsSkining()) {
				return true;
			}
		}

		return false;
	}

	bool FbxMeshObject::CheckMesh() {
		auto PtrDraw = GetComponent<BcPNTStaticModelDraw>(false);
		if (PtrDraw && PtrDraw->GetMeshResource()) {
			return true;
		}
		auto PtrTanDraw = GetComponent<BcPNTnTStaticModelDraw>(false);
		if (PtrTanDraw && PtrTanDraw->GetMeshResource()) {
			return true;
		}
		auto PtrBoneDraw = GetComponent<BcFbxPNTBoneModelDraw>(false);
		if (PtrBoneDraw) {
			auto FbxMeshPtr = PtrBoneDraw->GetMeshResource();
			if (FbxMeshPtr->IsSkining()) {
				return true;
			}
		}
		auto PtrTanBoneDraw = GetComponent<BcFbxPNTnTBoneModelDraw>(false);
		if (PtrTanBoneDraw) {
			auto FbxMeshPtr = PtrTanBoneDraw->GetMeshResource();
			if (FbxMeshPtr->IsSkining()) {
				return true;
			}
		}

		return false;
	}



	void FbxMeshObject::MoveFbxMesh(UINT FrameRate, UINT StartTime, UINT EndTime, bool IsLoop) {

		m_IsAnimeRun = false;
		auto PtrBoneDraw = GetComponent<BcFbxPNTBoneModelDraw>(false);
		auto PtrTanBoneDraw = GetComponent<BcFbxPNTnTBoneModelDraw>(false);
		if (PtrBoneDraw) {
			auto FbxMeshPtr = PtrBoneDraw->GetMeshResource();
			if (!FbxMeshPtr) {
				AfxMessageBox(L"FbxMesh���ǂݍ��܂�Ă܂���B");
				return;
			}
			UINT FrameLength = (EndTime - StartTime) * FrameRate;
			PtrBoneDraw->AddAnimation("run", StartTime, FrameLength, IsLoop, (float)FrameRate);
			PtrBoneDraw->SetCurrentAnimation("run");
			m_IsAnimeRun = true;
		}
		else if (PtrTanBoneDraw) {
			auto FbxMeshPtr = PtrTanBoneDraw->GetMeshResource();
			if (!FbxMeshPtr) {
				AfxMessageBox(L"FbxMesh���ǂݍ��܂�Ă܂���B");
				return;
			}
			UINT FrameLength = (EndTime - StartTime) * FrameRate;
			PtrTanBoneDraw->AddAnimation("run", StartTime, FrameLength, IsLoop, (float)FrameRate);
			PtrTanBoneDraw->SetCurrentAnimation("run");
			m_IsAnimeRun = true;
		}

		else {
			AfxMessageBox(L"�ǂݍ��񂾃��b�V���̓X�L�����b�V���ł͂���܂���B");
		}

	}

	void FbxMeshObject::AnimePoseStart() {

		auto PtrBoneDraw = GetComponent<BcFbxPNTBoneModelDraw>(false);
		auto PtrTanBoneDraw = GetComponent<BcFbxPNTnTBoneModelDraw>(false);
		if (PtrBoneDraw || PtrTanBoneDraw) {
			if (m_IsAnimeRun) {
				m_IsAnimeRun = false;
			}
			else {
				m_IsAnimeRun = true;
			}
		}
		else {
			AfxMessageBox(L"�ǂݍ��񂾃��b�V���̓X�L�����b�V���ł͂���܂���B");
		}
	}


	void FbxMeshObject::SaveStaticBinFile(ofstream& ofs, float Scale) {
		try {
			vector<VertexPositionNormalTexture> vertices;
			vector<VertexPositionNormalTangentTexture> vertices_withtan;

			vector<uint16_t> indices;
			vector<MaterialEx> materials;
			vector< shared_ptr<TextureResource> > textures;
			auto PtrDraw = GetComponent<BcPNTStaticModelDraw>(false);
			auto PtrTanDraw = GetComponent<BcPNTnTStaticModelDraw>(false);
			shared_ptr<FbxMeshResource2> Mesh;
			if (m_WithTangent) {
				Mesh = dynamic_pointer_cast<FbxMeshResource2>(PtrTanDraw->GetMeshResource());
				Mesh->GetStaticVerticesIndicesMaterialsWithTangent(vertices_withtan, indices, materials);
				for (auto& v : vertices_withtan) {
					v.position *= Scale;
				}
			}
			else {
				Mesh = dynamic_pointer_cast<FbxMeshResource2>(PtrDraw->GetMeshResource());
				Mesh->GetStaticVerticesIndicesMaterials(vertices, indices, materials);
				for (auto& v : vertices) {
					v.position *= Scale;
				}
			}

			//ofs.write(header.c_str(), 16);
			//���_�̕ۑ�
			BlockHeader VerTexHeader;
			if (m_WithTangent) {
				VerTexHeader.m_Type = BlockType::VertexWithTangent;
				VerTexHeader.m_Size = (UINT)vertices_withtan.size() * sizeof(VertexPositionNormalTangentTexture);
				ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
				ofs.write((const char*)&vertices_withtan.front(), VerTexHeader.m_Size);
			}
			else {
				VerTexHeader.m_Type = BlockType::Vertex;
				VerTexHeader.m_Size = (UINT)vertices.size() * sizeof(VertexPositionNormalTexture);
				ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
				ofs.write((const char*)&vertices.front(), VerTexHeader.m_Size);
			}
			//�C���f�b�N�X�̕ۑ�
			BlockHeader IndexHeader;
			IndexHeader.m_Type = BlockType::Index;
			IndexHeader.m_Size = (UINT)indices.size() * sizeof(uint16_t);
			ofs.write((const char*)&IndexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&indices.front(), IndexHeader.m_Size);
			//�}�e���A���̕ۑ�
			//�}�e���A�����̃w�b�_�̕ۑ�
			BlockHeader MaterialCountHeader;
			MaterialCountHeader.m_Type = BlockType::MaterialCount;
			MaterialCountHeader.m_Size = (UINT)materials.size();
			ofs.write((const char*)&MaterialCountHeader, sizeof(BlockHeader));
			//�}�e���A���{�̂̕ۑ�
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];
			BlockHeader MaterialHeader;
			MaterialHeader.m_Type = BlockType::Material;
			for (auto mat : materials) {
				wstring TextureFileName;
				if (mat.m_TextureResource) {
					TextureFileName = mat.m_TextureResource->GetTextureFileName();
					if (TextureFileName != L"") {
						::ZeroMemory(Drivebuff, sizeof(Drivebuff));
						::ZeroMemory(Dirbuff, sizeof(Dirbuff));
						::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
						::ZeroMemory(Extbuff, sizeof(Extbuff));
						//���W���[��������A�e�u���b�N�ɕ�����
						_wsplitpath_s(TextureFileName.c_str(),
							Drivebuff, _MAX_DRIVE,
							Dirbuff, _MAX_DIR,
							FileNamebuff, _MAX_FNAME,
							Extbuff, _MAX_EXT);
						TextureFileName = FileNamebuff;
						TextureFileName += Extbuff;
					}
				}
				SaveMaterialEx SaveMat;
				SaveMat.m_StartIndex = mat.m_StartIndex;
				SaveMat.m_IndexCount = mat.m_IndexCount;
				SaveMat.m_Diffuse = mat.m_Diffuse;
				SaveMat.m_Specular = mat.m_Specular;
				SaveMat.m_Ambient = mat.m_Ambient;
				SaveMat.m_Emissive = mat.m_Emissive;
				UINT TextureStrSize = ((UINT)TextureFileName.size() + 1) * sizeof(wchar_t);
				MaterialHeader.m_Size = sizeof(SaveMaterialEx) + TextureStrSize;
				ofs.write((const char*)&MaterialHeader, sizeof(BlockHeader));
				ofs.write((const char*)&SaveMat, sizeof(SaveMaterialEx));
				ofs.write((const char*)TextureFileName.c_str(), TextureStrSize);

			}
			//End(�w�b�_�̂�)
			BlockHeader EndHeader;
			EndHeader.m_Type = BlockType::End;
			EndHeader.m_Size = 0;
			ofs.write((const char*)&EndHeader, sizeof(BlockHeader));

		}
		catch (...) {
			throw;
		}

	}


	void FbxMeshObject::SaveSkinBinFile(ofstream& ofs, float Scale, UINT FrameParSec, UINT Start, UINT End) {
		try {
			vector<VertexPositionNormalTextureSkinning> vertices;
			vector<VertexPositionNormalTangentTextureSkinning> vertices_withtan;
			vector<uint16_t> indices;
			vector<MaterialEx> materials;
			vector<Bone> bones;
			map< string, UINT > mapBoneList;
			auto PtrDraw = GetComponent<BcFbxPNTBoneModelDraw>(false);
			auto PtrTanDraw = GetComponent<BcFbxPNTnTBoneModelDraw>(false);

			shared_ptr<FbxMeshResource2> Mesh;
			if (m_WithTangent) {
				Mesh = dynamic_pointer_cast<FbxMeshResource2>(PtrTanDraw->GetMeshResource());
				Mesh->GetSkinVerticesIndicesMaterialsWithTangent(vertices_withtan, indices, materials, bones, mapBoneList);
				for (auto& v : vertices_withtan) {
					v.position *= Scale;
				}
			}
			else {
				Mesh = dynamic_pointer_cast<FbxMeshResource2>(PtrDraw->GetMeshResource());
				Mesh->GetSkinVerticesIndicesMaterials(vertices, indices, materials, bones, mapBoneList);
				for (auto& v : vertices) {
					v.position *= Scale;
				}
			}

			vector<Mat4x4> animematrix;

			//�{�[�������݂̎��ԂɍX�V����
			vector<Bone> AnimeBones;
			AnimeBones.resize(bones.size());
			AnimeBones = bones;

			float start = (float)Start * (float)FrameParSec;
			float end = (float)End * (float)FrameParSec;

			float SampleSpan = 1.0f / (float)FrameParSec;

			//�A�j���f�[�^�𓾂�
			if (m_WithTangent) {
				PtrTanDraw->AddAnimation("save_run", (UINT)start, (UINT)(end - start), true, (float)FrameParSec);
				PtrTanDraw->SetCurrentAnimation("save_run");
				AnimationData animData = PtrTanDraw->GetAnimationData("save_run");

				for (float f = (float)Start; f < (float)End; f += SampleSpan) {
					Mesh->GenerateCurrentPose(AnimeBones, animData, f);
					for (auto b : AnimeBones) {
						Mat4x4 Mat = b.m_ConbinedPose;
						Mat._41 *= Scale;
						Mat._42 *= Scale;
						Mat._43 *= Scale;
						animematrix.push_back(Mat);
					}
				}
			}
			else {
				PtrDraw->AddAnimation("save_run", (UINT)start, (UINT)(end - start), true, (float)FrameParSec);
				PtrDraw->SetCurrentAnimation("save_run");
				AnimationData animData = PtrDraw->GetAnimationData("save_run");

				for (float f = (float)Start; f < (float)End; f += SampleSpan) {
					Mesh->GenerateCurrentPose(AnimeBones, animData, f);
					for (auto b : AnimeBones) {
						Mat4x4 Mat = b.m_ConbinedPose;
						Mat._41 *= Scale;
						Mat._42 *= Scale;
						Mat._43 *= Scale;
						animematrix.push_back(Mat);
					}
				}
			}
			//���_�̕ۑ�
			BlockHeader VerTexHeader;
			if (m_WithTangent) {
				VerTexHeader.m_Type = BlockType::SkinedVertexWithTangent;
				VerTexHeader.m_Size = (UINT)vertices_withtan.size() * sizeof(VertexPositionNormalTangentTextureSkinning);
				ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
				ofs.write((const char*)&vertices_withtan.front(), VerTexHeader.m_Size);
			}
			else {
				VerTexHeader.m_Type = BlockType::SkinedVertex;
				VerTexHeader.m_Size = (UINT)vertices.size() * sizeof(VertexPositionNormalTextureSkinning);
				ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
				ofs.write((const char*)&vertices.front(), VerTexHeader.m_Size);
			}
			//�C���f�b�N�X�̕ۑ�
			BlockHeader IndexHeader;
			IndexHeader.m_Type = BlockType::Index;
			IndexHeader.m_Size = (UINT)indices.size() * sizeof(uint16_t);
			ofs.write((const char*)&IndexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&indices.front(), IndexHeader.m_Size);
			//�}�e���A���̕ۑ�
			//�}�e���A�����̃w�b�_�̕ۑ�
			BlockHeader MaterialCountHeader;
			MaterialCountHeader.m_Type = BlockType::MaterialCount;
			MaterialCountHeader.m_Size = (UINT)materials.size();
			ofs.write((const char*)&MaterialCountHeader, sizeof(BlockHeader));
			//�}�e���A���{�̂̕ۑ�
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];
			BlockHeader MaterialHeader;
			MaterialHeader.m_Type = BlockType::Material;
			for (auto mat : materials) {
				wstring TextureFileName;
				if (mat.m_TextureResource) {
					TextureFileName = mat.m_TextureResource->GetTextureFileName();
					if (TextureFileName != L"") {
						::ZeroMemory(Drivebuff, sizeof(Drivebuff));
						::ZeroMemory(Dirbuff, sizeof(Dirbuff));
						::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
						::ZeroMemory(Extbuff, sizeof(Extbuff));
						//���W���[��������A�e�u���b�N�ɕ�����
						_wsplitpath_s(TextureFileName.c_str(),
							Drivebuff, _MAX_DRIVE,
							Dirbuff, _MAX_DIR,
							FileNamebuff, _MAX_FNAME,
							Extbuff, _MAX_EXT);
						TextureFileName = FileNamebuff;
						TextureFileName += Extbuff;
					}
				}
				SaveMaterialEx SaveMat;
				SaveMat.m_StartIndex = mat.m_StartIndex;
				SaveMat.m_IndexCount = mat.m_IndexCount;
				SaveMat.m_Diffuse = mat.m_Diffuse;
				SaveMat.m_Specular = mat.m_Specular;
				SaveMat.m_Ambient = mat.m_Ambient;
				SaveMat.m_Emissive = mat.m_Emissive;
				UINT TextureStrSize = ((UINT)TextureFileName.size() + 1) * sizeof(wchar_t);
				MaterialHeader.m_Size = sizeof(SaveMaterialEx) + TextureStrSize;
				ofs.write((const char*)&MaterialHeader, sizeof(BlockHeader));
				ofs.write((const char*)&SaveMat, sizeof(SaveMaterialEx));
				ofs.write((const char*)TextureFileName.c_str(), TextureStrSize);

			}
			//�{�[���J�E���g�i�w�b�_�̂݁j
			BlockHeader BoneCountHeader;
			BoneCountHeader.m_Type = BlockType::BoneCount;
			//�{�[���J�E���g�̏ꍇm_Size�̓{�[�����ɂȂ�
			BoneCountHeader.m_Size = (UINT)bones.size();
			ofs.write((const char*)&BoneCountHeader, sizeof(BlockHeader));

			//�A�j���[�V�����s��
			BlockHeader AnimeMatrixHeader;
			AnimeMatrixHeader.m_Type = BlockType::AnimeMatrix;
			AnimeMatrixHeader.m_Size = (UINT)animematrix.size() * sizeof(Mat4x4);
			ofs.write((const char*)&AnimeMatrixHeader, sizeof(BlockHeader));
			ofs.write((const char*)&animematrix.front(), AnimeMatrixHeader.m_Size);

			//End(�w�b�_�̂�)
			BlockHeader EndHeader;
			EndHeader.m_Type = BlockType::End;
			EndHeader.m_Size = 0;
			ofs.write((const char*)&EndHeader, sizeof(BlockHeader));
		}
		catch (...) {
			throw;
		}

	}





	//--------------------------------------------------------------------------------------
	//	class FbxMeshContainer : public GameObject;
	//	�p�r: FBX���b�V���I�u�W�F�N�g�̃R���e�i
	//--------------------------------------------------------------------------------------
	FbxMeshContainer::FbxMeshContainer(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_DataDir(L""),
		m_FbxFileName(L""),
		m_FbxResName(L""),
		m_CharaLocalScale(1.0f),
		m_CharaLocalPosition(0, 0, 0),
		m_IsReadStaticMesh(false),
		m_WithTangent(false),
		m_IsAnimeRun(false),
		m_TextureWrap(false)
	{}
	void FbxMeshContainer::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.0f, 0);

		SetAlphaActive(true);

	}

	void FbxMeshContainer::ClearFbxMesh() {
		try {
			for (auto& v : m_FbxMeshObjectVec) {
				GetStage()->RemoveGameObject<FbxMeshObject>(v);
			}
			m_FbxMeshObjectVec.clear();
			if (m_FbxResName != L"") {
				App::GetApp()->UnRegisterResource<FbxSceneResource>(m_FbxResName);
				m_FbxResName = L"";
			}
		}
		catch (...) {
			throw;
		}
	}


	void FbxMeshContainer::ResetFbxMesh(const wstring& DirName, const wstring& FbxName, size_t MeshIndex, float Scale, const Vec3& Position,
		bool IsReadStatic, bool WithTangent, const wstring& NormalFileName, bool TextureWrap) {
		try {
			ClearFbxMesh();
			m_DataDir = DirName;
			m_FbxFileName = FbxName;
			m_FbxResName = FbxName;
			m_CharaLocalScale = Scale;
			m_CharaLocalPosition = Position;
			m_IsReadStaticMesh = IsReadStatic;
			m_WithTangent = WithTangent;
			m_TextureWrap = TextureWrap;

			shared_ptr<FbxSceneResource> PtrFbxScene;
			PtrFbxScene = FbxSceneResource::CreateFbxScene(DirName, FbxName, "", m_IsReadStaticMesh, m_WithTangent);
			App::GetApp()->RegisterResource(m_FbxResName, PtrFbxScene);

			//�V�[���ɍ��킹�ăI�u�W�F�N�g�쐬
			size_t MeshSize = PtrFbxScene->GetFbxMeshResourceSize();
			for (size_t i = 0; i < MeshSize; i++) {
				auto ObjPtr = GetStage()->AddGameObject<FbxMeshObject>();
				ObjPtr->ResetFbxMesh(DirName, PtrFbxScene, i, Scale, Position,
					m_IsReadStaticMesh, WithTangent, NormalFileName, TextureWrap);
				m_FbxMeshObjectVec.push_back(ObjPtr);

			}
		}
		catch (...) {
			throw;
		}
	}

	bool FbxMeshContainer::CheckSkinMesh() {
		if (m_FbxMeshObjectVec.size() <= 0) {
			return false;
		}
		bool ret = true;
		//���ׂẴ��b�V�����X�L�����b�V���Ȃ�true
		for (auto& v : m_FbxMeshObjectVec) {
			if (!v->CheckSkinMesh()) {
				ret = false;
			}
		}
		return ret;
	}
	bool FbxMeshContainer::CheckMesh() {
		if (m_FbxMeshObjectVec.size() <= 0) {
			return false;
		}
		bool ret = true;
		//���ׂẴ��b�V�����L���Ȃ�true
		for (auto& v : m_FbxMeshObjectVec) {
			if (!v->CheckMesh()) {
				ret = false;
			}
		}
		return ret;
	}


	void FbxMeshContainer::MoveFbxMesh(UINT FrameRate, UINT StartTime, UINT EndTime, bool IsLoop) {
		for (auto& v : m_FbxMeshObjectVec) {
			v->MoveFbxMesh(FrameRate, StartTime, EndTime, IsLoop);
		}
	}

	void FbxMeshContainer::AnimePoseStart() {
		for (auto& v : m_FbxMeshObjectVec) {
			v->AnimePoseStart();
		}

	}

	void FbxMeshContainer::SaveStaticBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale) {
		if (m_FbxMeshObjectVec.size() > 0) {
			UINT version = 1;
			if (m_FbxMeshObjectVec.size() == 1) {
				version = 0;
			}
			wstring filename = Dir + FileName;
			ofstream ofs(filename, ios::out | ios::binary);
			//�o�[�W�����̕ۑ�
			string header;
			if (version == 0) {
				//�P�̃��b�V��
				header = "BDV1.0";
			}
			else {
				//�}���`���b�V��
				header = "BDV1.1";
			}
			if (header.size() < 16) {
				header.resize(16, '\0');
			}
			ofs.write(header.c_str(), 16);
			if (version == 1) {
				//�}���`���b�V���̏ꍇ���b�V������ۑ��i�u���b�N�w�b�_�̂݁j
				BlockHeader MeshCountHeader;
				MeshCountHeader.m_Type = BlockType::MashCount;
				MeshCountHeader.m_Size = (UINT)m_FbxMeshObjectVec.size();
				ofs.write((const char*)&MeshCountHeader, sizeof(BlockHeader));
			}
			for (auto&v : m_FbxMeshObjectVec) {
				v->SaveStaticBinFile(ofs, Scale);
			}
			ofs.close();
		}
	}
	void FbxMeshContainer::SaveSkinBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale,
		UINT FrameParSec, UINT Start, UINT End) {
		if (m_FbxMeshObjectVec.size() > 0) {
			UINT version = 1;
			if (m_FbxMeshObjectVec.size() == 1) {
				version = 0;
			}
			wstring filename = Dir + FileName;
			ofstream ofs(filename, ios::out | ios::binary);
			//�o�[�W�����̕ۑ�
			string header;
			if (version == 0) {
				//�P�̃��b�V��
				header = "BDV1.0";
			}
			else {
				//�}���`���b�V��
				header = "BDV1.1";
			}
			if (header.size() < 16) {
				header.resize(16, '\0');
			}
			ofs.write(header.c_str(), 16);
			if (version == 1) {
				//�}���`���b�V���̏ꍇ���b�V������ۑ��i�u���b�N�w�b�_�̂݁j
				BlockHeader MeshCountHeader;
				MeshCountHeader.m_Type = BlockType::MashCount;
				MeshCountHeader.m_Size = (UINT)m_FbxMeshObjectVec.size();
				ofs.write((const char*)&MeshCountHeader, sizeof(BlockHeader));
			}
			for (auto&v : m_FbxMeshObjectVec) {
				v->SaveSkinBinFile(ofs, Scale, FrameParSec, Start, End);
			}
			ofs.close();
		}
	}






}
//basecross
