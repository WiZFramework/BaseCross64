
/*!
@file DrawComponentsEx.h
@brief �g���`��R���|�[�l���g
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	Basic�V�F�[�_�[�p�R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	struct BasicConstants
	{
		bsm::Col4 diffuseColor;
		bsm::Col4 emissiveColor;
		bsm::Col4 specularColorAndPower;

		bsm::Col4 lightDirection[3];
		bsm::Col4 lightDiffuseColor[3];
		bsm::Col4 lightSpecularColor[3];

		bsm::Vec4 eyePosition;

		bsm::Col4 fogColor;
		bsm::Vec4 fogVector;

		bsm::Mat4x4 world;
		bsm::Vec4 worldInverseTranspose[3];
		bsm::Mat4x4 worldViewProj;
		//�ėp�t���O
		XMUINT4 activeFlg;
		//�ȉ��e
		bsm::Vec4 lightPos;
		bsm::Vec4 eyePos;
		bsm::Mat4x4 lightView;
		bsm::Mat4x4 lightProjection;

		bsm::Vec4 bones[3 * 72];

	};

	//--------------------------------------------------------------------------------------
	/// �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_CONSTANT_BUFFER(CBBasic, BasicConstants)


	//--------------------------------------------------------------------------------------
	/// PC���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_VERTEX_SHADER(BcVSPCStatic, VertexPositionColor)
	//--------------------------------------------------------------------------------------
	/// PT���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_VERTEX_SHADER(BcVSPTStatic, VertexPositionTexture)
	//--------------------------------------------------------------------------------------
	/// PCT���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_VERTEX_SHADER(BcVSPCTStatic, VertexPositionColorTexture)

	//--------------------------------------------------------------------------------------
	/// PC�p�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_PIXEL_SHADER(BcPSPCStatic);

	//--------------------------------------------------------------------------------------
	/// PT,PCT�p�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	DECLARE_DX11_PIXEL_SHADER(BcPSPTStatic);


	//--------------------------------------------------------------------------------------
	/// PNT���_�V�F�[�_(���_���C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticVL, VertexPositionNormalTexture)
	//�e����Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBoneVL, VertexPositionNormalTextureSkinning)

	//�e�t��
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticVLShadow, VertexPositionNormalTexture)
	//�e�t��Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBoneVLShadow, VertexPositionNormalTextureSkinning)


	
	//�o�C�A�X�t���e����
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticVLBn, VertexPositionNormalTexture)
	//�o�C�A�X�t���e����Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBoneVLBn, VertexPositionNormalTextureSkinning)

	//�o�C�A�X�t���e�t��
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticVLBnShadow, VertexPositionNormalTexture)
	//�o�C�A�X�t���e�t��Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBoneVLBnShadow, VertexPositionNormalTextureSkinning)


	//--------------------------------------------------------------------------------------
	/// PNT�s�N�Z���V�F�[�_(���_���C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTVL);
	//�e�t��
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTVLShadow);


	//--------------------------------------------------------------------------------------
	/// PNT���_�V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPL, VertexPositionNormalTexture)
	//�e����Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePL, VertexPositionNormalTextureSkinning)
	//�e�t��
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPLShadow, VertexPositionNormalTexture)
	//�e�t��Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePLShadow, VertexPositionNormalTextureSkinning)

	//�o�C�A�X�t���e����
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPLBn, VertexPositionNormalTexture)
	//�o�C�A�X�t���e����Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePLBn, VertexPositionNormalTextureSkinning)

	//�o�C�A�X�t���e�t��
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTStaticPLBnShadow, VertexPositionNormalTexture)
	//�o�C�A�X�t���e�t��Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTBonePLBnShadow, VertexPositionNormalTextureSkinning)

	//--------------------------------------------------------------------------------------
	/// PNT�s�N�Z���V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTPL);
	//�e�t��
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTPLShadow);


	//--------------------------------------------------------------------------------------
	/// PNTnT���_�V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTStaticPL, VertexPositionNormalTangentTexture)
	//�e����Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTBonePL, VertexPositionNormalTangentTextureSkinning)

	//�e��
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTStaticPLShadow, VertexPositionNormalTangentTexture)
	//�e��Bone
	DECLARE_DX11_VERTEX_SHADER(BcVSPNTnTBonePLShadow, VertexPositionNormalTangentTextureSkinning)


	//--------------------------------------------------------------------------------------
	/// PNTnT�s�N�Z���V�F�[�_(�s�N�Z�����C�e�B���O)
	//--------------------------------------------------------------------------------------
	//�e����
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTnTPL)
	//�e��
	DECLARE_DX11_PIXEL_SHADER(BcPSPNTnTPLShadow);

	//--------------------------------------------------------------------------------------
	///	Bc�`��I�u�W�F�N�g�̐e
	//--------------------------------------------------------------------------------------
	class BcBaseDraw : public DrawComponent{
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	StagePtr	�X�e�[�W�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		explicit BcBaseDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcBaseDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�^���g�o�b�t�@�̐ݒ�
		@param[out]	BcCb	�ݒ肷��R���X�^���g�o�b�t�@�\����
		@param[in]	data	���b�V���̃f�[�^
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetConstants(BasicConstants& BcCb, const MeshPrimData& data);
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
			//���C�g��ݒ�
			SetLightingParamaters();
			//�ʏ���
			BasicConstants BcCb;
			//�R���X�^���g�o�b�t�@�̍쐬
			SetConstants(BcCb, data);
			//�e�N�X�`��
			auto shTex = GetTextureResource();
			if (shTex) {
				//�e�N�X�`��������
				BcCb.activeFlg.y = 1;
			}
			else {
				//�`��R���|�[�l���g�ɂ̓e�N�X�`�����Ȃ�
				if (shTex = data.m_TextureResource.lock()) {
					//�e�N�X�`��������
					BcCb.activeFlg.y = 1;
				}
				else {
					BcCb.activeFlg.y = 0;
				}
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
				//�@���}�b�v
				if (GetNormalMapTextureResource()) {
					if (IsOwnShadowActive()) {
						//�Q�Ԗڂɓ����
						pD3D11DeviceContext->PSSetShaderResources(2, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
						//�T���v���[��ݒ�
						RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 2);
					}
					else {
						//1�Ԗڂɓ����
						pD3D11DeviceContext->PSSetShaderResources(1, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
						//�T���v���[��ݒ�
						RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 1);
					}
				}
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
			//��n��
//			Dev->InitializeStates();
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
			//���C�g��ݒ�
			SetLightingParamaters();
			BasicConstants BcCb;
			//�R���X�^���g�o�b�t�@�̍쐬
			SetConstants(BcCb, data);
			//���b�V�����̃}�e���A���̔z��
			auto& MatVec = data.m_MaterialExVec;
			size_t MatIndex = 0;
			for (auto& m : MatVec) {
				if (m.m_TextureResource) {
					//�e�N�X�`��������
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
					bsm::Col4 Em4 = m.m_Emissive;
					Em4.w = 0.0f;
					BcCb.emissiveColor = Em4;
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
				if (BcCb.activeFlg.y) {
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
							BcCb.activeFlg.y = 0;
						}
					}
					if (BcCb.activeFlg.y) {
						//�@���}�b�v
						if (GetNormalMapTextureResourceVec().size() > MatIndex) {
							//�����̖@���}�b�v������
							auto& normalVec = GetNormalMapTextureResourceVec();
							auto normalShptr = normalVec[MatIndex].lock();
							if (GetOwnShadowActive()) {
								//�Q�Ԗڂɓ����
								pD3D11DeviceContext->PSSetShaderResources(2, 1, normalShptr->GetShaderResourceView().GetAddressOf());
								//�T���v���[��ݒ�
								RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 2);
							}
							else {
								//1�Ԗڂɓ����
								pD3D11DeviceContext->PSSetShaderResources(1, 1, normalShptr->GetShaderResourceView().GetAddressOf());
								//�T���v���[��ݒ�
								RenderState->SetSamplerState(pD3D11DeviceContext,GetSamplerState(), 1);
							}
						}
						else if (GetNormalMapTextureResource()) {
							//�����ł͂Ȃ��Ĉ�����@���}�b�v������
							if (GetOwnShadowActive()) {
								//�Q�Ԗڂɓ����
								pD3D11DeviceContext->PSSetShaderResources(2, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
								//�T���v���[��ݒ�
								RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 2);
							}
							else {
								//1�Ԗڂɓ����
								pD3D11DeviceContext->PSSetShaderResources(1, 1, GetNormalMapTextureResource()->GetShaderResourceView().GetAddressOf());
								//�T���v���[��ݒ�
								RenderState->SetSamplerState(pD3D11DeviceContext, GetSamplerState(), 1);
							}
						}
					}
					else {
						//�V�F�[�_�[���\�[�X���N���A
						pD3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
						//�T���v���[���N���A
						RenderState->SetSamplerAllClear(pD3D11DeviceContext);
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
					L"BcStaticBaseDraw::UpdateVertices()"
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
		bsm::Col4 GetSpecularColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�y�L�����[�F�̐ݒ�
		@param[in]	col	�X�y�L�����[�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetSpecularColor(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�y�L�����[�F���Ȃ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void DisableSpecular();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�y�L�����[�p���[�̎擾
		@return	�X�y�L�����[�p���[�F
		*/
		//--------------------------------------------------------------------------------------
		float GetSpecularPower() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�y�L�����[�p���[�̐ݒ�
		@param[in]	pow	�X�y�L�����[�p���[�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetSpecularPower(float pow);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�y�L�����[�ƃp���[�F�̎擾
		@return	�X�y�L�����[�ƃp���[�F
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetSpecularColorAndPower() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�X�y�L�����[�ƃp���[�F�̐ݒ�
		@param[in]	col	�X�y�L�����[�ƃp���[�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetSpecularColorAndPower(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A���t�@�l�̎擾
		@return	�A���t�@�l
		*/
		//--------------------------------------------------------------------------------------
		float GetAlpha() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A���t�@�l�̐ݒ�
		@param[in]	pow	�A���t�@�l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetAlpha(float a);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J���[�ƃA���t�@�F�̎擾
		@return	�J���[�ƃA���t�@�F
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetColorAndAlpha() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�J���[�ƃA���t�@�F�̐ݒ�
		@param[in]	col �J���[�ƃA���t�@�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetColorAndAlpha(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�e�B���O���邩�ǂ���
		@return	����ꍇ��true
		*/
		//--------------------------------------------------------------------------------------
		bool IsLightingEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�e�B���O���邩�ǂ���
		@return	����ꍇ��true
		*/
		//--------------------------------------------------------------------------------------
		bool GetLightingEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�e�B���O���邩�ǂ����ݒ�
		@param[in] value ���C�e�B���O���邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetLightingEnabled(bool value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s�N�Z�����C�e�B���O���邩�ǂ���
		@return	����ꍇ��true
		*/
		//--------------------------------------------------------------------------------------
		bool GetPerPixelLighting() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s�N�Z�����C�e�B���O���邩�ǂ���
		@return	����ꍇ��true
		*/
		//--------------------------------------------------------------------------------------
		bool IsPerPixelLighting() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�s�N�Z�����C�e�B���O���邩�ǂ����ݒ�
		@param[in] value �s�N�Z�����C�e�B���O���邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetPerPixelLighting(bool value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�e�B���O�p�����[�^��ݒ肷��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetLightingParamaters();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�@���Ƀo�C�A�X�������邩�ǂ���
		@return	����ꍇ��true
		*/
		//--------------------------------------------------------------------------------------
		bool GetBiasedNormals() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�@���Ƀo�C�A�X�������邩�ǂ���
		@return	����ꍇ��true
		*/
		//--------------------------------------------------------------------------------------
		bool IsBiasedNormals() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�@���Ƀo�C�A�X�������邩�ǂ����ݒ�
		@param[in] value �@���Ƀo�C�A�X�������邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetBiasedNormals(bool value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A���r�G���g���C�g�F�̎擾
		@return	�A���r�G���g���C�g�F
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetAmbientLightColor()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�A���r�G���g���C�g�F�̐ݒ�
		@param[in] value �A���r�G���g���C�g�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetAmbientLightColor(const bsm::Col4& value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�C���f�b�N�X���L�����ǂ����`�F�b�N����i�����Ȃ��O�j
		@param[in] whichLight ���C�g�̃C���f�b�N�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		static void ValidateLightIndex(int whichLight);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	���C�g�̍ő�l�𓾂�
		@return	���C�g�̍ő�l
		*/
		//--------------------------------------------------------------------------------------
		int GetMaxDirectionalLights();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̃��C�g��L���ɂ��邩�ǂ����𓾂�
		@param[in] whichLight ���C�g�̃C���f�b�N�X
		@return	�L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetLightEnabled(int whichLight)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̃��C�g��L���ɂ��邩�ǂ����𓾂�
		@param[in] whichLight ���C�g�̃C���f�b�N�X
		@return	�L���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsLightEnabled(int whichLight)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̃��C�g��L���ɂ��邩�ǂ���
		@param[in] whichLight ���C�g�̃C���f�b�N�X
		@param[in] value �L���ɂ��邩�ǂ���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetLightEnabled(int whichLight, bool value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̃��C�g�̌����𓾂�
		@param[in] whichLight ���C�g�̃C���f�b�N�X
		@return	�w��̃��C�g�̌���
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3 GetLightDirection(int whichLight) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̃��C�g�̌�����ݒ肷��
		@param[in] whichLight ���C�g�̃C���f�b�N�X
		@param[in] value �l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetLightDirection(int whichLight, const bsm::Vec3& value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̃��C�g�̃f�t���[�Y�F�𓾂�
		@param[in] whichLight ���C�g�̃C���f�b�N�X
		@return	�w��̃��C�g�̃f�t���[�Y�F
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetLightDiffuseColor(int whichLight) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̃��C�g�̃f�t���[�Y�F��ݒ肷��
		@param[in] whichLight ���C�g�̃C���f�b�N�X
		@param[in] value �l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetLightDiffuseColor(int whichLight, const bsm::Col4& value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̃��C�g�̃X�y�L�����[�F�𓾂�
		@param[in] whichLight ���C�g�̃C���f�b�N�X
		@return	�w��̃��C�g�̃X�y�L�����[�F
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetLightSpecularColor(int whichLight) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�w��̃��C�g�̃X�y�L�����[�F��ݒ肷��
		@param[in] whichLight ���C�g�̃C���f�b�N�X
		@param[in] value �l
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetLightSpecularColor(int whichLight, const bsm::Col4& value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H�O���g�����ǂ���
		@return	�t�H�O���g���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool GetFogEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H�O���g�����ǂ���
		@return	�t�H�O���g���Ȃ�true
		*/
		//--------------------------------------------------------------------------------------
		bool IsFogEnabled() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H�O���g�����ǂ����w��
		@param[in]	b	�t�H�O���g���Ȃ�true
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetFogEnabled(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H�O�̊J�n�ʒu�𓾂�
		@return	�t�H�O�̊J�n�ʒu
		*/
		//--------------------------------------------------------------------------------------
		float GetFogStart() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H�O�̊J�n�ʒu�w��
		@param[in]	value	�t�H�O�̊J�n�ʒu
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetFogStart(float value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H�O�̏I���ʒu�𓾂�
		@return	�t�H�O�̏I���ʒu
		*/
		//--------------------------------------------------------------------------------------
		float GetFogEnd() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H�O�̏I���ʒu�w��
		@param[in]	value	�t�H�O�̏I���ʒu
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetFogEnd(float value);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H�O�F�̎擾
		@return	�t�H�O�F
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetFogColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�t�H�O�F�̐ݒ�
		@param[in]	col	�t�H�O�F
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetFogColor(const bsm::Col4& col);
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�@���}�b�v�e�N�X�`�����\�[�X�̎擾
		@return	�@���}�b�v�e�N�X�`�����\�[�X
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetNormalMapTextureResource() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�@���}�b�v�e�N�X�`�����\�[�X�̐ݒ�
		@param[in]	TextureRes	�e�N�X�`�����\�[�X
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetNormalMapTextureResource(const shared_ptr<TextureResource>& TextureRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�@���}�b�v�e�N�X�`�����\�[�X�̐ݒ�
		@param[in]	TextureKey	�e�N�X�`���L�[
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetNormalMapTextureResource(const wstring& TextureKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�@���}�b�v�e�N�X�`�����\�[�X�z��̎擾
		@return	�@���}�b�v�e�N�X�`�����\�[�X�z��
		*/
		//--------------------------------------------------------------------------------------
		const vector<weak_ptr<TextureResource>>& GetNormalMapTextureResourceVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�@���}�b�v�e�N�X�`�����\�[�X�z��̎擾
		@return	�@���}�b�v�e�N�X�`�����\�[�X�z��
		*/
		//--------------------------------------------------------------------------------------
		vector<weak_ptr<TextureResource>>& GetNormalMapTextureResourceVec();
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
		bool HitTestStaticMeshSegmentTriangles(const bsm::Vec3& StartPos, const bsm::Vec3& EndPos, bsm::Vec3& HitPoint,TRIANGLE& RetTri);
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
	///BcPCStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class BcPCStaticDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPCStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPCStaticDraw();
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
	///	BasicPTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class BcPTStaticDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPTStaticDraw();
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
	///	BasicPCTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class BcPCTStaticDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPCTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPCTStaticDraw();
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
	///	BcPNTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class BcPNTStaticDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTStaticDraw();
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
	///	BasicPNTStatic���f���`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class BcPNTStaticModelDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTStaticModelDraw();
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
	///	BcPNTBoneModel�`��R���|�[�l���g�i�{�[�����f���`��p�j
	//--------------------------------------------------------------------------------------
	class BcPNTBoneModelDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTBoneModelDraw();
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
	///	BasicPNTnTStatic�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class BcPNTnTStaticDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTStaticDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTStaticDraw();
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
	///	BasicPNTnT���f���`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class BcPNTnTStaticModelDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTStaticModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTStaticModelDraw();
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
	///	BcPNTnTBoneModelDraw�`��R���|�[�l���g�i�{�[�����f���`��p�j
	//--------------------------------------------------------------------------------------
	class BcPNTnTBoneModelDraw : public BcBaseDraw {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�R���X�g���N�^
		@param[in]	GameObjectPtr	�Q�[���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		explicit BcPNTnTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	�f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcPNTnTBoneModelDraw();
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




}
// end basecross
