/*!
@file ProjectShader.h
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	
	//--------------------------------------------------------------------------------------
	//	�`��p�̃V�F�[�_�N���X�̐錾�i�}�N���g�p�j
	//--------------------------------------------------------------------------------------
	// Diffuse����R���X�^���g�o�b�t�@ CBDiffuseSprite
	struct DiffuseSpriteConstantBuffer
	{
		Mat4x4 World;
		Col4 Emissive;
		Col4 Diffuse;
		DiffuseSpriteConstantBuffer() {
			memset(this, 0, sizeof(DiffuseSpriteConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBDiffuseSprite, DiffuseSpriteConstantBuffer)

	//VSPTSprite PSPTSprite
	DECLARE_DX11_VERTEX_SHADER(VSPTSprite, VertexPositionTexture)
	DECLARE_DX11_PIXEL_SHADER(PSPTSprite)


}
//end basecross

