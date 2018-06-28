/*!
@file DrawComponents.cpp
@brief �`��R���|�[�l���g����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#include "stdafx.h"
#include <pix.h>


namespace basecross {

	IMPLEMENT_DX12SHADER(VSPNTStatic, App::GetApp()->GetShadersPath() + L"VSPNTStatic.cso")
	IMPLEMENT_DX12SHADER(PSPNTStatic, App::GetApp()->GetShadersPath() + L"PSPNTStatic.cso")


	//--------------------------------------------------------------------------------------
	///	�ėp�`��p
	//--------------------------------------------------------------------------------------
	GenericDraw::GenericDraw() :
		ObjectInterface() {}
	GenericDraw::~GenericDraw() {}
	void GenericDraw::OnCreate() {}

	void GenericDraw::PCWireFrameDraw(const shared_ptr<GameObject>& GameObjectPtr, const shared_ptr<MeshResource>& MeshRes,
		const bsm::Col4& Emissive, const bsm::Col4& Diffuse, const bsm::Mat4x4& MeshToTransformMatrix) {
		auto Dev = App::GetApp()->GetDeviceResources();
	}


	void GenericDraw::DrawWireFrame(const shared_ptr<GameObject>& GameObj,
		const shared_ptr<MeshResource>& Mesh, const bsm::Mat4x4& MeshToTransformMatrix) {

		PCWireFrameDraw(GameObj, Mesh,
			bsm::Col4(0, 0, 0, 0), bsm::Col4(1, 1, 1, 1), MeshToTransformMatrix);
	}


	//--------------------------------------------------------------------------------------
	//	struct DrawComponent::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DrawComponent::Impl {
		BlendState m_BlendState;
		DepthStencilState m_DepthStencilState;
		RasterizerState m_RasterizerState;
		SamplerState m_SamplerState;
		bsm::Mat4x4 m_MeshToTransformMatrix;
		Impl() :
			m_MeshToTransformMatrix()
		{}
	};


	//--------------------------------------------------------------------------------------
	///	�`��R���|�[�l���g�̐e�N���X����
	//--------------------------------------------------------------------------------------
	DrawComponent::DrawComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	DrawComponent::~DrawComponent() {}


	BlendState DrawComponent::GetBlendState() const {
		return pImpl->m_BlendState;
	}

	DepthStencilState DrawComponent::GetDepthStencilState() const {
		return pImpl->m_DepthStencilState;
	}
	RasterizerState DrawComponent::GetRasterizerState() const {
		return pImpl->m_RasterizerState;
	}
	SamplerState DrawComponent::GetSamplerState() const {
		return pImpl->m_SamplerState;
	}


	void DrawComponent::SetBlendState(const BlendState state) {
		pImpl->m_BlendState = state;
	}
	void DrawComponent::SetDeviceBlendState() {
	}


	void DrawComponent::SetDepthStencilState(const DepthStencilState state) {
		pImpl->m_DepthStencilState = state;

	}

	void DrawComponent::SetDeviceDepthStencilState() {
	}

	void DrawComponent::SetRasterizerState(const RasterizerState state) {
		pImpl->m_RasterizerState = state;
	}
	void DrawComponent::SetDeviceRasterizerState() {
	}


	void DrawComponent::SetSamplerState(const SamplerState state) {
		pImpl->m_SamplerState = state;
	}
	void DrawComponent::SetDeviceSamplerState() {
	}



	const bsm::Mat4x4& DrawComponent::GetMeshToTransformMatrix() const {
		return pImpl->m_MeshToTransformMatrix;
	}
	void DrawComponent::SetMeshToTransformMatrix(const bsm::Mat4x4& Mat) {
		pImpl->m_MeshToTransformMatrix = Mat;
	}



	//--------------------------------------------------------------------------------------
	//	struct Shadowmap::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct Shadowmap::Impl {
		static float m_LightHeight;	//���C�g�̍����i���������̒l�Ŋ|����j
		static float m_LightNear;	//���C�g��Near
		static float m_LightFar;		//���C�g��Far
		static float m_ViewWidth;
		static float m_ViewHeight;

		//���b�V�����\�[�X
		weak_ptr<MeshResource> m_MeshResource;
		//�}���`���b�V�����\�[�X
		weak_ptr<MultiMeshResource> m_MultiMeshResource;


		Impl()
		{}
		~Impl() {}
	};

	float Shadowmap::Impl::m_LightHeight(20.0f);
	float Shadowmap::Impl::m_LightNear(1.0f);
	float Shadowmap::Impl::m_LightFar(200.0f);
	float Shadowmap::Impl::m_ViewWidth(32.0f);
	float Shadowmap::Impl::m_ViewHeight(32.0f);



	//--------------------------------------------------------------------------------------
	//	class Shadowmap : public Component;
	//	�p�r: �V���h�E�}�b�v�R���|�[�l���g�i�O�����p�j
	//--------------------------------------------------------------------------------------
	Shadowmap::Shadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	Shadowmap::~Shadowmap() {}

	float Shadowmap::GetLightHeight() { return Impl::m_LightHeight; }
	float Shadowmap::GetLightNear() { return  Impl::m_LightNear; }
	float Shadowmap::GetLightFar() { return  Impl::m_LightFar; }
	float Shadowmap::GetViewWidth() { return  Impl::m_ViewWidth; }
	float Shadowmap::GetViewHeight() { return  Impl::m_ViewHeight; }

	void Shadowmap::SetLightHeight(float f) { Impl::m_LightHeight = f; }
	void Shadowmap::SetLightNear(float f) { Impl::m_LightNear = f; }
	void Shadowmap::SetLightFar(float f) { Impl::m_LightFar = f; }
	void Shadowmap::SetViewWidth(float f) { Impl::m_ViewWidth = f; }
	void Shadowmap::SetViewHeight(float f) { Impl::m_ViewHeight = f; }
	void Shadowmap::SetViewSize(float f) { Impl::m_ViewWidth = Impl::m_ViewHeight = f; }



	shared_ptr<MeshResource> Shadowmap::GetMeshResource(bool ExceptionActive) const {
		auto shptr = pImpl->m_MeshResource.lock();
		if (shptr) {
			return shptr;
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"���b�V�����\�[�X��������܂���",
					L"if (pImpl->m_MeshResource.expired())",
					L"ShadowmapComp::GetMeshResource()"
				);
			}
		}
		return nullptr;
	}


	void Shadowmap::SetMeshResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"���b�V���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"ShadowmapComp::SetMeshResource()"
				);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...) {
			throw;
		}
	}
	void Shadowmap::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr) {
		pImpl->m_MeshResource = MeshResourcePtr;
	}

	shared_ptr<MultiMeshResource> Shadowmap::GetMultiMeshResource(bool ExceptionActive) const {
		auto shptr = pImpl->m_MultiMeshResource.lock();
		if (shptr) {
			return shptr;
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"���b�V�����\�[�X��������܂���",
					L"if (pImpl->m_MultiMeshResource.expired())",
					L"ShadowmapComp::GetMultiMeshResource()"
				);
			}
		}
		return nullptr;
	}
	void Shadowmap::SetMultiMeshResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"���b�V���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"ShadowmapComp::SetMultiMeshResource()"
				);
			}
			pImpl->m_MultiMeshResource = App::GetApp()->GetResource<MultiMeshResource>(ResKey);
		}
		catch (...) {
			throw;
		}
	}
	void Shadowmap::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr) {
		pImpl->m_MultiMeshResource = MeshResourcePtr;
	}

	void Shadowmap::DrawSigle(const MeshPrimData& data) {
	}


	void Shadowmap::OnDraw() {

	}



	//--------------------------------------------------------------------------------------
	//ParticleDraw�����̂��߂̍\����
	//--------------------------------------------------------------------------------------
	struct DrawParticleSprite {
		//�p�[�e�B�N���̃J�����܂ł̋���
		float m_ToCaneraLength;
		//���[���h�s��
		bsm::Mat4x4 m_WorldMatrix;
		//�e�N�X�`��
		shared_ptr<TextureResource> m_TextureRes;
		//Diffuse�J���[
		bsm::Col4 m_Diffuse;
		DrawParticleSprite() :
			m_ToCaneraLength(0),
			m_Diffuse(1, 1, 1, 1)

		{}
	};



	//--------------------------------------------------------------------------------------
	//	struct PCTParticleDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PCTParticleDraw::Impl {
		//�`��R���e�L�X�g
		shared_ptr<MeshResource> m_MeshResource;	//���b�V�����\�[�X
		vector<DrawParticleSprite> m_DrawParticleSpriteVec;
		const size_t m_MaxInstance;				///<�C���X�^���X�ő�l
//		ComPtr<ID3D11Buffer> m_MatrixBuffer;	///<�s��p�̒��_�o�b�t�@
												//���Z�������邩�ǂ���
		bool m_Addtype;
		Impl(size_t MaxInstance, bool AddType) :
			m_MaxInstance(MaxInstance),
			m_Addtype(AddType)
		{}
		~Impl() {}
		//���_�o�b�t�@�̍쐬
		void CreateParticleBuffers();
	};


	void PCTParticleDraw::Impl::CreateParticleBuffers() {
		try {
			float HelfSize = 0.5f;
			bsm::Vec4 col(1.0f, 1.0f, 1.0f, 1.0f);
			//���_�z��
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, HelfSize, 0),  col,bsm::Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, HelfSize, 0), col, bsm::Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, -HelfSize, 0),  col,bsm::Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, -HelfSize, 0),  col, bsm::Vec2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//���b�V���̍쐬
			m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, false);

			//�C���X�^���X�s��o�b�t�@�̍쐬
			//Max�l�ō쐬����
			vector<bsm::Mat4x4> matrices(m_MaxInstance, bsm::Mat4x4());
//			MeshResource::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);

		}
		catch (...) {
			throw;
		}
	}




	//--------------------------------------------------------------------------------------
	///	PCTParticle�`��R���|�[�l���g(�p�[�e�B�N���`��)
	//--------------------------------------------------------------------------------------
	PCTParticleDraw::PCTParticleDraw(const shared_ptr<GameObject>& GameObjectPtr, size_t MaxInstance, bool AddType) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl(MaxInstance, AddType))
	{}

	PCTParticleDraw::~PCTParticleDraw() {}

	void PCTParticleDraw::AddParticle(float ToCaneraLength, const bsm::Mat4x4& WorldMatrix,
		const shared_ptr<TextureResource>& TextureRes, const bsm::Col4& Diffuse) {
		DrawParticleSprite Item;
		Item.m_ToCaneraLength = ToCaneraLength;
		Item.m_WorldMatrix = WorldMatrix;
		Item.m_TextureRes = TextureRes;
		Item.m_Diffuse = Diffuse;
		pImpl->m_DrawParticleSpriteVec.push_back(Item);
	}

	void PCTParticleDraw::OnCreate() {
		pImpl->CreateParticleBuffers();
	}

	struct InstanceDrawStr {
		size_t Start;
		size_t Count;
		shared_ptr<TextureResource> Tex;
		bsm::Col4 m_Diffuse;
		InstanceDrawStr(size_t s, size_t c, shared_ptr<TextureResource> t,
			const bsm::Col4& Diffuse) :
			Start(s), Count(c), Tex(t), m_Diffuse(Diffuse) {}
	};

	void PCTParticleDraw::OnDraw() {

	}


	//--------------------------------------------------------------------------------------
	//	SpriteBaseDraw::Impl
	//--------------------------------------------------------------------------------------
	struct SpriteBaseDraw::Impl {
		//���b�V��
		shared_ptr<MeshResource> m_SpriteMesh;
		///�e�N�X�`�����\�[�X
		weak_ptr<TextureResource> m_TextureResource;
		//�G�~�b�V�u�F
		bsm::Col4 m_Emissive;
		//�f�t���[�Y�F
		bsm::Col4 m_Diffuse;
		Impl() :
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f)
		{}
	};

	SpriteBaseDraw::SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}

	SpriteBaseDraw::~SpriteBaseDraw() {}

	void SpriteBaseDraw::SetConstants(SpriteConstants& SpCb) {
		//�s��̎擾
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//�s��̒�`
		bsm::Mat4x4 World, Proj;
		World = PtrTrans->Get2DWorldMatrix();
		//�ˉe�s��̌���
		GetGameObject()->OnGet2DDrawProjMatrix(Proj);
		//�s��̍���
		World *= Proj;
		//�G�~�b�V�u
		SpCb.Emissive = GetEmissive();
		//�f�t�B�[�Y�͂��ׂĒʂ�
		SpCb.Diffuse = GetDiffuse();
		//�s��̐ݒ�
		SpCb.World = World;
	}

	shared_ptr<MeshResource> SpriteBaseDraw::GetMeshResource() const {
		return pImpl->m_SpriteMesh;
	}
	void SpriteBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_SpriteMesh = MeshRes;
	}

	void SpriteBaseDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_TextureResource = TextureRes;

	}

	void SpriteBaseDraw::SetTextureResource(const wstring& TextureKey) {
		this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<TextureResource> SpriteBaseDraw::GetTextureResource() const {
		//�e�N�X�`�����Ȃ����null��Ԃ�
		auto shptr = pImpl->m_TextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;
	}



	bsm::Col4 SpriteBaseDraw::GetEmissive() const {
		return pImpl->m_Emissive;
	}
	void SpriteBaseDraw::SetEmissive(const bsm::Col4& col) {
		pImpl->m_Emissive = col;
	}

	bsm::Col4 SpriteBaseDraw::GetDiffuse() const {
		return pImpl->m_Diffuse;
	}
	void SpriteBaseDraw::SetDiffuse(const bsm::Col4& col) {
		pImpl->m_Diffuse = col;
	}

	//--------------------------------------------------------------------------------------
	///	PCSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColor>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		try {
			CreateMesh(Vertices, indices);
		}
		catch (...) {
			throw;
		}
	}

	PCSpriteDraw::~PCSpriteDraw() {}

	void PCSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//�쐬����ĂȂ�������쐬����
			float HelfSize = 0.5f;
			//���_�z��
			vector<VertexPositionColor> Vertices = {
				{ VertexPositionColor(bsm::Vec3(-HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			{ VertexPositionColor(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			{ VertexPositionColor(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			{ VertexPositionColor(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			CreateMesh(Vertices, indices);
		}
	}

	void PCSpriteDraw::OnDraw() {
	}

	//--------------------------------------------------------------------------------------
	///	PTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PTSpriteDraw::PTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionTexture>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		try {
			CreateMesh(Vertices, indices);
		}
		catch (...) {
			throw;
		}
	}

	PTSpriteDraw::~PTSpriteDraw() {}

	void PTSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//�쐬����ĂȂ�������쐬����
			float HelfSize = 0.5f;
			//���_�z��
			vector<VertexPositionTexture> Vertices = {
				{ VertexPositionTexture(bsm::Vec3(-HelfSize, HelfSize, 0), bsm::Vec2(0.0f, 0.0f)) },
			{ VertexPositionTexture(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Vec2(1.0f, 0.0f)) },
			{ VertexPositionTexture(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Vec2(0.0f, 1.0f)) },
			{ VertexPositionTexture(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Vec2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			CreateMesh(Vertices, indices);
		}
	}

	void PTSpriteDraw::OnDraw() {

	}

	//--------------------------------------------------------------------------------------
	///	PCTSprite�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}


	PCTSpriteDraw::PCTSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr,
		vector<VertexPositionColorTexture>& Vertices, vector<uint16_t>& indices) :
		SpriteBaseDraw(GameObjectPtr)
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g��2D
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::None);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
		try {
			CreateMesh(Vertices, indices);
		}
		catch (...) {
			throw;
		}
	}

	PCTSpriteDraw::~PCTSpriteDraw() {}

	void PCTSpriteDraw::OnCreate() {
		auto SpriteMesh = GetMeshResource();
		if (!SpriteMesh) {
			//�쐬����ĂȂ�������쐬����
			float HelfSize = 0.5f;
			//���_�z��
			vector<VertexPositionColorTexture> Vertices = {
				{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, HelfSize, 0),bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(-HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(bsm::Vec3(HelfSize, -HelfSize, 0), bsm::Col4(1.0f,1.0f,1.0f,1.0f), bsm::Vec2(1.0f, 1.0f)) },
			};
			//�C���f�b�N�X�z��
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//���b�V���̍쐬�i�ύX�ł���j
			CreateMesh(Vertices, indices);
		}
	}

	void PCTSpriteDraw::OnDraw() {
	}

	//static�ϐ��̎���
	vector<bsm::Vec3> DrawObjectBase::m_TempPositions;


	//--------------------------------------------------------------------------------------
	///	Simple�`��Ɏg�p����\����(�e�Ή�)
	//--------------------------------------------------------------------------------------
	struct SmDrawObject :public DrawObjectBase {
		//���_�ύX����ꍇ�̃��b�V���i�I���W�i���j
		shared_ptr<MeshResource> m_OriginalMeshResource;
		//�I���W�i�����b�V�����g�����ǂ���
		bool m_UseOriginalMeshResource;
		///�e�N�X�`�����\�[�X
		weak_ptr<TextureResource> m_TextureResource;
		///�G�~�b�V�u�F
		bsm::Col4 m_Emissive;
		/// �f�t���[�Y�F
		bsm::Col4 m_Diffuse;
		/// �X�y�L�����[�F
		bsm::Col4 m_Specular;
		///�e�𓊉e���邩�ǂ���
		bool m_OwnShadowActive;
		//���f���ɓ����Ă���Diffuse���g�����ǂ���
		bool m_ModelDiffusePriority;
		//���f���ɓ����Ă���Emissive���g�����ǂ���
		bool m_ModelEmissivePriority;
		//���f���ɓ����Ă���e�N�X�`�����g�����ǂ���
		bool m_ModelTextureEnabled;
		///Instance�`��p
		///Instance�ő�l
		size_t m_MaxInstance;
		/// �s��p�̃��b�V��
		shared_ptr<MeshResource> m_InstanceMatrixMesh;
		///�s��̔z��
		vector<bsm::Mat4x4> m_MatrixVec;
		//�s��������N���A���邩�ǂ���
		bool m_AutoClearMatrixVec;
		SmDrawObject() :
			m_UseOriginalMeshResource(false),
			m_Emissive(0, 0, 0, 0),
			m_Diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_Specular(0, 0, 0, 0),
			m_OwnShadowActive(false),
			m_ModelDiffusePriority(false),
			m_ModelEmissivePriority(false),
			m_ModelTextureEnabled(true),
			m_MaxInstance(2000),
			m_AutoClearMatrixVec(false)
		{}
	};

	//--------------------------------------------------------------------------------------
	//	SmBaseDraw::Impl
	//--------------------------------------------------------------------------------------
	struct SmBaseDraw::Impl {
		SmDrawObject m_SmDrawObject;
	};

	//--------------------------------------------------------------------------------------
	///	Simple�`��I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	SmBaseDraw::SmBaseDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{
		//�p�C�v���C���X�e�[�g���f�t�H���g�̂RD
		SetBlendState(BlendState::Opaque);
		SetDepthStencilState(DepthStencilState::Default);
		SetRasterizerState(RasterizerState::CullBack);
		SetSamplerState(SamplerState::LinearClamp);
	}

	SmBaseDraw::~SmBaseDraw() {}

	void SmBaseDraw::SetConstants(Dx12Constants<SimpleConstants>& SmCb) {
		//�s��̒�`
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//�s��̒�`
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//���[���h�s��̌���
//		if (data.m_UseMeshToTransformMatrix) {
//			World = data.m_MeshToTransformMatrix * GetMeshToTransformMatrix();
//			World *= PtrTrans->GetWorldMatrix();
//		}
//		else {
			World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
//		}
		//�]�u����
		World.transpose();
		//�J�����𓾂�
		auto CameraPtr = GetGameObject()->OnGetDrawCamera();
		//�r���[�Ǝˉe�s��𓾂�
		ViewMat = CameraPtr->GetViewMatrix();
		//�]�u����
		ViewMat.transpose();
		//�]�u����
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		SmCb.m_ConstantBuffer.World = World;
		SmCb.m_ConstantBuffer.View = ViewMat;
		SmCb.m_ConstantBuffer.Projection = ProjMat;
		//�G�~�b�V�u
		SmCb.m_ConstantBuffer.Emissive = GetEmissive();
		//�f�t�B�[�Y
		SmCb.m_ConstantBuffer.Diffuse = GetDiffuse();
		//�X�y�L�����[
		SmCb.m_ConstantBuffer.Specular = GetSpecular();
		//���C�e�B���O
		auto StageLight = GetGameObject()->OnGetDrawLight();
		SmCb.m_ConstantBuffer.LightDir = StageLight.m_Directional;
		SmCb.m_ConstantBuffer.LightDir.w = 1.0f;
		SmCb.m_ConstantBuffer.EyePos = CameraPtr->GetEye();
		SmCb.m_ConstantBuffer.EyePos.w = 1.0f;
		//�e�p
		if (GetOwnShadowActive()) {
/*
			bsm::Vec3 CalcLightDir = -1.0 * StageLight.m_Directional;
			bsm::Vec3 LightAt = CameraPtr->GetAt();
			bsm::Vec3 LightEye = CalcLightDir;
			LightEye *= Shadowmap::GetLightHeight();
			LightEye = LightAt + LightEye;
			SmCb.LightPos = LightEye;
			SmCb.LightPos.w = 1.0f;
			bsm::Mat4x4 LightView, LightProj;
			//���C�g�̃r���[�Ǝˉe���v�Z
			LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
				Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
			SmCb.LightView = bsm::transpose(LightView);
			SmCb.LightProjection = bsm::transpose(LightProj);
*/
		}
		//�{�[���̐ݒ�
