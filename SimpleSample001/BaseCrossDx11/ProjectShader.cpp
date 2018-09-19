/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�`��p�̃V�F�[�_�N���X�̎��́i�}�N���g�p�j
	//--------------------------------------------------------------------------------------
	//PCDirect
	IMPLEMENT_DX11_VERTEX_SHADER(VSPCDirect, App::GetApp()->GetShadersPath() + L"VSPCDirect.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPCDirect, App::GetApp()->GetShadersPath() + L"PSPCDirect.cso")

}
//end basecross


