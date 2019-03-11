/*!
@file Scene.h
@brief �V�[��
*/
#pragma once

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase {
		//FBX�}�l�[�W���[�̃f���[�^�[
		struct FbxManagerDeleter
		{
			void operator()(FbxManager *p) {
				p->Destroy();
			}
		};
		//FBX�}�l�[�W��
		unique_ptr<FbxManager, FbxManagerDeleter> m_pFbxManager;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief FBX�}�l�[�W���̎擾
		@return	FBX�}�l�[�W��
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr<FbxManager, FbxManagerDeleter>& GetFbxManager() {
			return m_pFbxManager;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;

		void ReadFbxFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale, const Vec3& Position,
			bool IsReadStatic, bool WithTangent, const wstring& NormalFileName, bool TextureWrap);

		bool CheckSkinMesh();
		bool CheckMesh();

		void ResetNewStage();

		void MoveFbxFile(UINT FrameRate, UINT StartTime, UINT EndTime, bool IsLoop);

		void SaveStaticBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale);

		void SaveSkinBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale,
			UINT FrameParSec, UINT Start, UINT End);


		void AnimePoseStart();

	};

}

//end basecross