/*
		size_t BoneSz = pImpl->m_SmDrawObject.m_LocalBonesMatrix.size();
		if (BoneSz > 0) {
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				bsm::Mat4x4 mat = pImpl->m_SmDrawObject.m_LocalBonesMatrix[b];
				mat.transpose();
				SmCb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
				SmCb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				SmCb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}
		}
		else if (pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix.size() > data.m_MultiMeshIndex) {
			//�}���`���b�V���̃{�[����������
			//�{�[���̐ݒ�
			BoneSz = pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix[data.m_MultiMeshIndex].size();
			if (BoneSz > 0) {
				UINT cb_count = 0;
				for (size_t b = 0; b < BoneSz; b++) {
					bsm::Mat4x4 mat = pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix[data.m_MultiMeshIndex][b];
					mat.transpose();
					SmCb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
					SmCb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
					SmCb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
					cb_count += 3;
				}
			}
		}
*/
	}

	//�s��o�b�t�@�̍쐬
	void SmBaseDraw::CreateMatrixBuffer() {
/*
		//�C���X�^���X�s��o�b�t�@�̍쐬
		//Max�l�ō쐬����
		vector<bsm::Mat4x4> matrices(pImpl->m_SmDrawObject.m_MaxInstance);
		for (auto& m : matrices) {
			m = bsm::Mat4x4();
		}
		MeshResource::CreateDynamicVertexBuffer(pImpl->m_SmDrawObject.m_MatrixBuffer, matrices);
*/
	}

	//�s��o�b�t�@�̃}�b�v
	void SmBaseDraw::MapMatrixBuffer() {
/*
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�C���X�^���X�o�b�t�@�Ƀ}�b�v
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//�s��̃}�b�v
		if (FAILED(pID3D11DeviceContext->Map(pImpl->m_SmDrawObject.m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map���s
			throw BaseException(
				L"�s���Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"SmBaseDraw::MapMatrixBuffer()"
			);
		}
		//�s��̕ύX
		auto* matrices = (bsm::Mat4x4*)mappedBuffer.pData;
		bsm::Mat4x4 World;
		for (size_t i = 0; i < pImpl->m_SmDrawObject.m_MatrixVec.size(); i++) {
			//���[���h�s��
			World = pImpl->m_SmDrawObject.m_MatrixVec[i];
			//�]�u����
			World.transpose();
			matrices[i] = World;
		}
		//�A���}�b�v
		pID3D11DeviceContext->Unmap(pImpl->m_SmDrawObject.m_MatrixBuffer.Get(), 0);
*/
	}

	bool SmBaseDraw::IsOriginalMeshUse() const {
		return pImpl->m_SmDrawObject.m_UseOriginalMeshResource;

	}

	void SmBaseDraw::SetOriginalMeshUse(bool b) {
		pImpl->m_SmDrawObject.m_UseOriginalMeshResource = b;
	}

	shared_ptr<MeshResource> SmBaseDraw::GetOriginalMeshResource() const {
		return pImpl->m_SmDrawObject.m_OriginalMeshResource;

	}

	void SmBaseDraw::SetOriginalMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_SmDrawObject.m_OriginalMeshResource = MeshRes;

	}

	shared_ptr<MeshResource> SmBaseDraw::GetMeshResource() const {
		if (IsOriginalMeshUse()) {
			return pImpl->m_SmDrawObject.m_OriginalMeshResource;
		}
		else {
			auto shptr = pImpl->m_SmDrawObject.m_MeshResource.lock();
			if (shptr) {
				return shptr;
			}
		}
		return nullptr;
	}

	void SmBaseDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		pImpl->m_SmDrawObject.m_MeshResource = MeshRes;
	}

	void SmBaseDraw::SetMeshResource(const wstring& MeshKey) {
		this->SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}

	void SmBaseDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
		pImpl->m_SmDrawObject.m_TextureResource = TextureRes;

	}

	void SmBaseDraw::SetTextureResource(const wstring& TextureKey) {
		this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
	}

	shared_ptr<TextureResource> SmBaseDraw::GetTextureResource() const {
		//�e�N�X�`�����Ȃ����null��Ԃ�
		auto shptr = pImpl->m_SmDrawObject.m_TextureResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;

	}

	shared_ptr<MultiMeshResource> SmBaseDraw::GetMultiMeshResource() const {
		//�Ȃ����null��Ԃ�
		auto shptr = pImpl->m_SmDrawObject.m_MultiMeshResource.lock();
		if (shptr) {
			return shptr;
		}
		return nullptr;

	}

	void SmBaseDraw::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr) {
		pImpl->m_SmDrawObject.m_MultiMeshResource = MeshResourcePtr;

	}

	void SmBaseDraw::SetMultiMeshResource(const wstring& ResKey) {
		this->SetMultiMeshResource(App::GetApp()->GetResource<MultiMeshResource>(ResKey));

	}

	bsm::Col4 SmBaseDraw::GetEmissive() const {
		return pImpl->m_SmDrawObject.m_Emissive;

	}

	void SmBaseDraw::SetEmissive(const bsm::Col4& col) {
		pImpl->m_SmDrawObject.m_Emissive = col;
	}

	bsm::Col4 SmBaseDraw::GetDiffuse() const {
		return pImpl->m_SmDrawObject.m_Diffuse;

	}

	void SmBaseDraw::SetDiffuse(const bsm::Col4& col) {
		pImpl->m_SmDrawObject.m_Diffuse = col;

	}

	bsm::Col4 SmBaseDraw::GetSpecular() const {
		return pImpl->m_SmDrawObject.m_Specular;

	}

	void SmBaseDraw::SetSpecular(const bsm::Col4& col) {
		pImpl->m_SmDrawObject.m_Specular = col;
	}

	bool SmBaseDraw::GetOwnShadowActive() const {
		return pImpl->m_SmDrawObject.m_OwnShadowActive;

	}

	bool SmBaseDraw::IsOwnShadowActive() const {
		return pImpl->m_SmDrawObject.m_OwnShadowActive;

	}

	void SmBaseDraw::SetOwnShadowActive(bool b) {
		pImpl->m_SmDrawObject.m_OwnShadowActive = b;
	}

	bool SmBaseDraw::GetModelDiffusePriority() const {
		return pImpl->m_SmDrawObject.m_ModelDiffusePriority;

	}

	bool SmBaseDraw::IsModelDiffusePriority() const {
		return pImpl->m_SmDrawObject.m_ModelDiffusePriority;

	}

	void SmBaseDraw::SetModelDiffusePriority(bool b) {
		pImpl->m_SmDrawObject.m_ModelDiffusePriority = b;

	}

	bool SmBaseDraw::GetModelEmissivePriority() const {
		return pImpl->m_SmDrawObject.m_ModelEmissivePriority;

	}

	bool SmBaseDraw::IsModelEmissivePriority() const {
		return pImpl->m_SmDrawObject.m_ModelEmissivePriority;

	}

	void SmBaseDraw::SetModelEmissivePriority(bool b) {
		pImpl->m_SmDrawObject.m_ModelEmissivePriority = b;
	}

	bool SmBaseDraw::GetModelTextureEnabled() const {
		return pImpl->m_SmDrawObject.m_ModelTextureEnabled;

	}

	bool SmBaseDraw::IsModelTextureEnabled() const {
		return pImpl->m_SmDrawObject.m_ModelTextureEnabled;

	}

	void SmBaseDraw::SetModelTextureEnabled(bool b) {
		pImpl->m_SmDrawObject.m_ModelTextureEnabled = b;

	}

	void SmBaseDraw::BoneInit() {
		pImpl->m_SmDrawObject.BoneInit();

	}

	void SmBaseDraw::InterpolationMatrix(const bsm::Mat4x4& m1, const bsm::Mat4x4& m2, float t, bsm::Mat4x4& out) {
		pImpl->m_SmDrawObject.InterpolationMatrix(m1, m2, t, out);

	}

	void SmBaseDraw::AddAnimation(const wstring& Name, int StartSample, int SampleLength, bool Loop,
		float SamplesParSecond) {
		pImpl->m_SmDrawObject.AddAnimation(Name, StartSample, SampleLength, Loop, SamplesParSecond);

	}

	void SmBaseDraw::ChangeCurrentAnimation(const wstring& AnemationName, float StartTime) {
		pImpl->m_SmDrawObject.ChangeCurrentAnimation(AnemationName, StartTime);

	}

	const wstring& SmBaseDraw::GetCurrentAnimation() const {
		return pImpl->m_SmDrawObject.m_CurrentAnimeName;

	}

	float SmBaseDraw::GetCurrentAnimationTime() const {
		return pImpl->m_SmDrawObject.m_CurrentAnimeTime;
	}

	bool SmBaseDraw::IsTargetAnimeEnd() const {
		auto& AnimData = pImpl->m_SmDrawObject.GetAnimationData();
		return AnimData.m_IsAnimeEnd;
	}


	bool SmBaseDraw::UpdateAnimation(float ElapsedTime) {
		return pImpl->m_SmDrawObject.UpdateAnimation(ElapsedTime);

	}

	const vector< bsm::Mat4x4 >* SmBaseDraw::GetVecLocalBonesPtr() const {
		return &pImpl->m_SmDrawObject.m_LocalBonesMatrix;
	}

	vector< bsm::Mat4x4 >& SmBaseDraw::GetVecLocalBones() {
		return pImpl->m_SmDrawObject.m_LocalBonesMatrix;
	}


	const vector< bsm::Mat4x4 >* SmBaseDraw::GetVecMultiLocalBonesPtr(size_t index) const {
		if (pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix.size() > index) {
			return &(pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix[index]);
		}
		return nullptr;
	}

	vector< bsm::Mat4x4 >& SmBaseDraw::GetVecMultiLocalBones(size_t index) {
		if (pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix.size() <= index) {
			throw BaseException(
				L"�C���f�b�N�X������𒴂��Ă܂�",
				L"if (pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix.size() <= index)",
				L"SmBaseDraw::GetVecMultiLocalBones"
			);
		}
		return pImpl->m_SmDrawObject.m_MultiLocalBonesMatrix[index];
	}



	size_t SmBaseDraw::GetMaxInstance() const {
		return pImpl->m_SmDrawObject.m_MaxInstance;
	}

	void SmBaseDraw::ResizeMaxInstance(size_t NewSize) {
		pImpl->m_SmDrawObject.m_MaxInstance = NewSize;
		CreateMatrixBuffer();
	}

	void SmBaseDraw::AddMatrix(const bsm::Mat4x4& NewMat) {
		if (pImpl->m_SmDrawObject.m_MatrixVec.size() >= GetMaxInstance()) {
			throw BaseException(
				L"�C���X�^���X����𒴂��Ă܂�",
				L"if (pImpl->m_MatrixVec.size() >= GetMaxInstance())",
				L"SmBaseDraw::AddMatrix()"
			);
		}
		pImpl->m_SmDrawObject.m_MatrixVec.push_back(NewMat);
	}

	void SmBaseDraw::UpdateMultiMatrix(const vector<bsm::Mat4x4>& NewMatVec) {
		pImpl->m_SmDrawObject.m_MatrixVec.resize(NewMatVec.size());
		pImpl->m_SmDrawObject.m_MatrixVec = NewMatVec;
	}

	vector<bsm::Mat4x4>& SmBaseDraw::GetMatrixVec() const {
		return pImpl->m_SmDrawObject.m_MatrixVec;
	}

	void SmBaseDraw::ClearMatrixVec() {
		pImpl->m_SmDrawObject.m_MatrixVec.clear();
	}

	bool SmBaseDraw::IsAutoClearMatrixVec() const {
		return pImpl->m_SmDrawObject.m_AutoClearMatrixVec;
	}
	void SmBaseDraw::SetAutoClearMatrixVec(bool b) {
		pImpl->m_SmDrawObject.m_AutoClearMatrixVec = b;
	}
