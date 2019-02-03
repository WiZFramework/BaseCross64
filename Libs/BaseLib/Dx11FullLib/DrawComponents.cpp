/*!
@file DrawComponents.cpp
@brief �`��R���|�[�l���g����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"
#include "DrawComponents.h"

namespace basecross {

	//Shadowmap
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBShadow)
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmap, App::GetApp()->GetShadersPath() + L"VSShadowmap.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmapBone, App::GetApp()->GetShadersPath() + L"VSShadowmapBone.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSShadowmapBoneWithTan, App::GetApp()->GetShadersPath() + L"VSShadowmapBoneWithTan.cso")

	//�X�v���C�g�p
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBSprite)

	//PCSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCSprite, App::GetApp()->GetShadersPath() + L"VSPCSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCSprite, App::GetApp()->GetShadersPath() + L"PSPCSprite.cso")

	//PTSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTSprite, App::GetApp()->GetShadersPath() + L"VSPTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPTSprite, App::GetApp()->GetShadersPath() + L"PSPTSprite.cso")

	//PCTSprite
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTSprite, App::GetApp()->GetShadersPath() + L"VSPCTSprite.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTSprite, App::GetApp()->GetShadersPath() + L"PSPCTSprite.cso")


	//CBSimple
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBSimple)
	//PCStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCStatic, App::GetApp()->GetShadersPath() + L"VSPCStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCStatic, App::GetApp()->GetShadersPath() + L"PSPCStatic.cso")
	//PNStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNStatic, App::GetApp()->GetShadersPath() + L"VSPNStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNStatic, App::GetApp()->GetShadersPath() + L"PSPNStatic.cso")
	//PTStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTStatic, App::GetApp()->GetShadersPath() + L"VSPTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPTStatic, App::GetApp()->GetShadersPath() + L"PSPTStatic.cso")
	//PCTStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTStatic, App::GetApp()->GetShadersPath() + L"VSPCTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCTStatic, App::GetApp()->GetShadersPath() + L"PSPCTStatic.cso")
	//PNTStatic
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStatic, App::GetApp()->GetShadersPath() + L"VSPNTStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStatic, App::GetApp()->GetShadersPath() + L"PSPNTStatic.cso")
	//PNTStaticShadow
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticShadow, App::GetApp()->GetShadersPath() + L"VSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow, App::GetApp()->GetShadersPath() + L"PSPNTStaticShadow.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNTStaticShadow2, App::GetApp()->GetShadersPath() + L"PSPNTStaticShadow2.cso")
	//PNTBone
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTBone, App::GetApp()->GetShadersPath() + L"VSPNTBone.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTBoneShadow, App::GetApp()->GetShadersPath() + L"VSPNTBoneShadow.cso")

	//PCStaticInstance
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCStaticInstance, App::GetApp()->GetShadersPath() + L"VSPCStaticInstance.cso")
	//PTStaticInstance
	IMPLEMENT_DX11_VERTEX_SHADER(VSPTStaticInstance, App::GetApp()->GetShadersPath() + L"VSPTStaticInstance.cso")
	//PCTStaticInstance
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCTStaticInstance, App::GetApp()->GetShadersPath() + L"VSPCTStaticInstance.cso")
	//PNTStaticInstance
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticInstance, App::GetApp()->GetShadersPath() + L"VSPNTStaticInstance.cso")
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNTStaticInstanceShadow, App::GetApp()->GetShadersPath() + L"VSPNTStaticInstanceShadow.cso")



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
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
		switch (GetBlendState()) {
		case BlendState::Opaque:
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
			break;
		case BlendState::AlphaBlend:
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
			break;
		case BlendState::Additive:
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAdditive(), nullptr, 0xffffffff);
			break;
		case BlendState::NonPremultiplied:
			pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetNonPremultiplied(), nullptr, 0xffffffff);
			break;
		}
	}


	void DrawComponent::SetDepthStencilState(const DepthStencilState state) {
		pImpl->m_DepthStencilState = state;

	}

	void DrawComponent::SetDeviceDepthStencilState() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
		switch (GetDepthStencilState()) {
		case DepthStencilState::None:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
			break;
		case DepthStencilState::Default:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			break;
		case DepthStencilState::Read:
			pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthRead(), 0);
			break;
		}

	}

	void DrawComponent::SetRasterizerState(const RasterizerState state) {
		pImpl->m_RasterizerState = state;
	}
	void DrawComponent::SetDeviceRasterizerState() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
		switch (GetRasterizerState()) {
		case RasterizerState::CullBack:
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
			break;
		case RasterizerState::CullFront:
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
			break;
		case RasterizerState::CullNone:
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
			break;
		}
	}


	void DrawComponent::SetSamplerState(const SamplerState state) {
		pImpl->m_SamplerState = state;
	}
	void DrawComponent::SetDeviceSamplerState() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pSampler = nullptr;
		switch (GetSamplerState()) {
		case SamplerState::SamplerNone:
			//�T���v���[���N���A
			pID3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
			return;
			break;
		case SamplerState::PointWrap:
			pSampler = RenderStatePtr->GetPointWrap();
			break;
		case SamplerState::PointClamp:
			pSampler = RenderStatePtr->GetPointClamp();
			break;
		case SamplerState::LinearWrap:
			pSampler = RenderStatePtr->GetLinearWrap();
			break;
		case SamplerState::LinearClamp:
			pSampler = RenderStatePtr->GetLinearClamp();
			break;
		case SamplerState::AnisotropicWrap:
			pSampler = RenderStatePtr->GetAnisotropicWrap();
			break;
		case SamplerState::AnisotropicClamp:
			pSampler = RenderStatePtr->GetAnisotropicClamp();
			break;
		case SamplerState::ComparisonLinear:
			pSampler = RenderStatePtr->GetComparisonLinear();
			break;
		}
		//�T���v���[��ݒ�
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
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

		static float m_PosAdjustment;

		//���b�V�����\�[�X
		weak_ptr<MeshResource> m_MeshResource;
		//�}���`���b�V�����\�[�X
		weak_ptr<MultiMeshResource> m_MultiMeshResource;
		//�}���`���b�V���p�̕`�悷��C���f�b�N�X
		vector<bool> m_MultiMeshDrawVec;


		Impl()
		{}
		~Impl() {}
	};

	float Shadowmap::Impl::m_LightHeight(200.0f);
	float Shadowmap::Impl::m_LightNear(1.0f);
	float Shadowmap::Impl::m_LightFar(220.0f);
	float Shadowmap::Impl::m_ViewWidth(32.0f);
	float Shadowmap::Impl::m_ViewHeight(32.0f);
	float Shadowmap::Impl::m_PosAdjustment(0.1f);



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
	float Shadowmap::GetPosAdjustment() { return Impl::m_PosAdjustment; }

	void Shadowmap::SetLightHeight(float f) { Impl::m_LightHeight = f; }
	void Shadowmap::SetLightNear(float f) { Impl::m_LightNear = f; }
	void Shadowmap::SetLightFar(float f) { Impl::m_LightFar = f; }
	void Shadowmap::SetViewWidth(float f) { Impl::m_ViewWidth = f; }
	void Shadowmap::SetViewHeight(float f) { Impl::m_ViewHeight = f; }
	void Shadowmap::SetViewSize(float f) { Impl::m_ViewWidth = Impl::m_ViewHeight = f; }
	void Shadowmap::SetPosAdjustment(float f) { Impl::m_PosAdjustment = f; }



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

	void Shadowmap::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr) {
		pImpl->m_MultiMeshResource = MeshResourcePtr;
		pImpl->m_MultiMeshDrawVec.clear();
		for (size_t i = 0; i < MeshResourcePtr->GetMeshVecCount(); i++) {
			//�ŏ��͂��ׂẴ}���`���b�V����`��
			pImpl->m_MultiMeshDrawVec.push_back(true);
		}
	}

	void Shadowmap::SetMultiMeshResource(const wstring& ResKey) {
		this->SetMultiMeshResource(App::GetApp()->GetResource<MultiMeshResource>(ResKey));
	}

	bool Shadowmap::GetMultiMeshIsDraw(size_t index) const {
		if (index >= pImpl->m_MultiMeshDrawVec.size()) {
			throw BaseException(
				L"�C���f�b�N�X���}���`���b�V���̃��b�V�����𒴂��Ă܂�",
				L"if (index >= pImpl->m_MultiMeshDrawVec.size())",
				L"Shadowmap::GetMultiMeshIsDraw()"
			);
		}
		return pImpl->m_MultiMeshDrawVec[index];
	}
	void Shadowmap::SetMultiMeshIsDraw(size_t index, bool b) {
		if (index >= pImpl->m_MultiMeshDrawVec.size()) {
			throw BaseException(
				L"�C���f�b�N�X���}���`���b�V���̃��b�V�����𒴂��Ă܂�",
				L"if (index >= pImpl->m_MultiMeshDrawVec.size())",
				L"Shadowmap::SetMultiMeshIsDraw()"
			);
		}
		pImpl->m_MultiMeshDrawVec[index] = b;
	}


	void Shadowmap::DrawSigle(const MeshPrimData& data) {
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage) {
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�^�X�̃|�C���^
		auto RenderStatePtr = Dev->GetRenderState();

		//�s��̒�`
		bsm::Mat4x4 World, LightView, LightProj;
		//�s��̒�`
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();

		//���C�g�̎擾
		auto StageLight = PtrGameObject->OnGetDrawLight();
		//�ʒu�̎擾
		auto Pos = PtrTrans->GetWorldMatrix().transInMatrix();
		bsm::Vec3 PosSpan = StageLight.m_Directional;
		PosSpan *= pImpl->m_PosAdjustment;
		Pos += PosSpan;
		//���[���h�s��̌���
		World.affineTransformation(
			PtrTrans->GetScale(),			//�X�P�[�����O
			PtrTrans->GetPivot(),		//��]�̒��S�i�d�S�j
			PtrTrans->GetQuaternion(),				//��]�p�x
			Pos				//�ʒu
		);
		bsm::Mat4x4 RealWorldMatrix;
		//���[���h�s��̌���
		if (data.m_UseMeshToTransformMatrix) {
			RealWorldMatrix = data.m_MeshToTransformMatrix * GetMeshToTransformMatrix();
			RealWorldMatrix *= World;
		}
		else {
			RealWorldMatrix = GetMeshToTransformMatrix() * World;
		}
		//�r���[�s��̌���
		auto StageView = PtrStage->GetView();
		bsm::Vec3 LightDir = -1.0 * StageLight.m_Directional;
		bsm::Vec3 LightAt = StageView->GetTargetCamera()->GetAt();
		bsm::Vec3 LightEye = LightAt + (LightDir * GetLightHeight());
		//���C�g�̃r���[�Ǝˉe���v�Z
		LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
		LightProj = XMMatrixOrthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());

		ShadowConstants Cb;
		Cb.mWorld = bsm::transpose(RealWorldMatrix);
		Cb.mView = bsm::transpose(LightView);
		Cb.mProj = bsm::transpose(LightProj);

		bool IsSkin = false;
		bool IsSkinStride = false;
		auto MeshPtr = pImpl->m_MeshResource.lock();
		bool MeshFlg = MeshPtr && MeshPtr->IsSkining();
		auto MultiMeshPtr = pImpl->m_MultiMeshResource.lock();
		bool MultiMeshFlg = MultiMeshPtr && MultiMeshPtr->IsSkining(0);
		if (MeshFlg) {
			auto DrawCompPtr = GetGameObject()->GetDynamicComponent<DrawComponent>(false);
			if (auto* pLocalBoneVec = DrawCompPtr->GetVecLocalBonesPtr()) {
				if (pLocalBoneVec) {
					//�{�[���̐ݒ�
					size_t BoneSz = pLocalBoneVec->size();
					UINT cb_count = 0;
					for (size_t b = 0; b < BoneSz; b++) {
						bsm::Mat4x4 mat = pLocalBoneVec->at(b);
						mat.transpose();
						Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
						Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
						Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
						cb_count += 3;
					}
					IsSkin = true;
				}
			}
			IsSkinStride = true;
		}
		else if (MultiMeshFlg) {
			auto DrawCompPtr = GetGameObject()->GetDynamicComponent<DrawComponent>(false);
			if (auto* pLocalBoneVec = DrawCompPtr->GetVecMultiLocalBonesPtr(data.m_MultiMeshIndex)) {
				if (pLocalBoneVec) {
					//�{�[���̐ݒ�
					size_t BoneSz = pLocalBoneVec->size();
					UINT cb_count = 0;
					for (size_t b = 0; b < BoneSz; b++) {
						bsm::Mat4x4 mat = pLocalBoneVec->at(b);
						mat.transpose();
						Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
						Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
						Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
						cb_count += 3;
					}
					IsSkin = true;
				}
			}
			IsSkinStride = true;
		}
		//������`�揈��
		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(CBShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);

		if (IsSkin) {
			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = data.m_NumStride;
			UINT offset = 0;
			if (stride == sizeof(VertexPositionNormalTangentTextureSkinning)) {
				//���_�V�F�[�_�[�̃Z�b�g
				pID3D11DeviceContext->VSSetShader(VSShadowmapBoneWithTan::GetPtr()->GetShader(), nullptr, 0);
				//�C���v�b�g���C�A�E�g�̃Z�b�g
				pID3D11DeviceContext->IASetInputLayout(VSShadowmapBoneWithTan::GetPtr()->GetInputLayout());
			}
			else {
				//���_�V�F�[�_�[�̃Z�b�g
				pID3D11DeviceContext->VSSetShader(VSShadowmapBone::GetPtr()->GetShader(), nullptr, 0);
				//�C���v�b�g���C�A�E�g�̃Z�b�g
				pID3D11DeviceContext->IASetInputLayout(VSShadowmapBone::GetPtr()->GetInputLayout());
			}
			//���_�o�b�t�@���Z�b�g
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
		}
		else {
			//�C���v�b�g���C�A�E�g�̃Z�b�g
			pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = data.m_NumStride;
			UINT offset = 0;
			//���_�o�b�t�@���Z�b�g
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, data.m_VertexBuffer.GetAddressOf(), &stride, &offset);
			//���_�V�F�[�_�[�̃Z�b�g
			pID3D11DeviceContext->VSSetShader(VSShadowmap::GetPtr()->GetShader(), nullptr, 0);
		}
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(data.m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�s�N�Z���V�F�[�_�̓Z�b�g���Ȃ��I
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		//�W�I���g���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
		pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBShadow::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�R���X�^���g�o�b�t�@���s�N�Z���V�F�[�_�ɃZ�b�g
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
		//�`��
		pID3D11DeviceContext->DrawIndexed(data.m_NumIndicis, 0, 0);
		//��n���͌Ăяo�����ōs��
//		Dev->InitializeStates();
	}


	void Shadowmap::OnDraw() {
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage) {
			return;
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource(false);
		if (PtrMeshResource) {
			DrawSigle(PtrMeshResource->GetMashData());
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource(false);
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					DrawSigle(vec[i]);
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
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
			m_Diffuse(1,1,1,1)

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
		ComPtr<ID3D11Buffer> m_MatrixBuffer;	///<�s��p�̒��_�o�b�t�@
		//���Z�������邩�ǂ���
		bool m_Addtype;
		Impl(size_t MaxInstance,bool AddType) :
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
			MeshResource::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);

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
		const shared_ptr<TextureResource>& TextureRes,const bsm::Col4& Diffuse) {
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
		UINT Start;
		UINT Count;
		shared_ptr<TextureResource> Tex;
		bsm::Col4 m_Diffuse;
		InstanceDrawStr(UINT s, UINT c, shared_ptr<TextureResource> t,
			const bsm::Col4& Diffuse) :
			Start(s), Count(c), Tex(t), m_Diffuse(Diffuse){}
	};

	void PCTParticleDraw::OnDraw() {
		auto PtrStage = GetGameObject()->GetStage();
		if (!PtrStage) {
			return;
		}
		if (pImpl->m_DrawParticleSpriteVec.size() <= 0) {
			return;
		}

		//�J�����ʒu�Ń\�[�g
		auto func = [](DrawParticleSprite& Left, DrawParticleSprite& Right)->bool {
			return (Left.m_ToCaneraLength > Right.m_ToCaneraLength);
		};
		std::sort(pImpl->m_DrawParticleSpriteVec.begin(), pImpl->m_DrawParticleSpriteVec.end(), func);
		//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//�擪�̃e�N�X�`���𓾂�
		auto  NowTexPtr = pImpl->m_DrawParticleSpriteVec[0].m_TextureRes;

		vector<InstanceDrawStr> m_InstancVec;
		UINT NowStartIndex = 0;
		UINT NowDrawCount = 0;

		shared_ptr<TextureResource> NowTexRes = pImpl->m_DrawParticleSpriteVec[0].m_TextureRes;
		bsm::Col4 NowDiffuse  = bsm::Col4(1,1,1,1);
		for (size_t i = 0; i < pImpl->m_DrawParticleSpriteVec.size(); i++) {
			if (pImpl->m_DrawParticleSpriteVec[i].m_TextureRes != NowTexRes || 
				pImpl->m_DrawParticleSpriteVec[i].m_Diffuse != NowDiffuse) {
				m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes, NowDiffuse));
				NowStartIndex = (UINT)i;
				NowDrawCount = 0;
				NowTexRes = pImpl->m_DrawParticleSpriteVec[i].m_TextureRes;
				NowDiffuse = pImpl->m_DrawParticleSpriteVec[i].m_Diffuse;
			}
			else {
				NowDrawCount++;
			}
		}
		m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes, NowDiffuse));


		//�C���X�^���X�o�b�t�@�Ƀ}�b�v
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//�s��̃}�b�v
		if (FAILED(pD3D11DeviceContext->Map(pImpl->m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map���s
			throw BaseException(
				L"�s���Map�Ɏ��s���܂����B",
				L"if(FAILED(pD3D11DeviceContext->Map()))",
				L"PCTParticleDraw::OnDraw()"
			);
		}
		//�s��̕ύX
		auto* matrices = (bsm::Mat4x4*)mappedBuffer.pData;
		bsm::Mat4x4 World;
		for (size_t i = 0; i < pImpl->m_DrawParticleSpriteVec.size(); i++) {
			World = pImpl->m_DrawParticleSpriteVec[i].m_WorldMatrix;
			//�]�u����
			World.transpose();
			matrices[i] = World;
		}
		//�A���}�b�v
		pD3D11DeviceContext->Unmap(pImpl->m_MatrixBuffer.Get(), 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		//�`��̒��_�o�b�t�@�ƍs��o�b�t�@��ݒ�
		UINT stride[2] = { sizeof(VertexPositionColorTexture), sizeof(bsm::Mat4x4) };
		UINT offset[2] = { 0, 0 };

		ID3D11Buffer* pBuf[2] = { pImpl->m_MeshResource->GetVertexBuffer().Get(), pImpl->m_MatrixBuffer.Get() };
		pD3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(pImpl->m_MeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPCTStaticInstance::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPCTStaticInstance::GetPtr()->GetInputLayout());

		//�u�����h�X�e�[�g
		if (pImpl->m_Addtype) {
			//���Z����
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
		}
		else {
			//��������
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthRead(), 0);
		//�e�N�X�`���ƃT���v���[�̐ݒ�
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//�r���[�s��̌���
		bsm::Mat4x4 View, Proj;
		//�r���[�s��̌���
		auto StageView = PtrStage->GetView();
		View = StageView->GetTargetCamera()->GetViewMatrix();
		//�]�u����
		View.transpose();
		//�ˉe�s��̌���
		Proj = StageView->GetTargetCamera()->GetProjMatrix();
		//�]�u����
		Proj.transpose();


		//�R���X�^���g�o�b�t�@�̏���
		SimpleConstants sb;
		sb.World = bsm::Mat4x4();	//���[���h�s��̓_�~�[
		sb.View = View;
		sb.Projection = Proj;
		//�G�~�b�V�u���Z�͍s��Ȃ��B
		sb.Emissive = bsm::Col4(0, 0, 0, 0);
		sb.ActiveFlg.x = 1;

		for (auto& v : m_InstancVec) {
			sb.Diffuse = v.m_Diffuse;
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.Tex->GetShaderResourceView().GetAddressOf());
			//�`��
			pD3D11DeviceContext->DrawIndexedInstanced(pImpl->m_MeshResource->GetNumIndicis(), (UINT)v.Count, 0, 0, (UINT)v.Start);
		}
		//��n��
		Dev->InitializeStates();
		pImpl->m_DrawParticleSpriteVec.clear();
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

	SpriteBaseDraw::SpriteBaseDraw(const shared_ptr<GameObject>& GameObjectPtr):
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
	PCSpriteDraw::PCSpriteDraw(const shared_ptr<GameObject>& GameObjectPtr):
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

	PCSpriteDraw::~PCSpriteDraw(){}

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
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::CullNone);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//�V�F�[�_�̐ݒ�
			DrawSprite<VSPCSprite, PSPCSprite>(PtrMeshResource->GetMashData());
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
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
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::CullNone);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//�V�F�[�_�̐ݒ�
			DrawSprite<VSPTSprite, PSPTSprite>(PtrMeshResource->GetMashData());
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

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
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::CullNone);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//�V�F�[�_�̐ݒ�
			DrawSprite<VSPCTSprite, PSPCTSprite>(PtrMeshResource->GetMashData());
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}


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
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//�s��̒�`
		auto PtrTrans = GameObjectPtr->GetComponent<Transform>();
		//�s��̒�`
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//���[���h�s��̌���
		World = MeshToTransformMatrix * PtrTrans->GetWorldMatrix();
		//�]�u����
		World.transpose();
		//�J�����𓾂�
		auto CameraPtr = GameObjectPtr->OnGetDrawCamera();
		//�r���[�Ǝˉe�s��𓾂�
		ViewMat = CameraPtr->GetViewMatrix();
		//�]�u����
		ViewMat.transpose();
		//�]�u����
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		//�R���X�^���g�o�b�t�@�̏���
		SimpleConstants sb;
		sb.World = World;
		sb.View = ViewMat;
		sb.Projection = ProjMat;
		//�G�~�b�V�u
		sb.Emissive = Emissive;
		//�f�t�B�[�Y�͂��ׂĒʂ�
		sb.Diffuse = Diffuse;
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//���_�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pD3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//���_�V�F�[�_�ɓn��
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�s�N�Z���V�F�[�_�ɓn��
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPCStatic::GetPtr()->GetInputLayout());
		//�u�����h�X�e�[�g
		//�����������Ȃ�
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//���X�^���C�U�X�e�[�g(���C�A�t���[��)
		pD3D11DeviceContext->RSSetState(RenderState->GetWireframe());
		pD3D11DeviceContext->DrawIndexed(MeshRes->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates();
	}


	void GenericDraw::DrawWireFrame(const shared_ptr<GameObject>& GameObj,
		const shared_ptr<MeshResource>& Mesh, const bsm::Mat4x4& MeshToTransformMatrix) {

		PCWireFrameDraw(GameObj, Mesh,
			bsm::Col4(0,0,0,0), bsm::Col4(1, 1, 1, 1), MeshToTransformMatrix);
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
		/// �s��p�̒��_�o�b�t�@
		ComPtr<ID3D11Buffer> m_MatrixBuffer;
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

	void SmBaseDraw::SetConstants(SimpleConstants& SmCb, const MeshPrimData& data) {
		//�s��̒�`
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		//�s��̒�`
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//���[���h�s��̌���
		if (data.m_UseMeshToTransformMatrix) {
			World = data.m_MeshToTransformMatrix * GetMeshToTransformMatrix();
			World *= PtrTrans->GetWorldMatrix();
		}
		else {
			World = GetMeshToTransformMatrix() * PtrTrans->GetWorldMatrix();
		}
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
		SmCb.World = World;
		SmCb.View = ViewMat;
		SmCb.Projection = ProjMat;
		//�G�~�b�V�u
		SmCb.Emissive = GetEmissive();
		//�f�t�B�[�Y
		SmCb.Diffuse = GetDiffuse();
		//�X�y�L�����[
		SmCb.Specular = GetSpecular();
		//���C�e�B���O
		auto StageLight = GetGameObject()->OnGetDrawLight();
		SmCb.LightDir = StageLight.m_Directional;
		SmCb.LightDir.w = 1.0f;
		SmCb.EyePos = CameraPtr->GetEye();
		SmCb.EyePos.w = 1.0f;
		//�e�p
		if (GetOwnShadowActive()) {
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
		}
		//�{�[���̐ݒ�
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
	}

	//�s��o�b�t�@�̍쐬
	void SmBaseDraw::CreateMatrixBuffer() {
		//�C���X�^���X�s��o�b�t�@�̍쐬
		//Max�l�ō쐬����
		vector<bsm::Mat4x4> matrices(pImpl->m_SmDrawObject.m_MaxInstance);
		for (auto& m : matrices) {
			m = bsm::Mat4x4();
		}
		MeshResource::CreateDynamicVertexBuffer(pImpl->m_SmDrawObject.m_MatrixBuffer, matrices);
	}

	//�s��o�b�t�@�̃}�b�v
	void SmBaseDraw::MapMatrixBuffer() {
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
		pImpl->m_SmDrawObject.m_MultiMeshDrawVec.clear();
		for (size_t i = 0; i < MeshResourcePtr->GetMeshVecCount(); i++) {
			//�ŏ��͂��ׂẴ}���`���b�V����`��
			pImpl->m_SmDrawObject.m_MultiMeshDrawVec.push_back(true);
		}

	}

	void SmBaseDraw::SetMultiMeshResource(const wstring& ResKey) {
		this->SetMultiMeshResource(App::GetApp()->GetResource<MultiMeshResource>(ResKey));

	}

	bool SmBaseDraw::GetMultiMeshIsDraw(size_t index) const {
		if (index >= pImpl->m_SmDrawObject.m_MultiMeshDrawVec.size()) {
			throw BaseException(
				L"�C���f�b�N�X���}���`���b�V���̃��b�V�����𒴂��Ă܂�",
				L"if (index >= pImpl->m_BcDrawObject.m_MultiMeshDrawVec.size())",
				L"SmBaseDraw::GetMultiMeshIsDraw()"
			);
		}
		return pImpl->m_SmDrawObject.m_MultiMeshDrawVec[index];
	}
	void SmBaseDraw::SetMultiMeshIsDraw(size_t index, bool b) {
		if (index >= pImpl->m_SmDrawObject.m_MultiMeshDrawVec.size()) {
			throw BaseException(
				L"�C���f�b�N�X���}���`���b�V���̃��b�V�����𒴂��Ă܂�",
				L"if (index >= pImpl->m_BcDrawObject.m_MultiMeshDrawVec.size())",
				L"SmBaseDraw::SetMultiMeshIsDraw()"
			);
		}
		pImpl->m_SmDrawObject.m_MultiMeshDrawVec[index] = b;
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

	ComPtr<ID3D11Buffer>& SmBaseDraw::GetMatrixBuffer() const {
		return pImpl->m_SmDrawObject.m_MatrixBuffer;
	}

	void SmBaseDraw::GetStaticMeshLocalPositions(vector<bsm::Vec3>& vertices) {
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

	bool SmBaseDraw::HitTestStaticMeshSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp, bsm::Vec3& HitPoint, TRIANGLE& RetTri) {
		GetStaticMeshWorldPositions(pImpl->m_SmDrawObject.m_TempPositions);
		for (size_t i = 0; i < pImpl->m_SmDrawObject.m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_SmDrawObject.m_TempPositions[i];
			tri.m_B = pImpl->m_SmDrawObject.m_TempPositions[i + 1];
			tri.m_C = pImpl->m_SmDrawObject.m_TempPositions[i + 2];
			bsm::Vec3 ret;
			float t;
			//���̂̈ړ���StartSp����EndSp��1.0�Ƃ���
			bsm::Vec3 Valocity = EndSp.m_Center - StartSp.m_Center;
			if (HitTest::CollisionTestSphereTriangle(StartSp, Valocity, tri, 0.0f, 1.0f, t)) {
				auto Len = length(EndSp.m_Center - StartSp.m_Center);
				Len *= t;
				auto Nomal = EndSp.m_Center - StartSp.m_Center;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartSp.m_Center + Nomal;
				RetTri = tri;
				return true;
			}
		}
		return false;
	}

	void SmBaseDraw::GetSkinedMeshLocalPositions(vector<bsm::Vec3>& vertices) {
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

	bool SmBaseDraw::HitTestSkinedMeshSphereTriangles(const SPHERE& StartSp, const SPHERE& EndSp,
		bsm::Vec3& HitPoint, TRIANGLE& RetTri) {
		GetSkinedMeshWorldPositions(pImpl->m_SmDrawObject.m_TempPositions);
		for (size_t i = 0; i < pImpl->m_SmDrawObject.m_TempPositions.size(); i += 3) {
			TRIANGLE tri;
			tri.m_A = pImpl->m_SmDrawObject.m_TempPositions[i];
			tri.m_B = pImpl->m_SmDrawObject.m_TempPositions[i + 1];
			tri.m_C = pImpl->m_SmDrawObject.m_TempPositions[i + 2];
			bsm::Vec3 ret;
			float t;
			//���̂̈ړ���StartSp����EndSp��1.0�Ƃ���
			bsm::Vec3 Valocity = EndSp.m_Center - StartSp.m_Center;
			if (HitTest::CollisionTestSphereTriangle(StartSp, Valocity, tri, 0.0f, 1.0f, t)) {
				auto Len = length(EndSp.m_Center - StartSp.m_Center);
				Len *= t;
				auto Nomal = EndSp.m_Center - StartSp.m_Center;
				Nomal.normalize();
				Nomal *= Len;
				HitPoint = StartSp.m_Center + Nomal;
				RetTri = tri;
				return true;
			}
		}
		return false;
	}


	//--------------------------------------------------------------------------------------
	///	PCStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCStaticDraw::PCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr):
		SmBaseDraw(GameObjectPtr)
	{}

	PCStaticDraw::~PCStaticDraw(){}

	void PCStaticDraw::OnCreate(){}

	void PCStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic<VSPCStatic, PSPCStatic>(PtrMeshResource->GetMashData());
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					DrawStatic<VSPCStatic, PSPCStatic>(vec[i]);
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}

	//--------------------------------------------------------------------------------------
	///	PNStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNStaticDraw::PNStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PNStaticDraw::~PNStaticDraw() {}

	void PNStaticDraw::OnCreate() {}

	void PNStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic<VSPNStatic, PSPNStatic>(PtrMeshResource->GetMashData());
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					DrawStatic<VSPNStatic, PSPNStatic>(vec[i]);
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}



	//--------------------------------------------------------------------------------------
	///	PTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PTStaticDraw::PTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PTStaticDraw::~PTStaticDraw() {}

	void PTStaticDraw::OnCreate() {}

	void PTStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic<VSPTStatic, PSPTStatic>(PtrMeshResource->GetMashData());
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					DrawStatic<VSPTStatic, PSPTStatic>(vec[i]);
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}

	//--------------------------------------------------------------------------------------
	///	PCTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCTStaticDraw::PCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PCTStaticDraw::~PCTStaticDraw() {}

	void PCTStaticDraw::OnCreate() {}

	void PCTStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStatic<VSPCTStatic, PSPCTStatic>(PtrMeshResource->GetMashData());
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					DrawStatic<VSPCTStatic, PSPCTStatic>(vec[i]);
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}

	//--------------------------------------------------------------------------------------
	///	PNTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNTStaticDraw::PNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PNTStaticDraw::~PNTStaticDraw() {}

	void PNTStaticDraw::OnCreate() {
		//���C�e�B���O�݂̂��Ƌɒ[�ɂȂ�̂Œ���
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void PNTStaticDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			if (GetOwnShadowActive()) {
				DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow>(PtrMeshResource->GetMashData());
			}
			else {
				DrawStatic<VSPNTStatic, PSPNTStatic>(PtrMeshResource->GetMashData());
			}
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					if (GetOwnShadowActive()) {
						if (GetGameObject()->GetComponent<Shadowmap>(false)) {
							DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow2>(vec[i]);
						}
						else {
							DrawStatic<VSPNTStaticShadow, PSPNTStaticShadow>(vec[i]);
						}
					}
					else {
						DrawStatic<VSPNTStatic, PSPNTStatic>(vec[i]);
					}
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}

	//--------------------------------------------------------------------------------------
	///	PNTStaticModel�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNTStaticModelDraw::PNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PNTStaticModelDraw::~PNTStaticModelDraw() {}

	void PNTStaticModelDraw::OnCreate() {
		//���C�e�B���O�݂̂��Ƌɒ[�ɂȂ�̂Œ���
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void PNTStaticModelDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			if (GetOwnShadowActive()) {
				if (GetGameObject()->GetComponent<Shadowmap>(false)) {
					DrawModel<VSPNTStaticShadow, PSPNTStaticShadow2>(PtrMeshResource->GetMashData());
				}
				else {
					DrawModel<VSPNTStaticShadow, PSPNTStaticShadow>(PtrMeshResource->GetMashData());
				}
			}
			else {
				DrawModel<VSPNTStatic, PSPNTStatic>(PtrMeshResource->GetMashData());
			}
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					if (GetOwnShadowActive()) {
						if (GetGameObject()->GetComponent<Shadowmap>(false)) {
							DrawModel<VSPNTStaticShadow, PSPNTStaticShadow2>(vec[i]);
						}
						else {
							DrawModel<VSPNTStaticShadow, PSPNTStaticShadow>(vec[i]);
						}
					}
					else {
						DrawModel<VSPNTStatic, PSPNTStatic>(vec[i]);
					}
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();

	}

	//--------------------------------------------------------------------------------------
	///	PNTBoneModel�`��R���|�[�l���g�i�{�[�����f���`��p�j
	//--------------------------------------------------------------------------------------
	PNTBoneModelDraw::PNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}
	PNTBoneModelDraw::~PNTBoneModelDraw() {}

	void PNTBoneModelDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshRes) {
		SmBaseDraw::SetMeshResource(MeshRes);
		BoneInit();
	}

	void PNTBoneModelDraw::SetMeshResource(const wstring& MeshKey) {
		PNTBoneModelDraw::SetMeshResource(App::GetApp()->GetResource<MeshResource>(MeshKey));
	}

	void PNTBoneModelDraw::SetMultiMeshResource(const shared_ptr<MultiMeshResource>& MeshResourcePtr) {
		SmBaseDraw::SetMultiMeshResource(MeshResourcePtr);
		BoneInit();

	}
	void PNTBoneModelDraw::SetMultiMeshResource(const wstring& ResKey) {
		PNTBoneModelDraw::SetMultiMeshResource(App::GetApp()->GetResource<MultiMeshResource>(ResKey));
	}



	void PNTBoneModelDraw::OnCreate() {
		//���C�e�B���O�݂̂��Ƌɒ[�ɂȂ�̂Œ���
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
	}

	void PNTBoneModelDraw::OnDraw() {
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			//�V�F�[�_�̐ݒ�
			if (IsOwnShadowActive()) {
				//�e�t��
				if (GetGameObject()->GetComponent<Shadowmap>(false)) {
					//�V���h�E�}�b�v������Ύ��ȉe�h�~�p�̃s�N�Z���V�F�[�_
					DrawModel<VSPNTBoneShadow, PSPNTStaticShadow2>(PtrMeshResource->GetMashData());
				}
				else {
					DrawModel<VSPNTBoneShadow, PSPNTStaticShadow>(PtrMeshResource->GetMashData());
				}
			}
			else {
				DrawModel<VSPNTBone, PSPNTStatic>(PtrMeshResource->GetMashData());
			}
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					if (GetOwnShadowActive()) {
						if (GetGameObject()->GetComponent<Shadowmap>(false)) {
							DrawModel<VSPNTBoneShadow, PSPNTStaticShadow2>(vec[i]);
						}
						else {
							DrawModel<VSPNTBoneShadow, PSPNTStaticShadow>(vec[i]);
						}
					}
					else {
						DrawModel<VSPNTBone, PSPNTStatic>(vec[i]);
					}
				}
			}
		}

		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	PCStaticInstance�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCStaticInstanceDraw::PCStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PCStaticInstanceDraw::~PCStaticInstanceDraw() {}

	void PCStaticInstanceDraw::OnCreate() {
		//�s��o�b�t�@�̍쐬
		CreateMatrixBuffer();
	}

	void PCStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStaticInstance<VSPCStaticInstance, PSPCStatic>(PtrMeshResource->GetMashData());
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					DrawStaticInstance<VSPCStaticInstance, PSPCStatic>(vec[i]);
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
		//�����s��N���A�Ȃ�
		if (IsAutoClearMatrixVec()) {
			//�s��z��̃N���A
			ClearMatrixVec();
		}
	}

	//--------------------------------------------------------------------------------------
	///	PTStaticInstance�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PTStaticInstanceDraw::PTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PTStaticInstanceDraw::~PTStaticInstanceDraw() {}

	void PTStaticInstanceDraw::OnCreate() {
		//�s��o�b�t�@�̍쐬
		CreateMatrixBuffer();
	}

	void PTStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStaticInstance<VSPTStaticInstance, PSPTStatic>(PtrMeshResource->GetMashData());
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					DrawStaticInstance<VSPTStaticInstance, PSPTStatic>(vec[i]);
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
		//�����s��N���A�Ȃ�
		if (IsAutoClearMatrixVec()) {
			//�s��z��̃N���A
			ClearMatrixVec();
		}
	}

	//--------------------------------------------------------------------------------------
	///	PCTStaticInstance�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PCTStaticInstanceDraw::PCTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PCTStaticInstanceDraw::~PCTStaticInstanceDraw() {}

	void PCTStaticInstanceDraw::OnCreate() {
		//�s��o�b�t�@�̍쐬
		CreateMatrixBuffer();
	}

	void PCTStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			DrawStaticInstance<VSPCTStaticInstance, PSPCTStatic>(PtrMeshResource->GetMashData());
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					DrawStaticInstance<VSPCTStaticInstance, PSPCTStatic>(vec[i]);
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
		//�����s��N���A�Ȃ�
		if (IsAutoClearMatrixVec()) {
			//�s��z��̃N���A
			ClearMatrixVec();
		}
	}

	//--------------------------------------------------------------------------------------
	///	PNTStaticInstance�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	PNTStaticInstanceDraw::PNTStaticInstanceDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		SmBaseDraw(GameObjectPtr)
	{}

	PNTStaticInstanceDraw::~PNTStaticInstanceDraw() {}

	void PNTStaticInstanceDraw::OnCreate() {
		//���C�e�B���O�݂̂��Ƌɒ[�ɂȂ�̂Œ���
		SetEmissive(bsm::Col4(0.5f, 0.5f, 0.5f, 0.0f));
		SetDiffuse(bsm::Col4(0.6f, 0.6f, 0.6f, 1.0f));
		//�s��o�b�t�@�̍쐬
		CreateMatrixBuffer();
	}

	void PNTStaticInstanceDraw::OnDraw() {
		MapMatrixBuffer();
		if (GetGameObject()->GetAlphaActive()) {
			if (!(GetBlendState() == BlendState::AlphaBlend || GetBlendState() == BlendState::Additive)) {
				SetBlendState(BlendState::AlphaBlend);
			}
			SetRasterizerState(RasterizerState::DoubleDraw);
		}
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetMeshResource();
		if (PtrMeshResource) {
			if (GetOwnShadowActive()) {
				if (GetGameObject()->GetComponent<Shadowmap>(false)) {
					DrawStaticInstance<VSPNTStaticInstanceShadow, PSPNTStaticShadow2>(PtrMeshResource->GetMashData());
				}
				else {
					DrawStaticInstance<VSPNTStaticInstanceShadow, PSPNTStaticShadow>(PtrMeshResource->GetMashData());
				}
			}
			else {
				DrawStaticInstance<VSPNTStaticInstance, PSPNTStatic>(PtrMeshResource->GetMashData());
			}
		}
		//�}���`���b�V�����\�[�X�̎擾
		auto PtrMultiMeshResource = GetMultiMeshResource();
		if (PtrMultiMeshResource) {
			size_t count = PtrMultiMeshResource->GetMeshVecCount();
			auto& vec = PtrMultiMeshResource->GetMeshVec();
			for (size_t i = 0; i < count; i++) {
				if (GetMultiMeshIsDraw(i)) {
					if (GetOwnShadowActive()) {
						if (GetGameObject()->GetComponent<Shadowmap>(false)) {
							DrawStaticInstance<VSPNTStaticInstanceShadow, PSPNTStaticShadow2>(vec[i]);
						}
						else {
							DrawStaticInstance<VSPNTStaticInstanceShadow, PSPNTStaticShadow>(vec[i]);
						}
					}
					else {
						DrawStaticInstance<VSPNTStaticInstance, PSPNTStatic>(vec[i]);
					}
				}
			}
		}
		//��n��
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->InitializeStates();
		//�����s��N���A�Ȃ�
		if (IsAutoClearMatrixVec()) {
			//�s��z��̃N���A
			ClearMatrixVec();
		}

	}


}
//end basecross

