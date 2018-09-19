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
	//�X�v���C�g�p�R���X�^���g�o�b�t�@�\����
	struct SpriteConstantBuffer
	{
		Mat4x4 World;
		Col4 Emissive;
		SpriteConstantBuffer() {
			memset(this, 0, sizeof(SpriteConstantBuffer));
		};
	};
	DECLARE_DX11_CONSTANT_BUFFER(CBSprite, SpriteConstantBuffer)
	//PCSprite
	DECLARE_DX11_VERTEX_SHADER(VSPCSprite, VertexPositionColor)
	DECLARE_DX11_PIXEL_SHADER(PSPCSprite)


}
//end basecross

