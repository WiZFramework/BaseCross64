/*!
@file DrawComponents.h
@brief Draw�R���|�[�l���g
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/*!
	@brief	�r���[�|�[�g�ƃV�T�[��`�ݒ�
	@param[in]	vp	�r���[�|�[�g
	@return	�Ȃ�
	*/
	//--------------------------------------------------------------------------------------
	inline void RsSetViewport(const Viewport& vp) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		D3D11_VIEWPORT vp11;
		vp11.Height = vp.Height;
		vp11.Width = vp.Width;
		vp11.MinDepth = vp.MinDepth;
		vp11.MaxDepth = vp.MaxDepth;
		vp11.TopLeftX = vp.TopLeftX;
		vp11.TopLeftY = vp.TopLeftY;
		pID3D11DeviceContext->RSSetViewports(1, &vp11);
		CD3D11_RECT rect(
			(LONG)vp11.TopLeftX,
			(LONG)vp11.TopLeftY,
			(LONG)vp11.TopLeftX + (LONG)vp11.Width,
			(LONG)vp11.TopLeftY + (LONG)vp11.Height
		);
		pID3D11DeviceContext->RSSetScissorRects(1, &rect);
	}

	//--------------------------------------------------------------------------------------
	///	Shadow�R���X�^���g�o�b�t�@�\����
	//--------------------------------------------------------------------------------------
	struct ShadowConstants
	{
		/// ���[���h�s��
		bsm::Mat4x4 mWorld;
		/// �r���[�s��
		bsm::Mat4x4 mView;
		/// �ˉe�s��
		bsm::Mat4x4 mProj;
		/// Bone�p
		bsm::Vec4 Bones[3 * 72];
		ShadowConstants() {
			memset(this, 0, sizeof(ShadowConstants));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBShadow, ShadowConstants)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmap, VertexPositionNormalTexture)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmapBone, VertexPositionNormalTextureSkinning)
	DECLARE_DX11_VERTEX_SHADER(VSShadowmapBoneWithTan, VertexPositionNormalTangentTextureSkinning)


	//--------------------------------------------------------------------------------------
	///	�X�v���C�g�p�R���X�^���g�o�b�t�@�\����
	//--------------------------------------------------------------------------------------
	struct SpriteConstants
	{
		/// ���[���h�s��
		bsm::Mat4x4 World;
		/// �G�~�b�V�u�F
		bsm::Col4 Emissive;
		/// �f�t���[�Y�F
		bsm::Col4 Diffuse;
		SpriteConstants() {
			memset(this, 0, sizeof(SpriteConstants));
			Diffuse = bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	/// �X�v���C�g�R���X�^���g�o�b�t�@
	DECLARE_DX11_CONSTANT_BUFFER(CBSprite, SpriteConstants)
	//PCSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCSprite, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCSprite)
	//PTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPTSprite, VertexPositionTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPTSprite)
	//PCTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCTSprite, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTSprite)



	//--------------------------------------------------------------------------------------
	///	SimpleConstants�R���X�^���g�o�b�t�@�\����
	//--------------------------------------------------------------------------------------
	struct SimpleConstants
	{
		/// ���[���h�s��
		bsm::Mat4x4 World;
		/// �r���[�s��
		bsm::Mat4x4 View;
		/// �ˉe�s��
		bsm::Mat4x4 Projection;
		/// �G�~�b�V�u�F
		bsm::Col4 Emissive;
		/// �f�t���[�Y�F
		bsm::Col4 Diffuse;
		/// �X�y�L�����[
		bsm::Col4 Specular;
		/// �e�N�X�`��=x���A�N�e�B�u���ǂ���
		XMUINT4 ActiveFlg;
		/// ���C�C�g����
		bsm::Vec4 LightDir;
		/// ���C�g�ʒu
		bsm::Vec4 LightPos;
		/// Eye�̈ʒu
		bsm::Vec4 EyePos;
		/// ���C�g�r���[�s��
		bsm::Mat4x4 LightView;
		/// ���C�g�ˉe�s��
		bsm::Mat4x4 LightProjection;
		/// Bone�z��
		bsm::Vec4 Bones[3 * 72];
		SimpleConstants() {
			memset(this, 0, sizeof(SimpleConstants));
			Diffuse = bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f);
		};
	};
	//CBSimple
	DECLARE_DX11_CONSTANT_BUFFER(CBSimple, SimpleConstants)
	//PCStatic
	DECLARE_DX11_VERTEX_SHADER(VSPCStatic, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCStatic)
	//PNStatic
	DECLARE_DX11_VERTEX_SHADER(VSPNStatic, VertexPositionNormal)
	DECLARE_DX11_PIXEL_SHADER(PSPNStatic)
	//PTStatic
	DECLARE_DX11_VERTEX_SHADER(VSPTStatic, VertexPositionTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPTStatic)
	///PCTStatic
	DECLARE_DX11_VERTEX_SHADER(VSPCTStatic, VertexPositionColorTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPCTStatic)
	///PNTStatic
	DECLARE_DX11_VERTEX_SHADER(VSPNTStatic, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStatic)
	///PNTStaticShadow
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticShadow, VertexPositionNormalTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow)
	DECLARE_DX11_PIXEL_SHADER(PSPNTStaticShadow2)
	///PNTBone
	DECLARE_DX11_VERTEX_SHADER(VSPNTBone, VertexPositionNormalTextureSkinning)
	DECLARE_DX11_VERTEX_SHADER(VSPNTBoneShadow, VertexPositionNormalTextureSkinning)
	//PCStaticInstance
	DECLARE_DX11_VERTEX_SHADER(VSPCStaticInstance, VertexPositionColorMatrix)
	//PTStaticInstance
	DECLARE_DX11_VERTEX_SHADER(VSPTStaticInstance, VertexPositionTextureMatrix)
	//PCTStaticInstance
	DECLARE_DX11_VERTEX_SHADER(VSPCTStaticInstance, VertexPositionColorTextureMatrix)
	///PNTStaticInstance
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticInstance, VertexPositionNormalTextureMatrix)
	DECLARE_DX11_VERTEX_SHADER(VSPNTStaticInstanceShadow, VertexPositionNormalTextureMatrix)


	class GameObject;

	//--------------------------------------------------------------------------------------
	///	�`��R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	class DrawComponent : public Component {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		DrawComponent(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�v���e�N�g�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DrawComponent();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�u�����h�X�e�[�g�̎擾
		@return	�u�����h�X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		BlendState GetBlendState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�v�X�X�e���V���X�e�[�g�̎擾
		@return	�f�v�X�X�e���V���X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		DepthStencilState GetDepthStencilState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���X�^���C�U�X�e�[�g�̎擾
		@return	���X�^���C�U�X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		RasterizerState GetRasterizerState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�T���v���[�X�e�[�g�̎擾
		@return	�T���v���[�X�e�[�g
		*/
		//--------------------------------------------------------------------------------------
		SamplerState GetSamplerState() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�u�����h�X�e�[�g�̐ݒ�
		@param[in]	state	�u�����h�X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBlendState(const BlendState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�u�����h�X�e�[�g�̃f�o�C�X�ւ̐ݒ�B
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDeviceBlendState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�v�X�X�e���V���X�e�[�g�̐ݒ�
		@param[in]	state	�f�v�X�X�e���V���X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDepthStencilState(const DepthStencilState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�v�X�X�e���V���X�e�[�g�̃f�o�C�X�ւ̐ݒ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDeviceDepthStencilState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���X�^���C�U�X�e�[�g�̐ݒ�
		@param[in]	state	���X�^���C�U�X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetRasterizerState(const RasterizerState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���X�^���C�U�X�e�[�g�̃f�o�C�X�ւ̐ݒ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDeviceRasterizerState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�T���v���[�X�e�[�g�̐ݒ�
		@param[in]	state	�T���v���[�X�e�[�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetSamplerState(const SamplerState state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�T���v���[�X�e�[�g�̃f�o�C�X�ւ̐ݒ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDeviceSamplerState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V���ƃ��[���h�s��̊Ԃ�⊮����s��̎擾<br />
		���b�V���̃T�C�Y������A���S�̈ʒu���Q�[���I�u�W�F�N�g�̃��[���h�s��ƍ��ق�����ꍇ�A<br />
		���̊Ԃ�⊮����s���ݒ�ł���B
		@return	���b�V���ƃ��[���h�s��̊Ԃ�⊮����s��
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Mat4x4& GetMeshToTransformMatrix() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V���ƃ��[���h�s��̊Ԃ�⊮����s��̎擾<br />
		���b�V���̃T�C�Y������A���S�̈ʒu���Q�[���I�u�W�F�N�g�̃��[���h�s��ƍ��ق�����ꍇ�A<br />
		���̊Ԃ�⊮����s���ݒ�ł���B
		@param[in]	Mat	�ݒ肷��s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshToTransformMatrix(const bsm::Mat4x4& Mat);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă����|�C���^<br />
		�V���h�E�}�b�v�Ȃǂ���Q�Ƃł���悤�ɉ��z�֐��ɂ���<br />
		�h���N���X�Ń{�[������������ꍇ�͑��d��`����
		@return	�{�[���s��̔z��̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< bsm::Mat4x4 >* GetVecLocalBonesPtr() const { return nullptr; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă����|�C���^�i�}���`���b�V���Łj<br />
		�V���h�E�}�b�v�Ȃǂ���Q�Ƃł���悤�ɉ��z�֐��ɂ���<br />
		�h���N���X�Ń{�[������������ꍇ�͑��d��`����
		@return	�{�[���s��̔z��̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< bsm::Mat4x4 >* GetVecMultiLocalBonesPtr(size_t index) const { return nullptr; }
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	�V���h�E�}�b�v�R���|�[�l���g�i�O�����p�j
	//--------------------------------------------------------------------------------------
	class Shadowmap : public DrawComponent {
		void DrawSigle(const MeshPrimData& data);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit Shadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Shadowmap();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�g�̍����𓾂�
		@return	���C�g�̍���
		*/
		//--------------------------------------------------------------------------------------
		static float GetLightHeight();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�g��Near�l�𓾂�
		@return	���C�g��Near�l
		*/
		//--------------------------------------------------------------------------------------
		static float GetLightNear();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�g��Far�l�𓾂�
		@return	���C�g��Far�l
		*/
		//--------------------------------------------------------------------------------------
		static float GetLightFar();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�r���[�̕��𓾂�
		@return	�r���[�̕�
		*/
		//--------------------------------------------------------------------------------------
		static float GetViewWidth();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�r���[�̍����𓾂�
		@return	�r���[�̍���
		*/
		//--------------------------------------------------------------------------------------
		static float GetViewHeight();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�̈ʒu�����l�iY�����j�𓾂�
		@return	�e�̈ʒu�����l�iY�����j
		*/
		//--------------------------------------------------------------------------------------
		static float GetPosAdjustment();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�g�̍�����ݒ肷��
		@param[in]	f	����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void SetLightHeight(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�g��Near�l��ݒ肷��
		@param[in]	f	Near�l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void SetLightNear(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�g��Far�l��ݒ肷��
		@param[in]	f	Far�l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void SetLightFar(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�r���[�̕���ݒ肷��
		@param[in]	f	�r���[�̕�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void SetViewWidth(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�r���[�̍�����ݒ肷��
		@param[in]	f	�r���[�̍���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void SetViewHeight(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�r���[�̕��y�э����𓯂��l�ɐݒ肷��
		@param[in]	f	�r���[�̕��y�э����i���l�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void SetViewSize(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�̈ʒu�����l�iY�����j��ݒ肷��
		@param[in]	f	�e�̈ʒu�����l�iY�����j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void SetPosAdjustment(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V�����\�[�X�𓾂�
		@param[in]	ExceptionActive	�G���[���ɗ�O���������邩�ǂ���
		@return	���b�V�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MeshResource> GetMeshResource(bool ExceptionActive = true) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V�����\�[�X��ݒ肷��
		@param[in]	ResKey	���b�V�����\�[�X�̃L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V�����\�[�X��ݒ肷��
		@param[in]	MeshResourcePtr	���b�V�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`���b�V�����\�[�X�𓾂�
		@param[in]	ExceptionActive	�G���[���ɗ�O���������邩�ǂ���
		@return	���b�V�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MultiMeshResource> GetMultiMeshResource(bool ExceptionActive = true) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`���b�V�����\�[�X��ݒ肷��
		@param[in]	ResKey	���b�V�����\�[�X�̃L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMultiMeshResource(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`���b�V�����\�[�X��ݒ肷��
		@param[in]	MeshResourcePtr	���b�V�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr);
		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�V�����B�i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`�揈���B
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	PCTParticle�`��R���|�[�l���g(�p�[�e�B�N���`��)
	//--------------------------------------------------------------------------------------
	class PCTParticleDraw : public DrawComponent {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		@param[in]	MaxInstance	�p�[�e�B�N�������
		@param[in]	AddType	���Z�������邩�ǂ���
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTParticleDraw(const shared_ptr<GameObject>& GameObjectPtr, size_t MaxInstance,bool AddType = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCTParticleDraw();
		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`��p�p�[�e�B�N����o�^����
		@param[in]	ToCaneraLength	�J�����܂ł̋���
		@param[in]	WorldMatrix	���[���h�s��
		@param[in]	TextureRes	�e�N�X�`��
		@param[in]	TextureRes	�e�N�X�`��
		@param[in]	Diffuse   Diffuse�J���[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddParticle(float ToCaneraLength, const bsm::Mat4x4& WorldMatrix, 
			const shared_ptr<TextureResource>& TextureRes,const bsm::Col4& Diffuse = bsm::Col4(1,1,1,1));
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	Sprite�`��I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class SpriteBaseDraw : public DrawComponent {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		explicit SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SpriteBaseDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�^���g�o�b�t�@�̐ݒ�
		@param[out]	SmCb	�ݒ肷��R���X�^���g�o�b�t�@�\����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetConstants(SpriteConstants& SpCb);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���_�ύX�ł���X�v���C�g���b�V�����쐬����i���łɂ���ꍇ�͍����ւ���j
		@param[in]	Vertices	���_�̔z��
		@param[in]	indices		�C���f�b�N�X�̔z��
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void CreateMesh(vector<T>& Vertices, vector<uint16_t>& indices) {
			try {
				//���b�V���̍쐬�i�ύX�ł���j
				auto SpriteMesh = MeshResource::CreateMeshResource(Vertices, indices, true);
				SetMeshResource(SpriteMesh);
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���_�o�b�t�@�̍X�V
		@param[in]	Vertices	���_�z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void UpdateVertices(const vector<T>& Vertices) {
			auto SpriteMesh = GetMeshResource();
			if (!SpriteMesh) {
				throw BaseException(
					L"���b�V�����쐬����Ă��܂���",
					L"if (!GetMeshResource())",
					L"SpriteBaseDraw::UpdateVertices()"
				);

			}
			if (Vertices.size() > SpriteMesh->GetNumVertices()) {
				throw BaseException(
					L"�X�V���钸�_���傫�����܂�",
					L"if (Vertices.size() > SpriteMesh->GetNumVertices())",
					L"SpriteBaseDraw::UpdateVertices()"
				);
			}
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

			//���_�̕ύX
			//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
			D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
			D3D11_MAPPED_SUBRESOURCE mappedBuffer;
			//���_�̃}�b�v
			if (FAILED(pD3D11DeviceContext->Map(SpriteMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
				// Map���s
				throw BaseException(
					L"���_��Map�Ɏ��s���܂����B",
					L"if(FAILED(pID3D11DeviceContext->Map()))",
					L"SpriteBaseDraw::UpdateVertices()"
				);
			}
			//���_�̕ύX
			T* vertices
				= (T*)mappedBuffer.pData;
			for (size_t i = 0; i < SpriteMesh->GetNumVertices(); i++) {
				vertices[i] = Vertices[i];
			}
			//�A���}�b�v
			pD3D11DeviceContext->Unmap(SpriteMesh->GetVertexBuffer().Get(), 0);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�v���C�g���b�V���̕`��
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_
		@param[in]	data	���b�V���̃f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawSprite(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//NULL�̃V�F�[�_���\�[�X�̏���
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			//�T���v���[�̏���
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			//�e�I�u�W�F�N�g���ʏ���
			//�V�F�[�_�̐ݒ�
			//���_�V�F�[�_
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//�s�N�Z���V�F�[�_
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			//�ʏ���
			SpriteConstants sb;
			//�R���X�^���g�o�b�t�@�̍쐬
			SetConstants(sb);
			//�e�N�X�`��
			auto shTex = GetTextureResource();
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = data.m_NumStride;
			UINT offset = 0;
			//�`����@�̃Z�b�g
			pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);
			//���_�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			//�e�����_�����O�X�e�[�g�̐ݒ�
			//�u�����h�X�e�[�g
			RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
			//�f�v�X�X�e���V���X�e�[�g
			RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
			//�e�N�X�`���ƃT���v���[
			if (shTex) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
				//�T���v���[��ݒ�
				RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
			}
			else {
				//�V�F�[�_�[���\�[�X���N���A
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				//�T���v���[���N���A
				RenderState->SetSamplerAllClear(pD3D11DeviceContext);
			}
			//���X�^���C�U�X�e�[�g�ƕ`��
			RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
			//�`��
			pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V�����\�[�X�̎擾
		@return	���b�V�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MeshResource> GetMeshResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V�����\�[�X�̐ݒ�
		@param[in]	MeshRes	���b�V�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�N�X�`�����\�[�X�̐ݒ�
		@param[in]	TextureRes	�e�N�X�`�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�N�X�`�����\�[�X�̐ݒ�
		@param[in]	TextureKey	�o�^����Ă���e�N�X�`���L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const wstring& TextureKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�N�X�`�����\�[�X�̎擾
		@return	�e�N�X�`�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetTextureResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�G�~�b�V�u�F�̎擾
		@return	�G�~�b�V�u�F
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetEmissive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�G�~�b�V�u�F�̐ݒ�
		@param[in]	col	�G�~�b�V�u�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetEmissive(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�t���[�Y�F�̎擾
		@return	�f�t���[�Y�F
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetDiffuse() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�t���[�Y�F�̐ݒ�
		@param[in]	col	�f�t���[�Y�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDiffuse(const bsm::Col4& col);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	PCSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PCSpriteDraw : public SpriteBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		@param[in]	Vertices	�������_�̔z��
		@param[in]	indices		�����C���f�b�N�X�̔z��
		*/
		//--------------------------------------------------------------------------------------
		explicit PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
			vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCSpriteDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	PTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PTSpriteDraw : public SpriteBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		@param[in]	Vertices	�������_�̔z��
		@param[in]	indices		�����C���f�b�N�X�̔z��
		*/
		//--------------------------------------------------------------------------------------
		explicit PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
			vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PTSpriteDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	PCTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PCTSpriteDraw : public SpriteBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		@param[in]	Vertices	�������_�̔z��
		@param[in]	indices		�����C���f�b�N�X�̔z��
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
			vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCTSpriteDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	�A�j���[�V�����f�[�^�\����.
	//--------------------------------------------------------------------------------------
	struct	AnimationData
	{
		///	�X�^�[�g�T���v��
		UINT	m_StartSample;
		///	�T���v���̒���
		UINT	m_SampleLength;
		///	���[�v���邩�ǂ���
		bool	m_IsLoop;
		///	�A�j�����I���������ǂ���
		bool	m_IsAnimeEnd;
		///	1�b������̃t���[��
		float	m_SamplesParSecond;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^.
		*/
		//--------------------------------------------------------------------------------------
		AnimationData()
		{
			ZeroMemory(this, sizeof(AnimationData));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^.
		@param[in]	StartSample	�X�^�[�g�t���[��
		@param[in]	SampleLength	�t���[���̒���
		@param[in]	bLoop	���[�v���邩�ǂ���
		@param[in]	SamplesParSecond = 30.0f	1�b������̃t���[����
		*/
		//--------------------------------------------------------------------------------------
		AnimationData(UINT StartSample, UINT SampleLength, bool bLoop,
			float SamplesParSecond = 30.0f) :
			m_StartSample{ StartSample },
			m_SampleLength{ SampleLength },
			m_IsLoop{ bLoop },
			m_IsAnimeEnd{ false },
			m_SamplesParSecond{ SamplesParSecond }
		{}
	};

	//--------------------------------------------------------------------------------------
	///	�ėp�`��p
	//--------------------------------------------------------------------------------------
	class GenericDraw :public ObjectInterface {
		void PCWireFrameDraw(const shared_ptr<GameObject>& GameObjectPtr, const shared_ptr<MeshResource>& MeshRes,
			const bsm::Col4& Emissive, const bsm::Col4& Diffuse, const bsm::Mat4x4& MeshToTransformMatrix);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		GenericDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GenericDraw();
		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�A�t���[���`�揈��
		@param[in]	GameObj	�Q�[���I�u�W�F�N�g
		@param[in]	Mesh	PC���b�V��

		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void DrawWireFrame(const shared_ptr<GameObject>& GameObj,
			const shared_ptr<MeshResource>& Mesh,const bsm::Mat4x4& MeshToTransformMatrix = bsm::Mat4x4());

	};

	//--------------------------------------------------------------------------------------
	///	�`��Ɏg�p����\����(�e)
	//--------------------------------------------------------------------------------------
	struct DrawObjectBase {
		//���b�V�����\�[�X
		weak_ptr<MeshResource> m_MeshResource;
		//�O�p�`�Փ˔��蓙�Ɏg���e���|�����z��istatic�j
		static vector<bsm::Vec3> m_TempPositions;
		//�}���`���b�V�����\�[�X
		weak_ptr<MultiMeshResource> m_MultiMeshResource;
		//�ȉ��A�{�[�����f���p
		//���[�J���{�[���s��̔z��
		vector<bsm::Mat4x4> m_LocalBonesMatrix;
		//���[�J���{�[���s��̔z��(�}���`���b�V���p)
		vector<vector<bsm::Mat4x4>> m_MultiLocalBonesMatrix;
		//�A�j���[�V������`�̃}�b�v
		map<wstring, AnimationData> m_AnimationMap;
		//���݂̃A�j���[�V������
		wstring m_CurrentAnimeName;
		//�J�����g�A�j���[�V����
		float m_CurrentAnimeTime;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		DrawObjectBase() :
			m_CurrentAnimeName(L""),
			m_CurrentAnimeTime(0)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �{�[���̏�����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void BoneInit() {
			auto MeshRes = m_MeshResource.lock();
			if (MeshRes && MeshRes->IsSkining() && MeshRes->GetBoneCount() > 0 && MeshRes->GetSampleCount() > 0) {
				//�擪�̃{�[�����̍s��ŏ�����
				m_LocalBonesMatrix.resize(MeshRes->GetBoneCount());
				auto& SampleMatrixVec = MeshRes->GetSampleMatrixVec();
				for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
					m_LocalBonesMatrix[i] = SampleMatrixVec[i];
				}
			}
			//�}���`���b�V���p
			auto MultiMeshRes = m_MultiMeshResource.lock();
			if (MultiMeshRes && MultiMeshRes->IsSkining(0) && MultiMeshRes->GetBoneCount(0) > 0 && MultiMeshRes->GetSampleCount(0) > 0) {
				m_MultiLocalBonesMatrix.resize(MultiMeshRes->GetMeshVecCount());
				for (size_t i = 0; i < MultiMeshRes->GetMeshVecCount(); i++) {
					m_MultiLocalBonesMatrix[i].resize(MultiMeshRes->GetBoneCount(i));
					auto& SampleMatrixVec = MultiMeshRes->GetSampleMatrixVec(i);
					for (UINT j = 0; j < m_MultiLocalBonesMatrix[i].size(); j++) {
						m_MultiLocalBonesMatrix[i][j] = SampleMatrixVec[j];
					}
				}
			}

		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V������ύX����
		@param[in]	AnemationName	�A�j���[�V�������i���łɒǉ�����Ă�����́j
		@param[in]	StartTime = 0.0f	�J�n����̕b��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ChangeCurrentAnimation(const wstring& AnemationName, float StartTime = 0.0f) {
			auto MeshRes = m_MeshResource.lock();
			bool MeshResFlg = MeshRes && MeshRes->IsSkining() && MeshRes->GetBoneCount() > 0 && MeshRes->GetSampleCount() > 0;
			//�}���`���b�V���p
			auto MultiMeshRes = m_MultiMeshResource.lock();
			bool MultiMeshResFlg = MultiMeshRes && MultiMeshRes->IsSkining(0) && MultiMeshRes->GetBoneCount(0) > 0 && MultiMeshRes->GetSampleCount(0) > 0;
			if (MeshResFlg || MultiMeshResFlg) {
				if (AnemationName == L"") {
					throw BaseException(
						L"�A�j���[�V���������󔒂ł�",
						L"if (AnemationName == L\"\")",
						L"DrawObjectBase::SetCurrentAnimation()"
					);
				}
				auto it = m_AnimationMap.find(AnemationName);
				if (it != m_AnimationMap.end()) {
					//�w��̖��O����������
					m_CurrentAnimeName = AnemationName;
					m_CurrentAnimeTime = StartTime;
					//�A�j���[�V�����͏I�����Ă��Ȃ�
					it->second.m_IsAnimeEnd = false;
				}
				else {
					//������Ȃ�
					throw BaseException(
						L"�w��̃A�j���[�V�����͓o�^����Ă܂���",
						AnemationName,
						L"DrawObjectBase::SetCurrentAnimation()"
					);
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A�j���[�V������ǉ�����
		@param[in]	Name	�A�j���[�V������
		@param[in]	StartSample	�J�n�T���v��
		@param[in]	SampleLength	�T���v���̒���
		@param[in]	Loop	���[�v���邩�ǂ���
		@param[in]	SamplesParSecond = 30.0f	1�b������̃T���v����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
			float SamplesParSecond = 30.0f) {
			auto MeshRes = m_MeshResource.lock();
			bool MeshResFlg = MeshRes && MeshRes->IsSkining() && MeshRes->GetBoneCount() > 0 && MeshRes->GetSampleCount() > 0;
			//�}���`���b�V���p
			auto MultiMeshRes = m_MultiMeshResource.lock();
			bool MultiMeshResFlg = MultiMeshRes && MultiMeshRes->IsSkining(0) && MultiMeshRes->GetBoneCount(0) > 0 && MultiMeshRes->GetSampleCount(0) > 0;
			if (MeshResFlg || MultiMeshResFlg) {
				if (Name == L"") {
					throw BaseException(
						L"�A�j���[�V���������󔒂ł�",
						L"if (Name == L\"\")",
						L"DrawObjectBase::AddAnimation()"
					);
				}
				if (StartSample < 0 || SampleLength < 0) {
					throw BaseException(
						L"�J�n�T���v�����T���v������0�����ł�",
						L"if (StartSample < 0 || SampleLength < 0)",
						L"DrawObjectBase::AddAnimation()"
					);
				}
				if (SamplesParSecond <= 0.0f) {
					throw BaseException(
						L"�T���v�����b��0�ȉ��ł�",
						L"if (SamplesParSecond <= 0.0f)",
						L"DrawObjectBase::AddAnimation()"
					);
				}
				//�d���L�[������΍����ւ���
				AnimationData Data((UINT)StartSample, (UINT)SampleLength, Loop, SamplesParSecond);
				m_AnimationMap[Name] = Data;
				if (m_AnimationMap.size() == 1) {
					//1�����o�^���Ȃ�������A�J�����g�A�j���͊Y���A�j���ƂȂ�
					ChangeCurrentAnimation(Name, 0);
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�n�s�񂩂�I���s��̊Ԃ�t���Ԏ��̍s���Ԃ�
		@param[in]	m1	�J�n�̍s��
		@param[in]	m2	�I���̍s��
		@param[in]	t	���ԁi0����1.0f�j
		@param[out]	out	���ʂ��󂯎��s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void InterpolationMatrix(const bsm::Mat4x4& m1, const bsm::Mat4x4& m2, float t, bsm::Mat4x4& out) {
			bsm::Vec3 Scale1, Pos1;
			bsm::Quat Qt1;
			m1.decompose(Scale1, Qt1, Pos1);
			Qt1.normalize();

			bsm::Vec3 Scale2, Pos2;
			bsm::Quat Qt2;

			m2.decompose(Scale2, Qt2, Pos2);
			Qt2.normalize();

			bsm::Vec3 ScaleOut, PosOut;
			bsm::Quat QtOut;

			ScaleOut = Lerp::CalculateLerp(Scale1, Scale2, 0.0f, 1.0f, t, Lerp::Linear);
			PosOut = Lerp::CalculateLerp(Pos1, Pos2, 0.0f, 1.0f, t, Lerp::Linear);
			QtOut = XMQuaternionSlerp(Qt1, Qt2, t);
			out.affineTransformation(ScaleOut, bsm::Vec3(0, 0, 0), QtOut, PosOut);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V�����f�[�^�𓾂�
		@return	���݂̃A�j���[�V�����f�[�^
		*/
		//--------------------------------------------------------------------------------------
		AnimationData& GetAnimationData() {
			auto MeshRes = m_MeshResource.lock();
			bool MeshResFlg = MeshRes && MeshRes->IsSkining() && MeshRes->GetBoneCount() > 0 && MeshRes->GetSampleCount() > 0;
			//�}���`���b�V���p
			auto MultiMeshRes = m_MultiMeshResource.lock();
			bool MultiMeshResFlg = MultiMeshRes && MultiMeshRes->IsSkining(0) && MultiMeshRes->GetBoneCount(0) > 0 && MultiMeshRes->GetSampleCount(0) > 0;
			if (MeshResFlg || MultiMeshResFlg) {
				if (m_CurrentAnimeName == L"") {
					//������Ȃ�
					throw BaseException(
						L"�J�����g�A�j���[�V�������ݒ肳��Ă܂���",
						L"if (m_CurrentAnimeName == L\"\")",
						L"DrawObjectBase::GetAnimationData()"
					);
				}
				return m_AnimationMap[m_CurrentAnimeName];
			}
			//�{�[���f�[�^�ł͂Ȃ�
			throw BaseException(
				L"�{�[���t���̃��b�V���ł͂���܂���",
				L"!MeshRes->IsSkining()",
				L"DrawObjectBase::GetAnimationData()"
			);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V������i�߂�
		@param[in]	ElapsedTime	�o�ߎ���
		@return	�A�j���[�V�������I�������true
		*/
		//--------------------------------------------------------------------------------------
		bool UpdateAnimation(float ElapsedTime) {
			auto MeshRes = m_MeshResource.lock();
			bool MeshResFlg = MeshRes && MeshRes->IsSkining() && MeshRes->GetBoneCount() > 0 && MeshRes->GetSampleCount() > 0;
			//�}���`���b�V���p
			auto MultiMeshRes = m_MultiMeshResource.lock();
			bool MultiMeshResFlg = MultiMeshRes && MultiMeshRes->IsSkining(0) && MultiMeshRes->GetBoneCount(0) > 0 && MultiMeshRes->GetSampleCount(0) > 0;
			if (MeshResFlg || MultiMeshResFlg) {
				if (ElapsedTime < 0.0f) {
					throw BaseException(
						L"�A�j���[�V�����X�V�Ƀ}�C�i�X�͐ݒ�ł��܂���",
						L"if (ElapsedTime < 0.0f)",
						L"DrawObjectBase::UpdateAnimation()"
					);
				}
				if (m_CurrentAnimeName == L"") {
					//������Ȃ�
					throw BaseException(
						L"�J�����g�A�j���[�V�������ݒ肳��Ă܂���",
						L"if (m_CurrentAnimeName == L\"\")",
						L"DrawObjectBase::UpdateAnimation()"
					);
				}
				if (MeshResFlg) {
					auto PtrMesh = MeshRes;
					UINT SampleCount = PtrMesh->GetSampleCount();
					auto& SampleMatrixVec = PtrMesh->GetSampleMatrixVec();
					UINT BoneCount = PtrMesh->GetBoneCount();
					auto& TgtAnimeData = m_AnimationMap[m_CurrentAnimeName];
					if (TgtAnimeData.m_StartSample >= SampleCount) {
						//�X�^�[�g�̃T���v�����Ō�̃T���v���ȍ~������
						TgtAnimeData.m_StartSample = SampleCount - 1;
						TgtAnimeData.m_SampleLength = 0;
						UINT UITgtSample = TgtAnimeData.m_StartSample;
						//�Ō�̃T���v����\��
						for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
							m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
						}
						m_CurrentAnimeTime = 0;
						if (TgtAnimeData.m_IsLoop) {
							TgtAnimeData.m_IsAnimeEnd = false;
							return false;
						}
						else {
							TgtAnimeData.m_IsAnimeEnd = true;
							return true;
						}
					}
					//���łɃA�j�����I�����Ă���
					if (TgtAnimeData.m_IsAnimeEnd) {
						//���݂̃��[�J���s����g�p
						return true;
					}
					//�J�����g�^�C�����X�V
					m_CurrentAnimeTime += ElapsedTime;
					//�X�^�[�g�ʒu���v�Z
					auto FLOATTgtSample = (float)TgtAnimeData.m_StartSample + m_CurrentAnimeTime * TgtAnimeData.m_SamplesParSecond;
					UINT UITgtSample = (UINT)FLOATTgtSample;
					UINT UILastSample = TgtAnimeData.m_StartSample + TgtAnimeData.m_SampleLength;
					if (UILastSample >= SampleCount) {
						UILastSample = SampleCount - 1;
					}
					if (UITgtSample >= UILastSample) {
						UITgtSample = UILastSample - 1;
						//�Ō�̃T���v����\��
						for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
							m_LocalBonesMatrix[i] = SampleMatrixVec[BoneCount * UITgtSample + i];
						}
						if (TgtAnimeData.m_IsLoop) {
							TgtAnimeData.m_IsAnimeEnd = false;
							//���[�v����̂ŃJ�����g�^�C����0�ɂ���
							m_CurrentAnimeTime = 0;
							return false;
						}
						else {
							m_CurrentAnimeTime = TgtAnimeData.m_SampleLength / TgtAnimeData.m_SamplesParSecond;
							TgtAnimeData.m_IsAnimeEnd = true;
							return true;
						}
					}
					else {
						//�T���v���ƃT���v���̊Ԃ̊������v�Z
						FLOATTgtSample -= (float)UITgtSample;
						UINT UINextSample = UITgtSample + 1;
						for (UINT i = 0; i < m_LocalBonesMatrix.size(); i++) {
							InterpolationMatrix(
								SampleMatrixVec[BoneCount * UITgtSample + i],
								SampleMatrixVec[BoneCount * UINextSample + i],
								FLOATTgtSample, m_LocalBonesMatrix[i]);
						}
						//�A�j���͏I����ĂȂ�
						return false;
					}
					return true;
				}
				else if (MultiMeshResFlg) {
					//�T���v�����͍ŏ��̃��b�V���̂��g�p
					UINT SampleCount = MultiMeshRes->GetSampleCount(0);
					auto& TgtAnimeData = m_AnimationMap[m_CurrentAnimeName];
					if (TgtAnimeData.m_StartSample >= SampleCount) {
						//�X�^�[�g�̃T���v�����Ō�̃T���v���ȍ~������
						TgtAnimeData.m_StartSample = SampleCount - 1;
						TgtAnimeData.m_SampleLength = 0;
						UINT UITgtSample = TgtAnimeData.m_StartSample;
						//�Ō�̃T���v����\��
						for (size_t mc = 0; mc < MultiMeshRes->GetMeshVecCount(); mc++) {
							auto& SampleMatrixVec = MultiMeshRes->GetSampleMatrixVec(mc);
							UINT BoneCount = MultiMeshRes->GetBoneCount(mc);
							for (UINT i = 0; i < m_MultiLocalBonesMatrix[mc].size(); i++) {
								m_MultiLocalBonesMatrix[mc][i] = SampleMatrixVec[BoneCount * UITgtSample + i];
							}
						}
						m_CurrentAnimeTime = 0;
						if (TgtAnimeData.m_IsLoop) {
							TgtAnimeData.m_IsAnimeEnd = false;
							return false;
						}
						else {
							TgtAnimeData.m_IsAnimeEnd = true;
							return true;
						}
					}
					//���łɃA�j�����I�����Ă���
					if (TgtAnimeData.m_IsAnimeEnd) {
						//���݂̃��[�J���s����g�p
						return true;
					}
					//�J�����g�^�C�����X�V
					m_CurrentAnimeTime += ElapsedTime;
					//�X�^�[�g�ʒu���v�Z
					auto FLOATTgtSample = (float)TgtAnimeData.m_StartSample + m_CurrentAnimeTime * TgtAnimeData.m_SamplesParSecond;
					UINT UITgtSample = (UINT)FLOATTgtSample;
					UINT UILastSample = TgtAnimeData.m_StartSample + TgtAnimeData.m_SampleLength;
					if (UILastSample >= SampleCount) {
						UILastSample = SampleCount - 1;
					}
					if (UITgtSample >= UILastSample) {
						UITgtSample = UILastSample - 1;
						//�Ō�̃T���v����\��
						for (size_t mc = 0; mc < MultiMeshRes->GetMeshVecCount(); mc++) {
							auto& SampleMatrixVec = MultiMeshRes->GetSampleMatrixVec(mc);
							UINT BoneCount = MultiMeshRes->GetBoneCount(mc);
							for (UINT i = 0; i < m_MultiLocalBonesMatrix[mc].size(); i++) {
								m_MultiLocalBonesMatrix[mc][i] = SampleMatrixVec[BoneCount * UITgtSample + i];
							}
						}
						if (TgtAnimeData.m_IsLoop) {
							TgtAnimeData.m_IsAnimeEnd = false;
							//���[�v����̂ŃJ�����g�^�C����0�ɂ���
							m_CurrentAnimeTime = 0;
							return false;
						}
						else {
							m_CurrentAnimeTime = TgtAnimeData.m_SampleLength / TgtAnimeData.m_SamplesParSecond;
							TgtAnimeData.m_IsAnimeEnd = true;
							return true;
						}
					}
					else {
						//�T���v���ƃT���v���̊Ԃ̊������v�Z
						FLOATTgtSample -= (float)UITgtSample;
						UINT UINextSample = UITgtSample + 1;
						for (size_t mc = 0; mc < MultiMeshRes->GetMeshVecCount(); mc++) {
							auto& SampleMatrixVec = MultiMeshRes->GetSampleMatrixVec(mc);
							UINT BoneCount = MultiMeshRes->GetBoneCount(mc);

							for (UINT i = 0; i < m_MultiLocalBonesMatrix[mc].size(); i++) {
								InterpolationMatrix(
									SampleMatrixVec[BoneCount * UITgtSample + i],
									SampleMatrixVec[BoneCount * UINextSample + i],
									FLOATTgtSample, m_MultiLocalBonesMatrix[mc][i]);
							}
						}
						//�A�j���͏I����ĂȂ�
						return false;
					}
					return true;
				}
			}
			return true;
		}
	};


	//--------------------------------------------------------------------------------------
	///	������V�V�X�e��
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	///	Simple�`��I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class SmBaseDraw : public DrawComponent {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		explicit SmBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SmBaseDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�^���g�o�b�t�@�̐ݒ�
		@param[out]	SmCb	�ݒ肷��R���X�^���g�o�b�t�@�\����
		@param[in]	data	���b�V���̃f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetConstants(SimpleConstants& SmCb, const MeshPrimData& data);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s��o�b�t�@�̍쐬(�C���X�^���X�`��p)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void CreateMatrixBuffer();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s��o�b�t�@�̃}�b�v(�C���X�^���X�`��p)
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void MapMatrixBuffer();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�^�e�B�b�N���b�V���̕`��
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_
		@param[in]	data	���b�V���̃f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawStatic(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//NULL�̃V�F�[�_���\�[�X�̏���
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			//�T���v���[�̏���
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			//�e�I�u�W�F�N�g���ʏ���
			//�V�F�[�_�̐ݒ�
			//���_�V�F�[�_
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//�s�N�Z���V�F�[�_
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			//�ʏ���
			SimpleConstants SmCb;
			//�R���X�^���g�o�b�t�@�̍쐬
			SetConstants(SmCb, data);
			//�e�N�X�`��
			auto shTex = GetTextureResource();
			if (shTex) {
				//�e�N�X�`��������
				SmCb.ActiveFlg.x = 1;
			}
			else {
				//�`��R���|�[�l���g�ɂ̓e�N�X�`�����Ȃ�
				if (shTex = data.m_TextureResource.lock()) {
					//�e�N�X�`��������
					SmCb.ActiveFlg.x = 1;
				}
				else {
					SmCb.ActiveFlg.x = 0;
				}
			}
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = data.m_NumStride;
			UINT offset = 0;
			//�`����@�̃Z�b�g
			pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);
			//���_�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			//�e�����_�����O�X�e�[�g�̐ݒ�
			//�u�����h�X�e�[�g
			RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
			//�f�v�X�X�e���V���X�e�[�g
			RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
			//�e�N�X�`���ƃT���v���[
			if (shTex) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
				//�T���v���[��ݒ�
				RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
			}
			else {
				//�V�F�[�_�[���\�[�X���N���A
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				//�T���v���[���N���A
				RenderState->SetSamplerAllClear(pD3D11DeviceContext);
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
			//���X�^���C�U�X�e�[�g�ƕ`��
			if (GetRasterizerState() == RasterizerState::DoubleDraw) {
				//���������p
				//���X�^���C�U�X�e�[�g(���`��)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//�`��
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
				//���X�^���C�U�X�e�[�g�i�\�`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			}
			else {
				RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
				//�`��
				pD3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
			}
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�^�e�B�b�N���b�V���̃C���X�^���X�`��
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_
		@param[in]	data	���b�V���̃f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawStaticInstance(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//NULL�̃V�F�[�_���\�[�X�̏���
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			//�T���v���[�̏���
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			//�e�I�u�W�F�N�g���ʏ���
			//�V�F�[�_�̐ݒ�
			//���_�V�F�[�_
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//�s�N�Z���V�F�[�_
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			//�ʏ���
			SimpleConstants SmCb;
			//�R���X�^���g�o�b�t�@�̍쐬
			SetConstants(SmCb, data);
			//�e�N�X�`��
			auto shTex = GetTextureResource();
			if (shTex) {
				//�e�N�X�`��������
				SmCb.ActiveFlg.x = 1;
			}
			else {
				//�e�N�X�`�����Ȃ�
				SmCb.ActiveFlg.x = 0;
			}
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�X�g���C�h�ƃI�t�Z�b�g
			//�`��̒��_�o�b�t�@�ƍs��o�b�t�@��ݒ�
			UINT stride[2] = { data.m_NumStride, sizeof(bsm::Mat4x4) };
			UINT offset[2] = { 0, 0 };
			ID3D11Buffer* pBuf[2] = { data.m_VertexBuffer.Get(), GetMatrixBuffer().Get() };
			//�`����@�̃Z�b�g
			pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);
			//���_�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			//�e�����_�����O�X�e�[�g�̐ݒ�
			//�u�����h�X�e�[�g
			RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
			//�f�v�X�X�e���V���X�e�[�g
			RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
			//�e�N�X�`���ƃT���v���[
			if (shTex) {
				pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
				//�T���v���[��ݒ�
				RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
			}
			else {
				//�V�F�[�_�[���\�[�X���N���A
				pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
				//�T���v���[���N���A
				RenderState->SetSamplerAllClear(pD3D11DeviceContext);
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
			//���X�^���C�U�X�e�[�g�ƕ`��
			if (GetRasterizerState() == RasterizerState::DoubleDraw) {
				//���������p
				//���X�^���C�U�X�e�[�g(���`��)
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//�`��
				pD3D11DeviceContext->DrawIndexedInstanced(data.m_NumIndicis, (UINT)GetMatrixVec().size(), 0, 0, 0);
				//���X�^���C�U�X�e�[�g�i�\�`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexedInstanced(data.m_NumIndicis, (UINT)GetMatrixVec().size(), 0, 0, 0);
			}
			else {
				RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
				//�`��
				pD3D11DeviceContext->DrawIndexedInstanced(data.m_NumIndicis, (UINT)GetMatrixVec().size(), 0, 0, 0);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���f�����b�V���̕`��
		@tparam[in]	T_VShader	�g�p���钸�_�V�F�[�_
		@tparam[in]	T_PShader	�g�p����s�N�Z���V�F�[�_
		@param[in]	data	���b�V���̃f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T_VShader, typename T_PShader>
		void DrawModel(const MeshPrimData& data) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto RenderState = Dev->GetRenderState();
			//NULL�̃V�F�[�_���\�[�X�̏���
			ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
			//�T���v���[�̏���
			ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
			ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			//�e�I�u�W�F�N�g���ʏ���
			//�V�F�[�_�̐ݒ�
			//���_�V�F�[�_
			pD3D11DeviceContext->VSSetShader(T_VShader::GetPtr()->GetShader(), nullptr, 0);
			//�C���v�b�g���C�A�E�g�̐ݒ�
			pD3D11DeviceContext->IASetInputLayout(T_VShader::GetPtr()->GetInputLayout());
			//�s�N�Z���V�F�[�_
			pD3D11DeviceContext->PSSetShader(T_PShader::GetPtr()->GetShader(), nullptr, 0);
			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = data.m_NumStride;
			UINT offset = 0;
			//�`����@�̃Z�b�g
			pD3D11DeviceContext->IASetPrimitiveTopology(data.m_PrimitiveTopology);
			//���_�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
			//�e�����_�����O�X�e�[�g�̐ݒ�
			//�u�����h�X�e�[�g
			RenderState->SetBlendState(pD3D11DeviceContext, GetBlendState());
			//�f�v�X�X�e���V���X�e�[�g
			RenderState->SetDepthStencilState(pD3D11DeviceContext, GetDepthStencilState());
			//�e�ƃT���v���[
			if (GetOwnShadowActive()) {
				//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
				auto ShadowmapPtr = Dev->GetShadowMapRenderTarget();
				ID3D11ShaderResourceView* pShadowSRV = ShadowmapPtr->GetShaderResourceView();
				pD3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
				//�V���h�E�}�b�v�T���v���[
				ID3D11SamplerState* pShadowSampler = RenderState->GetComparisonLinear();
				pD3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSampler);
			}
			//�ʏ���
			SimpleConstants SmCb;
			//�R���X�^���g�o�b�t�@�̍쐬
			SetConstants(SmCb, data);
			//���b�V�����̃}�e���A���̔z��
			auto& MatVec = data.m_MaterialExVec;
			size_t MatIndex = 0;
			for (auto& m : MatVec) {
				if (m.m_TextureResource) {
					//�e�N�X�`��������
					SmCb.ActiveFlg.x = 1;
				}
				else {
					//�e�N�X�`�����Ȃ�
					SmCb.ActiveFlg.x = 0;
				}
				if (IsModelDiffusePriority()) {
					SmCb.Diffuse = m.m_Diffuse;
				}
				if (IsModelEmissivePriority()) {
					bsm::Col4 Em4 = m.m_Emissive;
					Em4.w = 0.0f;
					SmCb.Emissive = Em4;
				}
				//�R���X�^���g�o�b�t�@�̍X�V
				pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &SmCb, 0, 0);
				//�R���X�^���g�o�b�t�@�̐ݒ�
				ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//���_�V�F�[�_�ɓn��
				pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//�s�N�Z���V�F�[�_�ɓn��
				pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				if (SmCb.ActiveFlg.x) {
					//�V�F�[�_�Ƀe�N�X�`���̐ݒ肪����Ă���
					//�T���v���[�̐ݒ�
					RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 0);
					//�e�N�X�`���̑I��
					if (IsModelTextureEnabled()) {
						//���f���̃e�N�X�`�����L��
						pD3D11DeviceContext->PSSetShaderResources(0, 1, m.m_TextureResource->GetShaderResourceView().GetAddressOf());
					}
					else {
						//���f���̃e�N�X�`��������
						auto shTex = GetTextureResource();
						if (shTex) {
							//�R���|�[�l���g�Ƀe�N�X�`��������
							pD3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
						}
						else {
							//�R���|�[�l���g�Ƀe�N�X�`�����Ȃ�
							SmCb.ActiveFlg.x = 0;
						}
					}
				}
				else {
					//�V�F�[�_�[���\�[�X���N���A
					pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
					//�T���v���[���N���A
					RenderState->SetSamplerAllClear(pD3D11DeviceContext);
				}
				//���X�^���C�U�X�e�[�g�ƕ`��
				if (GetRasterizerState() == RasterizerState::DoubleDraw) {
					//���������p
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
					RenderState->SetRasterizerState(pD3D11DeviceContext, GetRasterizerState());
					//�`��
					pD3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
				}
				MatIndex++;
			}
		}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I���W�i�����b�V�����g�����ǂ���
		@return	�g���ꍇ��true
		*/
		//--------------------------------------------------------------------------------------
		bool IsOriginalMeshUse() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I���W�i�����b�V�����g�����ǂ�����ݒ�
		@param[in]	b	�I���W�i�����b�V�����g�����ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetOriginalMeshUse(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I���W�i���ȃ��b�V�����\�[�X�̎擾
		@return	�I���W�i���ȃ��b�V�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MeshResource> GetOriginalMeshResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I���W�i���ȃ��b�V�����\�[�X�̐ݒ�
		@param[in]	MeshRes	���b�V�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I���W�i�����b�V�����쐬����
		@param[in]	Vertices	���_�̔z��
		@param[in]	indices		�C���f�b�N�X�̔z��
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		void CreateOriginalMesh(vector<T>& Vertices, vector<uint16_t>& indices) {
			try {
				auto MeshRes = MeshResource::CreateMeshResource(Vertices, indices, true);
				SetOriginalMeshResource(MeshRes);
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I���W�i�����b�V���̒��_�o�b�t�@��ύX����
		@param[in]	Vertices	���_�z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		void UpdateVertices(const vector<T>& Vertices) {
			auto MeshRes = GetOriginalMeshResource();
			if (!MeshRes) {
				throw BaseException(
					L"�I���W�i�����b�V�����쐬����Ă��܂���",
					L"if (!GetOriginalMeshResource())",
					L"StaticBaseDraw::UpdateVertices()"
				);

			}
			MeshRes->UpdateVirtexBuffer(Vertices);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V�����\�[�X�̎擾(�I���W�i�����ǂ����͓����ŏ��������)
		@return	���b�V�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MeshResource> GetMeshResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I���W�i���ł͂Ȃ����b�V�����\�[�X�̐ݒ�(���z�֐�)
		@param[in]	MeshRes	���b�V�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�I���W�i���ł͂Ȃ����b�V�����\�[�X�̐ݒ�
		@param[in]	MeshKey	���b�V���L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const wstring& MeshKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�N�X�`�����\�[�X�̐ݒ�
		@param[in]	TextureRes	�e�N�X�`�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�N�X�`�����\�[�X�̐ݒ�
		@param[in]	TextureKey	�o�^����Ă���e�N�X�`���L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const wstring& TextureKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e�N�X�`�����\�[�X�̎擾
		@return	�e�N�X�`�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetTextureResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`���b�V�����\�[�X�𓾂�
		@param[in]	ExceptionActive	�G���[���ɗ�O���������邩�ǂ���
		@return	���b�V�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<MultiMeshResource> GetMultiMeshResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`���b�V�����\�[�X��ݒ肷��
		@param[in]	MeshResourcePtr	���b�V�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`���b�V�����\�[�X��ݒ肷��
		@param[in]	ResKey	���b�V�����\�[�X�̃L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMultiMeshResource(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�G�~�b�V�u�F�̎擾
		@return	�G�~�b�V�u�F
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetEmissive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�G�~�b�V�u�F�̐ݒ�
		@param[in]	col	�G�~�b�V�u�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetEmissive(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�t���[�Y�F�̎擾
		@return	�f�t���[�Y�F
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetDiffuse() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�t���[�Y�F�̐ݒ�
		@param[in]	col	�f�t���[�Y�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetDiffuse(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�y�L�����[�F�̎擾
		@return	�X�y�L�����[�F
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetSpecular() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�y�L�����[�F�̐ݒ�
		@param[in]	col	�X�y�L�����[�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetSpecular(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e��`�悷�邩�ǂ�������
		@return	�e��`�悷���true
		*/
		//--------------------------------------------------------------------------------------
		bool GetOwnShadowActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e��`�悷�邩�ǂ�������
		@return	�e��`�悷���true
		*/
		//--------------------------------------------------------------------------------------
		bool IsOwnShadowActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�e��`�悷�邩�ǂ����ݒ肷��
		@param[in]	b		�e��`�悷�邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetOwnShadowActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�t�B�[�Y�F�����f���ݒ�D�悩�ǂ�������
		@return	�f�t�B�[�Y�F�����f���ݒ�D��Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetModelDiffusePriority() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�t�B�[�Y�F�����f���ݒ�D�悩�ǂ�������
		@return	�f�t�B�[�Y�F�����f���ݒ�D��Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsModelDiffusePriority() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�t�B�[�Y�F�����f���ݒ�D�悩�ǂ����ݒ肷��
		@param[in]	b	�f�t�B�[�Y�F�����f���ݒ�D�悩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetModelDiffusePriority(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�G�~�b�V�u�F�����f���ݒ�D�悩�ǂ�������
		@return	�G�~�b�V�u�F�����f���ݒ�D��Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetModelEmissivePriority() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�G�~�b�V�u�F�����f���ݒ�D�悩�ǂ�������
		@return	�G�~�b�V�u�F�����f���ݒ�D��Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsModelEmissivePriority() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�G�~�b�V�u�F�����f���ݒ�D�悩�ǂ����ݒ肷��
		@param[in]	b	�G�~�b�V�u�F�����f���ݒ�D�悩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetModelEmissivePriority(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���f���̃e�N�X�`����L���ɂ��邩�ǂ�������
		@return	���f���̃e�N�X�`����L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetModelTextureEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���f���̃e�N�X�`����L���ɂ��邩�ǂ�������
		@return	���f���̃e�N�X�`����L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsModelTextureEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���f���̃e�N�X�`����L���ɂ��邩�ǂ����ݒ肷��
		@param[in]	b	���f���̃e�N�X�`����L���ɂ��邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetModelTextureEnabled(bool b);
		//Bone�`��p
		//--------------------------------------------------------------------------------------
		/*!
		@brief �{�[���̏�����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void BoneInit();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J�n�s�񂩂�I���s��̊Ԃ�t���Ԏ��̍s���Ԃ�
		@param[in]	m1	�J�n�̍s��
		@param[in]	m2	�I���̍s��
		@param[in]	t	���ԁi0����1.0f�j
		@param[out]	out	���ʂ��󂯎��s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void InterpolationMatrix(const bsm::Mat4x4& m1, const bsm::Mat4x4& m2, float t, bsm::Mat4x4& out);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A�j���[�V������ǉ�����
		@param[in]	Name	�A�j���[�V������
		@param[in]	StartSample	�J�n�T���v��
		@param[in]	SampleLength	�T���v���̒���
		@param[in]	Loop	���[�v���邩�ǂ���
		@param[in]	SamplesParSecond = 30.0f	1�b������̃T���v����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
			float SamplesParSecond = 30.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V������ύX����
		@param[in]	AnemationName	�A�j���[�V�������i���łɒǉ�����Ă�����́j
		@param[in]	StartTime = 0.0f	�J�n����̕b��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ChangeCurrentAnimation(const wstring& AnemationName, float StartTime = 0.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V�������𓾂�
		@return	�A�j���[�V�������i������j
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetCurrentAnimation() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V�����^�C���𓾂�
		@return	�A�j���[�V�����^�C��
		*/
		//--------------------------------------------------------------------------------------
		float GetCurrentAnimationTime() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V�������I�����Ă��邩�ǂ���
		@return	�C�����Ă����true
		*/
		//--------------------------------------------------------------------------------------
		bool IsTargetAnimeEnd() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���݂̃A�j���[�V������i�߂�
		@param[in]	ElapsedTime	�o�ߎ���
		@return	�A�j���[�V�������I�������true
		*/
		//--------------------------------------------------------------------------------------
		bool UpdateAnimation(float ElapsedTime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[�J���{�[���s��z��𓾂�
		@return	���[�J���{�[���s��z��̐擪�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< bsm::Mat4x4 >* GetVecLocalBonesPtr() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[�J���{�[���s��z��̎Q�Ƃ𓾂�(���������\)
		@return	���[�J���{�[���s��z��̎Q��
		*/
		//--------------------------------------------------------------------------------------
		vector< bsm::Mat4x4 >& GetVecLocalBones();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[�J���{�[���s��z��𓾂�i�}���`���b�V���Łj
		@return	���[�J���{�[���s��z��̐擪�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< bsm::Mat4x4 >* GetVecMultiLocalBonesPtr(size_t index) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���[�J���{�[���s��z��̎Q�Ƃ𓾂�(�}���`���b�V���ŔŁA���������\)
		@return	���[�J���{�[���s��z��̎Q�Ɓi�͈͊O�Ȃ��O�j
		*/
		//--------------------------------------------------------------------------------------
		vector< bsm::Mat4x4 >& GetVecMultiLocalBones(size_t index);
		//�C���X�^���X�`��p
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s��̏���𓾂�
		@return	�s��̏����
		*/
		//--------------------------------------------------------------------------------------
		size_t GetMaxInstance() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s��̏����ύX����
		@param[in]	NewSize		�V�������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ResizeMaxInstance(size_t NewSize);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����`��̍s���ǉ�����
		@param[in]	NewMatVec		�V�����s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void AddMatrix(const bsm::Mat4x4& NewMat);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����`��̍s������S�ɍX�V����
		@param[in]	NewMatVec		�V�����s��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateMultiMatrix(const vector<bsm::Mat4x4>& NewMatVec);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����`��̍s��̔z����擾����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		vector<bsm::Mat4x4>& GetMatrixVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����`��̍s����N���A����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ClearMatrixVec();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`���s��z��������N���A���邩�ǂ���
		@return	�N���A�����true
		*/
		//--------------------------------------------------------------------------------------
		bool IsAutoClearMatrixVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�`���s��z��������N���A���邩�ǂ�����ݒ�
		@param[in]	b	�`���s��z��������N���A���邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetAutoClearMatrixVec(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief�@�s��̃o�b�t�@���擾����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer>& GetMatrixBuffer() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V���̃��[�J�����_�z��𓾂�
		@param[out]	vertices	�󂯎�钸�_�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetStaticMeshLocalPositions(vector<bsm::Vec3>& vertices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V���̃��[���h���_�z��𓾂�
		@param[out]	vertices	�󂯎�钸�_�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetStaticMeshWorldPositions(vector<bsm::Vec3>& vertices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����ƃ��b�V���̃��[���h���_�̏Փ˔���
		@param[in]	StartPos	�����̊J�n�_
		@param[in]	EndPos	�����̏I���_
		@param[out]	HitPoint	�Փ˂��Ă����ꍇ�̏Փ˓_
		@param[out]	RetTri	�Փ˂��Ă����ꍇ�̎O�p�`
		@return	�Փ˂��Ă�����true
		*/
		//--------------------------------------------------------------------------------------
		bool HitTestStaticMeshSegmentTriangles(const bsm::Vec3& StartPos, const bsm::Vec3& EndPos, bsm::Vec3& HitPoint, TRIANGLE& RetTri);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�L�������ς݂̃��b�V���̃��[�J�����_�z��𓾂�
		@param[out]	vertices	�󂯎�钸�_�̔z��
		@return	�Ȃ��i�{�[�����Ȃ���Η�O�j
		*/
		//--------------------------------------------------------------------------------------
		void GetSkinedMeshLocalPositions(vector<bsm::Vec3>& vertices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�L�������ς݂̃��b�V���̃��[���h���_�z��𓾂�
		@param[out]	vertices	�󂯎�钸�_�̔z��
		@return	�Ȃ��i�{�[�����Ȃ���Η�O�j
		*/
		//--------------------------------------------------------------------------------------
		void GetSkinedMeshWorldPositions(vector<bsm::Vec3>& vertices);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�����ƃX�L�����b�V���̃��[���h���_�̏Փ˔���
		@param[in]	StartPos	�����̊J�n�_
		@param[in]	EndPos	�����̏I���_
		@param[out]	HitPoint	�Փ˂��Ă����ꍇ�̏Փ˓_
		@param[out]	RetTri	�Փ˂��Ă����ꍇ�̎O�p�`
		@return	�Փ˂��Ă�����true
		*/
		//--------------------------------------------------------------------------------------
		bool HitTestSkinedMeshSegmentTriangles(const bsm::Vec3& StartPos, const bsm::Vec3& EndPos, bsm::Vec3& HitPoint, TRIANGLE& RetTri);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	PCStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PCStaticDraw : public SmBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	PNStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PNStaticDraw : public SmBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PNStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	PTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PTStaticDraw : public SmBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PTStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	PCTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PCTStaticDraw : public SmBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCTStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	PNTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PNTStaticDraw : public SmBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTStaticDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	PNTStaticModel�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PNTStaticModelDraw : public SmBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTStaticModelDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	PNTBoneModel�`��R���|�[�l���g�i�{�[�����f���`��p�j
	//--------------------------------------------------------------------------------------
	class PNTBoneModelDraw : public SmBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTBoneModelDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V�����\�[�X�̐ݒ�
		@param[in]	MeshRes	���b�V�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���b�V�����\�[�X�̓o�^
		@param[in]	MeshKey	�o�^����Ă��郁�b�V���L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshResource(const wstring& MeshKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`���b�V�����\�[�X��ݒ肷��
		@param[in]	MeshResourcePtr	���b�V�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�}���`���b�V�����\�[�X��ݒ肷��
		@param[in]	ResKey	���b�V�����\�[�X�̃L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetMultiMeshResource(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	///	PCStaticInstance�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PCStaticInstanceDraw : public SmBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PCStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCStaticInstanceDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	PTStaticInstance�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PTStaticInstanceDraw : public SmBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PTStaticInstanceDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	PCTStaticInstance�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PCTStaticInstanceDraw : public SmBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PCTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PCTStaticInstanceDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	PNTStaticInstance�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class PNTStaticInstanceDraw : public SmBaseDraw {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit PNTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTStaticInstanceDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnCreate����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnUpdate�����i��֐��j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


}
//end basecross
