/*!
@file FbxMesh.h
@brief FbxMesh�֘A
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	// Bone�\����.
	//--------------------------------------------------------------------------------------
	struct	Bone
	{
		//!��{�|�[�Y�ւ̍s��
		Mat4x4	m_BindPose;
		//!���݂̍s��
		Mat4x4	m_CurrentPose;
		//!�v�Z���ꂽ���݂̍s��
		Mat4x4	m_ConbinedPose;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �R���X�g���N�^.
		*/
		//--------------------------------------------------------------------------------------
		Bone()
		{
			::ZeroMemory(this, sizeof(Bone));
		}
	};



	struct FbxMeshObjData {
		wstring m_DataDir;
		wstring m_FbxFileName;
		size_t m_MeshIndex;
		float m_Scale;
	};

	struct FbxMeshObjSaveData {
		wstring m_DataDir;
		wstring m_BinFileName;
		wstring m_Header;
		float m_Scale;
		UINT m_StartFrame;
		UINT m_FrameCount;
		float m_FrameParSecond;
		float m_SampleSpan;

		FbxMeshObjSaveData() :
			m_Scale(1.0f),
			m_StartFrame(0),
			m_FrameCount(1),
			m_FrameParSecond(30.0f),
			m_SampleSpan(1.0f / 30.0f)
		{}
	};

	struct SaveMaterialEx {
		//!�J�n�C���f�b�N�X
		UINT m_StartIndex;
		//!�`��C���f�b�N�X�J�E���g
		UINT m_IndexCount;
		//! �f�t�B�[�Y�i���̂̐F�j
		Col4 m_Diffuse;
		//! �X�y�L�����[�i���ˌ��j
		Col4 m_Specular;
		//! �A���r�G���g�i���F�j
		Col4 m_Ambient;
		//! �G�~�b�V�u�i���ˌ��j
		Col4 m_Emissive;
	};

	class FbxSceneResource;

	//--------------------------------------------------------------------------------------
	///	FBX���b�V�����\�[�X(MeshResource�h����)
	//--------------------------------------------------------------------------------------
	class FbxMeshResource2 : public MeshResource {
		friend class ObjectFactory;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �X�^�e�B�b�NFBX����̃C���X�^���X�̍\�z.
		@return	�Ȃ��i��O��throw�����j
		*/
		//--------------------------------------------------------------------------------------
		void CreateInstanceFromStaticFbx();
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �X�L�����b�V��FBX����̃C���X�^���X�̍\�z.
		@return	�Ȃ��i��O��throw�����j
		*/
		//--------------------------------------------------------------------------------------
		void CreateInstanceFromSkinFbx();
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �}�e���A�������o��
		@param[out] materials	�}�e���A���z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetMaterialVec(vector<MaterialEx>& materials);
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �v���e�N�g�R���X�g���N�^.<br/>
		FBX���b�V������f�[�^��ǂݏo��
		@param FbxSceneResourcePtr	FBX�V�[�����\�[�X
		@param pFbxMesh	FBX���b�V�����\�[�X
		@param NeedStatic	�X�L�����b�V���ł��X�^�e�B�b�N���b�V���ɂ��邩�ǂ���
		@param WithTangent	�^���W�F���g��ǂނ��ǂ���
		*/
		//--------------------------------------------------------------------------------------
		FbxMeshResource2(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic, bool WithTangent);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���_�ƃC���f�b�N�X�ƃ}�e���A�������o��
		@param[out] vertices	���_�z��
		@param[out] indices	�C���f�b�N�X�z��
		@param[out] materials	�}�e���A���z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetStaticVerticesIndicesMaterials(vector<VertexPositionNormalTexture>& vertices,
			vector<uint16_t>& indices, vector<MaterialEx>& materials);
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���_�ƃC���f�b�N�X�ƃ}�e���A�������o��(�^���W�F���g�t��)
		@param[out] vertices	���_�z��
		@param[out] indices	�C���f�b�N�X�z��
		@param[out] materials	�}�e���A���z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetStaticVerticesIndicesMaterialsWithTangent(vector<VertexPositionNormalTangentTexture>& vertices,
			vector<uint16_t>& indices, vector<MaterialEx>& materials);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���_�ƃC���f�b�N�X�ƃ}�e���A�������o��
		@param[out] vertices	���_�z��
		@param[out] indices	�C���f�b�N�X�z��
		@param[out] materials	�}�e���A���z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetSkinVerticesIndicesMaterials(vector<VertexPositionNormalTextureSkinning>& vertices,
			vector<uint16_t>& indices, vector<MaterialEx>& materials,
			vector<Bone>& Bones, map< string, UINT >& mapBoneList);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���_�ƃC���f�b�N�X�ƃ}�e���A�������o��(�^���W�F���g�t��)
		@param[out] vertices	���_�z��
		@param[out] indices	�C���f�b�N�X�z��
		@param[out] materials	�}�e���A���z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetSkinVerticesIndicesMaterialsWithTangent(vector<VertexPositionNormalTangentTextureSkinning>& vertices,
			vector<uint16_t>& indices, vector<MaterialEx>& materials,
			vector<Bone>& Bones, map< string, UINT >& mapBoneList);


		//--------------------------------------------------------------------------------------
		/*!
		@breaf �X�^�e�B�b�N�Ăяo��<br/>
		FBX���b�V������f�[�^��ǂݏo��
		@param FbxSceneResourcePtr	FBX�V�[�����\�[�X
		@param pFbxMesh	FBX���b�V�����\�[�X
		@param NeedStatic	�X�L�����b�V���ł��X�^�e�B�b�N���b�V���ɂ��邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<FbxMeshResource2> CreateFbxMeshResource(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic = false,
			bool WithTangent = false);
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/// FBX���b�V���̃f���[�^�[�֐��I�u�W�F�N�g.
		//--------------------------------------------------------------------------------------
		struct FbxMeshDeleter
		{
			void operator()(FbxMesh *p) {
				//�폜�̓}�l�[�W�����s���̂ŉ������Ȃ�
			}
		};
		//--------------------------------------------------------------------------------------
		/// FBX�X�L���̃f���[�^�[�֐��I�u�W�F�N�g.
		//--------------------------------------------------------------------------------------
		struct FbxSkinDeleter
		{
			void operator()(FbxSkin *p) {
				//�폜�̓}�l�[�W�����s���̂ŉ������Ȃ�
			}
		};
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �f�X�g���N�^.
		*/
		//--------------------------------------------------------------------------------------
		virtual ~FbxMeshResource2();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�L�j���O���邩�ǂ���.<br />
		���z�֐��Ȃ̂ŁA�h���N���X�œƎ��ɑ��d��`����
		@return	�X�L�j���O����ꍇ��true
		*/
		//--------------------------------------------------------------------------------------
		virtual bool IsSkining() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf FBX���b�V���̎擾
		@return	FBX���b�V���̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		const unique_ptr<FbxMesh, FbxMeshDeleter>& GetFbxMesh() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf FBX�X�L���̎擾
		@return	FBX�X�L���̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		const unique_ptr<FbxSkin, FbxSkinDeleter>& GetFbxSkin() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �{�[���̔z��𓾂�
		@return	�{�[���̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector< Bone >& GetBonesVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �{�[�����̎擾
		@return	�{�[����
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumBones() const {
			return (UINT)GetBonesVec().size();
		}
		//����
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �w��̃A�j���[�V�����̐ݒ肳��Ă��鎞�Ԃɂ�����A�J�����g�|�[�Y�ƍ����ϊ��s����Čv�Z����
		@param�@tgtBones	�ύX���ׂ��{�[���z��
		@param�@rAnimationData	�w��̃A�j���[�V�����f�[�^
		@param�@CurrentTime	���Ԏw��
		@return	���[�v�Ȃ��ōŌ�ɓ��B������true
		*/
		//--------------------------------------------------------------------------------------
		bool GenerateCurrentPose(vector<Bone>& tgtBones, AnimationData& rAnimationData, float CurrentTime);

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	FBX�V�[�����\�[�X
	//--------------------------------------------------------------------------------------
	class FbxSceneResource : public BaseResource {
		friend class ObjectFactory;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf FBX���b�V���z����ċA�I�ɓǂݍ���
		@param�@pFbxNode	�^�[�Q�b�g�ƂȂ�m�[�h
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CreateFbxMeshVector(FbxNode* pFbxNode);
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �v���e�N�g�R���X�g���N�^.<br/>
		FBX�t�@�C������FBX�V�[�����\�z����
		@param DataDir	�f�[�^�f�B���N�g��
		@param FileName	FBX�t�@�C����
		@param SceneName	�V�[����
		@param NeedStatic	static�\�z�����邩�ǂ���
		@param WithTangent	�^���W�F���g��ǂݍ��ނ��ǂ���
		*/
		//--------------------------------------------------------------------------------------
		FbxSceneResource(const wstring& DataDir,
			const wstring& FileName, const string& SceneName, bool NeedStatic, bool WithTangent);
		//������
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ������
		@param�@�Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
	public:
		//--------------------------------------------------------------------------------------
		/// FBX�V�[���̃f���[�^�[�֐��I�u�W�F�N�g.
		//--------------------------------------------------------------------------------------
		struct FbxSceneDeleter
		{
			//--------------------------------------------------------------------------------------
			/*!
			@breaf FBX�V�[����delete.<br />
			�폜�̓}�l�[�W�����s���̂ŉ������Ȃ�
			@param FbxScene *p	FBX�V�[���̃|�C���^
			@return	�Ȃ�
			*/
			//--------------------------------------------------------------------------------------
			void operator()(FbxScene *p) {
				//�폜�̓}�l�[�W�����s���̂ŉ������Ȃ�
			}
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~FbxSceneResource();
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �\�z�pstatic�֐�.
		@param DataDir	�f�[�^�f�B���N�g��
		@param FileName		�t�@�C����
		@param SceneName	�V�[����
		@param NeedStati	static���b�V���Ƃ��č\�z���邩�ǂ���
		@param WithTangent	�^���W�F���g��ǂݍ��ނ��ǂ���
		@return�@FBX�V�[�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<FbxSceneResource> CreateFbxScene(const wstring& DataDir,
			const wstring& FileName, const string& SceneName = "", bool NeedStatic = false, bool WithTangent = false);
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �f�[�^�f�B���N�g���𓾂�
		@param�@�Ȃ�
		@return	�f�[�^�f�B���N�g��
		*/
		//--------------------------------------------------------------------------------------
		wstring GetDataDir() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �t�@�C�����𓾂�
		@param�@�Ȃ�
		@return	�t�@�C����
		*/
		//--------------------------------------------------------------------------------------
		wstring GetFileName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf FBX�V�[�����𓾂�
		@param�@�Ȃ�
		@return	FBX�V�[����
		*/
		//--------------------------------------------------------------------------------------
		string GetFbxSceneName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���b�V�����\�[�X�𓾂�
		@param�@Index	FBX���̃��b�V��ID
		@return	FBX���b�V�����\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<FbxMeshResource2> GetFbxMeshResource(size_t Index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���b�V�����\�[�X���𓾂�
		@param�@�Ȃ�
		@return	FBX�V�[���Ɋ܂܂�郁�b�V�����\�[�X��
		*/
		//--------------------------------------------------------------------------------------
		size_t GetFbxMeshResourceSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���b�V�����\�[�X�z��𓾂�
		@param�@�Ȃ�
		@return	FBX�V�[���Ɋ܂܂�郁�b�V�����\�[�X�z��
		*/
		//--------------------------------------------------------------------------------------
		vector< shared_ptr<FbxMeshResource2> >& GetFbxMeshResourceVec() const;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	BcFbxPNTBoneModelDraw�`��R���|�[�l���g�i�{�[�����f���`��p�j
	//--------------------------------------------------------------------------------------
	class BcFbxPNTBoneModelDraw : public BcPNTStaticModelDraw {
	protected:
		//�{�[���s������\�[�X����ǂݍ���
		void SetBoneVec();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit BcFbxPNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcFbxPNTBoneModelDraw();
		//�A�N�Z�T
		//�{�[���s������\�[�X����ǂݍ���
		void GetBoneVec(vector< Bone >& Bones);
		//�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă����|�C���^
		const vector< Bone >& GetVecLocalFbxBones() const;
		const vector< Bone >* GetVecLocalFbxBonesPtr() const;

		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes)override;
		void SetMeshResource(const wstring& MeshKey);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf �A�j���[�V������ݒ肷��i���łɂ��̒�`������΁A�����ւ���j
		@param�@Name	�A�j���[�V������
		@param�@StartFrame,	�X�^�[�g�ʒu
		@param�@FrameLength	����
		@param�@Loop,			���[�v���邩�ǂ���
		@param�@FramesParSecond �P�b������̃t���[����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddAnimation(const char* Name, UINT StartFrame, UINT FrameLength, bool Loop,
			float FramesParSecond = 30.0f);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf �A�j���[�V�����f�[�^�𓾂�B������Η�O
		@return	�A�j���[�V�����f�[�^
		*/
		//--------------------------------------------------------------------------------------
		const AnimationData& GetAnimationData(const string& AnimeName) const;

		//�J�����g�A�j���[�V����
		const string& GetCurrentAnimation() const;
		void SetCurrentAnimation(const string& AnemationName, float StartTime = 0.0f);
		//���݂̃A�j���[�V�������I�����Ă��邩�ǂ���
		bool IsTargetAnimeEnd() const;

		//�w�肵��ID�̃{�[���̌��݂̍s����擾����
		void GetBoneMatrix(UINT BoneId, Mat4x4& Matrix) const;

		//�w�肵��ID�̃{�[���̌��݂̃��[�J���s����擾����i�e�q�֌W���\�z����ȂǗp�j
		void GetLocalBoneMatrix(UINT BoneId, Mat4x4& Matrix) const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[�J���{�[���s��z��𓾂�
		@return	���[�J���{�[���s��z��̐擪�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< Mat4x4 >* GetVecLocalBonesPtr() const;

		float GetCurrentTime() const;


		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V������i�߂�
		@param[in]	ElapsedTime	�o�ߎ���
		@return	�A�j���[�V�������I�������true
		*/
		//--------------------------------------------------------------------------------------
		bool UpdateAnimation(float ElapsedTime);
		//����
		virtual void OnCreate()override;
		virtual void OnDraw()override;

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	BcFbxPNTnTBoneModelDraw�`��R���|�[�l���g�i�{�[�����f���`��p�j�^���W�F���g�t��
	//--------------------------------------------------------------------------------------
	class BcFbxPNTnTBoneModelDraw : public BcPNTnTStaticModelDraw {
	protected:
		//�{�[���s������\�[�X����ǂݍ���
		void SetBoneVec();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit BcFbxPNTnTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcFbxPNTnTBoneModelDraw();
		//�A�N�Z�T
		//�{�[���s������\�[�X����ǂݍ���
		void GetBoneVec(vector< Bone >& Bones);
		//�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă����|�C���^
		const vector< Bone >& GetVecLocalFbxBones() const;
		const vector< Bone >* GetVecLocalFbxBonesPtr() const;

		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes)override;
		void SetMeshResource(const wstring& MeshKey);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf �A�j���[�V������ݒ肷��i���łɂ��̒�`������΁A�����ւ���j
		@param�@Name	�A�j���[�V������
		@param�@StartFrame,	�X�^�[�g�ʒu
		@param�@FrameLength	����
		@param�@Loop,			���[�v���邩�ǂ���
		@param�@FramesParSecond �P�b������̃t���[����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddAnimation(const char* Name, UINT StartFrame, UINT FrameLength, bool Loop,
			float FramesParSecond = 30.0f);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf �A�j���[�V�����f�[�^�𓾂�B������Η�O
		@return	�A�j���[�V�����f�[�^
		*/
		//--------------------------------------------------------------------------------------
		const AnimationData& GetAnimationData(const string& AnimeName) const;

		//�J�����g�A�j���[�V����
		const string& GetCurrentAnimation() const;
		void SetCurrentAnimation(const string& AnemationName, float StartTime = 0.0f);
		//���݂̃A�j���[�V�������I�����Ă��邩�ǂ���
		bool IsTargetAnimeEnd() const;

		//�w�肵��ID�̃{�[���̌��݂̍s����擾����
		void GetBoneMatrix(UINT BoneId, Mat4x4& Matrix) const;

		//�w�肵��ID�̃{�[���̌��݂̃��[�J���s����擾����i�e�q�֌W���\�z����ȂǗp�j
		void GetLocalBoneMatrix(UINT BoneId, Mat4x4& Matrix) const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[�J���{�[���s��z��𓾂�
		@return	���[�J���{�[���s��z��̐擪�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< Mat4x4 >* GetVecLocalBonesPtr() const;

		float GetCurrentTime() const;


		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V������i�߂�
		@param[in]	ElapsedTime	�o�ߎ���
		@return	�A�j���[�V�������I�������true
		*/
		//--------------------------------------------------------------------------------------
		bool UpdateAnimation(float ElapsedTime);
		//����
		virtual void OnCreate()override;
		virtual void OnDraw()override;

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class FbxMeshObject : public GameObject;
	//	�p�r: FBX���b�V���̃I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class FbxMeshObject : public GameObject {
		wstring m_DataDir;
		size_t m_MeshIndex;

		shared_ptr<FbxMeshResource2> m_PtrFbxMesh;

		float m_CharaLocalScale;
		Vec3 m_CharaLocalPosition;
		bool m_IsReadStaticMesh;
		bool m_WithTangent;
		//�e�N�X�`�������b�s���O�������邩�ǂ���
		BOOL m_TextureWrap;

		//�A�j���[�V�������s�����ǂ���
		bool m_IsAnimeRun;

	public:
		FbxMeshObject(const shared_ptr<Stage>& StagePtr);


		virtual ~FbxMeshObject() {}

		void ResetFbxMesh(const wstring& DirName, const shared_ptr<FbxSceneResource>& SceneRes, size_t MeshIndex, float Scale, const Vec3& Position,
			bool IsReadStatic, bool WithTangent, const wstring& NormalFileName, bool TextureWrap);

		bool CheckSkinMesh();
		bool CheckMesh();


		void MoveFbxMesh(UINT FrameRate, UINT StartTime, UINT EndTime, bool IsLoop);

		void AnimePoseStart();


		void SaveStaticBinFile(ofstream& ofs, float Scale);
		void SaveSkinBinFile(ofstream& ofs, float Scale, UINT FrameParSec, UINT Start, UINT End);


		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;


	};

	//--------------------------------------------------------------------------------------
	//	class FbxMeshContainer : public GameObject;
	//	�p�r: FBX���b�V���I�u�W�F�N�g�̃R���e�i
	//--------------------------------------------------------------------------------------
	class FbxMeshContainer : public GameObject {
		wstring m_DataDir;
		wstring m_FbxFileName;
		wstring m_FbxResName;

		float m_CharaLocalScale;
		Vec3 m_CharaLocalPosition;
		bool m_IsReadStaticMesh;
		bool m_WithTangent;
		//�e�N�X�`�������b�s���O�������邩�ǂ���
		BOOL m_TextureWrap;

		//�A�j���[�V�������s�����ǂ���
		bool m_IsAnimeRun;

		vector<shared_ptr<FbxMeshObject>> m_FbxMeshObjectVec;

	public:
		FbxMeshContainer(const shared_ptr<Stage>& StagePtr);
		virtual ~FbxMeshContainer() {}
		void ResetFbxMesh(const wstring& DirName, const wstring& FbxName, size_t MeshIndex, float Scale, const Vec3& Position,
			bool IsReadStatic, bool WithTangent, const wstring& NormalFileName, bool TextureWrap);

		void ClearFbxMesh();


		bool CheckSkinMesh();
		bool CheckMesh();


		void MoveFbxMesh(UINT FrameRate, UINT StartTime, UINT EndTime, bool IsLoop);

		void AnimePoseStart();

		void SaveStaticBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale);
		void SaveSkinBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale,
			UINT FrameParSec, UINT Start, UINT End);


		//������
		virtual void OnCreate() override;
		virtual void OnUpdate() override {}
		virtual void OnUpdate2() override {}


	};



}
//end basecross