/*
	ComPtr<ID3D11Buffer>& SmBaseDraw::GetMatrixBuffer() const {
		return pImpl->m_SmDrawObject.m_MatrixBuffer;
	}
*/
	void SmBaseDraw::GetStaticMeshLocalPositions(vector<bsm::Vec3>& vertices) {
/*
		auto ReshRes = GetMeshResource();
		if (!ReshRes) {
			throw BaseException(
				L"���b�V�����\�[�X������܂���",
				L"if (!ReshRes)",
				L"SmBaseDraw::GetStaticMeshLocalPositions()"
			);
		}
		vertices.clear();
		ReshRes->GetLocalPositions(vertices);
*/
	}

	void SmBaseDraw::GetStaticMeshWorldPositions(vector<bsm::Vec3>& vertices) {
		GetStaticMeshLocalPositions(vertices);
		//���[���h�s��̔��f
		auto WorldMat = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		for (auto& v : vertices) {
			v *= WorldMat;
		}
	}

	bool SmBaseDraw::HitTestStaticMeshSegmentTriangles(const bsm::Vec3& StartPos, const bsm::Vec3& EndPos, bsm::Vec3& HitPoint,
		TRIANGLE& RetTri) {
		GetStaticMeshWorldPositions(pImpl->m_SmDrawObject.m_TempPositions);
		for (size_t i = 0; i < pImpl->m_SmDrawObject.m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_SmDrawObject.m_TempPositions[i];
			tri.m_B = pImpl->m_SmDrawObject.m_TempPositions[i + 1];
			tri.m_C = pImpl->m_SmDrawObject.m_TempPositions[i + 2];
			bsm::Vec3 ret;
			float t;
			if (HitTest::SEGMENT_TRIANGLE(StartPos, EndPos, tri, ret, t)) {
				auto Len = length(EndPos - StartPos);
				Len *= t;
				auto Nomal = EndPos - StartPos;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartPos + Nomal;
				RetTri = tri;
				return true;
			}
		}
		return false;
	}


	void SmBaseDraw::GetSkinedMeshLocalPositions(vector<bsm::Vec3>& vertices) {
/*
		if (GetVecLocalBones().size() == 0) {
			throw BaseException(
				L"�{�[���s�񂪂���܂���",
				L"if (GetVecLocalBones().size() == 0)",
				L"SmBaseDraw::GetSkinedMeshLocalPositions()"
			);
		}
		auto ReshRes = GetMeshResource();
		if (!ReshRes) {
			throw BaseException(
				L"���b�V�����\�[�X������܂���",
				L"if (!ReshRes)",
				L"SmBaseDraw::GetSkinedMeshLocalPositions()"
			);
		}
		vertices.clear();
		auto& Bones = GetVecLocalBones();
		auto& PosVec = ReshRes->GetVerteces();
		auto& SkinVec = ReshRes->GetSkins();
		for (auto& v : PosVec) {
			vertices.push_back(v.position);
		}
		//�X�L�j���O����
		for (size_t j = 0; j < vertices.size(); j++) {
			bsm::Mat4x4 skinning(0);
			for (size_t i = 0; i < 4; i++)
			{
				skinning += Bones[SkinVec[j].indices[i]] * SkinVec[j].weights[i];
			}
			skinning._14 = 1.0f;
			skinning._24 = 1.0f;
			skinning._34 = 1.0f;
			skinning._44 = 1.0f;
			bsm::Vec4 p(vertices[j]);
			p.w = 1.0f;
			p *= skinning;
			vertices[j] = p;
		}
*/
	}

	void SmBaseDraw::GetSkinedMeshWorldPositions(vector<bsm::Vec3>& vertices) {
		GetSkinedMeshLocalPositions(vertices);
		//���[���h�s��̔��f
		auto WorldMat = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
		for (auto& v : vertices) {
			v *= WorldMat;
		}
	}

	bool SmBaseDraw::HitTestSkinedMeshSegmentTriangles(const bsm::Vec3& StartPos, const bsm::Vec3& EndPos,
		bsm::Vec3& HitPoint, TRIANGLE& RetTri) {
		GetSkinedMeshWorldPositions(pImpl->m_SmDrawObject.m_TempPositions);
		for (size_t i = 0; i < pImpl->m_SmDrawObject.m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_SmDrawObject.m_TempPositions[i];
			tri.m_B = pImpl->m_SmDrawObject.m_TempPositions[i + 1];
			tri.m_C = pImpl->m_SmDrawObject.m_TempPositions[i + 2];
			bsm::Vec3 ret;
			float t;
			if (HitTest::SEGMENT_TRIANGLE(StartPos, EndPos, tri, ret, t)) {
				auto Len = length(EndPos - StartPos);
				Len *= t;
				auto Nomal = EndPos - StartPos;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartPos + Nomal;
				RetTri = tri;
				return true;
			}
		}
		return false;
	}



	///�V�F�[�_�[���\�[�X�r���[�i�e�N�X�`���j�쐬
	void SmBaseDraw::CreateShaderResourceView() {
		auto ShPtr = GetTextureResource();
		if (!ShPtr) {
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		//�e�N�X�`���n���h�����쐬
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(
			m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
			0,
			0
		);
		//�e�N�X�`���̃V�F�[�_���\�[�X�r���[���쐬
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		//�t�H�[�}�b�g
		srvDesc.Format = ShPtr->GetTextureResDesc().Format;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = ShPtr->GetTextureResDesc().MipLevels;
		//�V�F�[�_���\�[�X�r���[
		Dev->GetDevice()->CreateShaderResourceView(
			ShPtr->GetTexture().Get(),
			&srvDesc,
			Handle);
	}

	void SmBaseDraw::RefreshTrace() {
		//�u�����h�X�e�[�g�ƃ��X�^���C�U�����ւ�
		if (GetGameObject()->IsAlphaActive()) {
			D3D12_BLEND_DESC blend_desc;
			D3D12_RENDER_TARGET_BLEND_DESC Target;
			ZeroMemory(&blend_desc, sizeof(blend_desc));
			blend_desc.AlphaToCoverageEnable = false;
			blend_desc.IndependentBlendEnable = false;
			ZeroMemory(&Target, sizeof(Target));
			Target.BlendEnable = true;
			Target.SrcBlend = D3D12_BLEND_SRC_ALPHA;
			Target.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			Target.BlendOp = D3D12_BLEND_OP_ADD;
			Target.SrcBlendAlpha = D3D12_BLEND_ONE;
			Target.DestBlendAlpha = D3D12_BLEND_ZERO;
			Target.BlendOpAlpha = D3D12_BLEND_OP_ADD;
			Target.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
				blend_desc.RenderTarget[i] = Target;
			}
			m_PineLineDesc.BlendState = blend_desc;
		}
		else {
			m_PineLineDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		}
		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		m_CullFrontPipelineState = PipelineState::CreateDirect(m_PineLineDesc);

		m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		m_CullBackPipelineState = PipelineState::CreateDirect(m_PineLineDesc);

	}

	void SmBaseDraw::UpdateConstantBuffer() {
		memcpy(m_pConstantBuffer, reinterpret_cast<void**>(&m_SimpleConstants.m_ConstantBuffer),
			sizeof(m_SimpleConstants.m_ConstantBuffer));
	}


	///�`�揈��
	void SmBaseDraw::DrawObject() {
		auto ShMesh = GetMeshResource();
		auto ShTex = GetTextureResource();
		if (!ShMesh) {
			return;
		}

		//�R�}���h���X�g�̃��Z�b�g
		if (GetGameObject()->IsAlphaActive()) {
			CommandList::Reset(m_CullFrontPipelineState, m_CommandList);
		}
		else {
			CommandList::Reset(m_CullBackPipelineState, m_CommandList);
		}
		ShMesh->UpdateResources<VertexPositionNormalTexture>(m_CommandList);
		if (ShTex) {
			ShTex->UpdateResources(m_CommandList);
		}
		//�`��
		m_CommandList->SetGraphicsRootSignature(m_RootSignature.Get());
		ID3D12DescriptorHeap* ppHeaps[] = { m_DescriptorHeap.Get(), m_SamplerDescriptorHeap.Get() };
		m_CommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

		for (size_t i = 0; i < m_GPUDescriptorHandleVec.size(); i++) {
			m_CommandList->SetGraphicsRootDescriptorTable((UINT)i, m_GPUDescriptorHandleVec[i]);
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		m_CommandList->RSSetViewports(1, &Dev->GetViewport());
		m_CommandList->RSSetScissorRects(1, &Dev->GetScissorRect());
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
			Dev->GetRtvHeap()->GetCPUDescriptorHandleForHeapStart(),
			Dev->GetFrameIndex(),
			Dev->GetRtvDescriptorSize());
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(
			Dev->GetDsvHeap()->GetCPUDescriptorHandleForHeapStart()
		);
		m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

		m_CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_CommandList->IASetIndexBuffer(&ShMesh->GetIndexBufferView());
		m_CommandList->IASetVertexBuffers(0, 1, &ShMesh->GetVertexBufferView());


		m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		if (GetGameObject()->IsAlphaActive()) {
			m_CommandList->SetPipelineState(m_CullBackPipelineState.Get());
			m_CommandList->DrawIndexedInstanced(ShMesh->GetNumIndicis(), 1, 0, 0, 0);
		}

		//�R�}���h���X�g�̃N���[�Y
		CommandList::Close(m_CommandList);
		//�f�o�C�X�ɃR�}���h���X�g�𑗂�
		Dev->InsertDrawCommandLists(m_CommandList.Get());
	}

	void SmBaseDraw::DrawPNTNotShadow() {
		auto PtrStage = GetGameObject()->GetStage();
		auto PtrMeshResource = GetMeshResource();
		if (GetTextureResource()) {
			m_SimpleConstants.m_ConstantBuffer.ActiveFlg.x = 1;
			CreateShaderResourceView();
		}
		else {
			m_SimpleConstants.m_ConstantBuffer.ActiveFlg.x = 0;
		}
		SetConstants(m_SimpleConstants);
		//�X�V
		UpdateConstantBuffer();
		DrawObject();
	}


	void SmBaseDraw::CreatePNTNotShadow() {
		//���[�g�V�O�l�`��
		m_RootSignature = RootSignature::CreateSrvSmpCbv();
		//�f�X�N�v���^�q�[�v
		auto Dev = App::GetApp()->GetDeviceResources();
		{
			m_DescriptorHandleIncrementSize =
				Dev->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
			//�f�X�N�v���^�q�[�v
			m_DescriptorHeap = DescriptorHeap::CreateCbvSrvUavHeap(1 + 1);
			//�T���v���[�f�X�N�v���^�q�[�v
			m_SamplerDescriptorHeap = DescriptorHeap::CreateSamplerHeap(1);
			//GPU���f�X�N�v���^�q�[�v�̃n���h���̔z��̍쐬
			m_GPUDescriptorHandleVec.clear();
			CD3DX12_GPU_DESCRIPTOR_HANDLE SrvHandle(
				m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			m_GPUDescriptorHandleVec.push_back(SrvHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE SamplerHandle(
				m_SamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				0,
				0
			);
			m_GPUDescriptorHandleVec.push_back(SamplerHandle);
			CD3DX12_GPU_DESCRIPTOR_HANDLE CbvHandle(
				m_DescriptorHeap->GetGPUDescriptorHandleForHeapStart(),
				1,
				m_DescriptorHandleIncrementSize
			);
			m_GPUDescriptorHandleVec.push_back(CbvHandle);

		}
		//�T���v���[
		{
			auto SamplerDescriptorHandle = m_SamplerDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			DynamicSampler::CreateSampler(SamplerState::LinearClamp, SamplerDescriptorHandle);
		}
		//�R���X�^���g�o�b�t�@
		{
			//�R���X�^���g�o�b�t�@��256�o�C�g�ɃA���C�������g
			UINT ConstBuffSize = (sizeof(m_SimpleConstants.m_ConstantBuffer) + 255) & ~255;
			//�R���X�^���g�o�b�t�@���\�[�X�i�A�b�v���[�h�q�[�v�j�̍쐬
			ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(ConstBuffSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&m_SimpleConstants.m_ConstantBufferUploadHeap)),
				L"�R���X�^���g�o�b�t�@�p�̃A�b�v���[�h�q�[�v�쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommittedResource()",
				L"PNTStaticDraw::CreateNotShadow()"
			);
			//�R���X�^���g�o�b�t�@�̃r���[���쐬
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = m_SimpleConstants.m_ConstantBufferUploadHeap->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = ConstBuffSize;
			//�R���X�^���g�o�b�t�@�r���[���쐬���ׂ��f�X�N�v���^�q�[�v��̃n���h�����擾
			//�V�F�[�_���\�[�X������ꍇ�R���X�^���g�o�b�t�@�̓V�F�[�_���\�[�X�r���[�̂��Ƃɐݒu����
			CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(
				m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				1,
				m_DescriptorHandleIncrementSize
			);

			Dev->GetDevice()->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			//�R���X�^���g�o�b�t�@�̃A�b�v���[�h�q�[�v�̃}�b�v
			CD3DX12_RANGE readRange(0, 0);
			ThrowIfFailed(m_SimpleConstants.m_ConstantBufferUploadHeap->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffer)),
				L"�R���X�^���g�o�b�t�@�̃}�b�v�Ɏ��s���܂���",
				L"pImpl->m_ConstantBufferUploadHeap->Map()",
				L"PNTStaticDraw::CreateNotShadow()"
			);

		}
		//�V�F�[�_���\�[�X�r���[�̓e�N�X�`���Z�b�g���ɍ쐬
		//�p�C�v���C���X�e�[�g�̍쐬
		{
			PipelineState::CreateDefault3D<VertexPositionNormalTexture, VSPNTStatic, PSPNTStatic>(m_RootSignature, m_PineLineDesc);
			m_PineLineDesc.RasterizerState.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
			m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
			m_CullFrontPipelineState = PipelineState::CreateDirect(m_PineLineDesc);
			m_PineLineDesc.RasterizerState.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
			m_CullBackPipelineState = PipelineState::CreateDirect(m_PineLineDesc);
		}
		//�R�}���h���X�g�͗��ʃJ�����O�ɏ�����
		{
			m_CommandList = CommandList::CreateDefault(m_CullBackPipelineState);
			//�R���X�^���g�o�b�t�@�X�V
			UpdateConstantBuffer();
			CommandList::Close(m_CommandList);
		}
	}





	//--------------------------------------------------------------------------------------
	///	PNTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{
	}

	PNTStaticDraw::~PNTStaticDraw() {}



	void PNTStaticDraw::OnCreate() {
		//���C�e�B���O�݂̂��Ƌɒ[�ɂȂ�̂Œ���
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
		//PNT�e�����ɏ�����
		CreatePNTNotShadow();
	}

	void PNTStaticDraw::OnDraw() {
		DrawPNTNotShadow();
	}

}
//end basecross
