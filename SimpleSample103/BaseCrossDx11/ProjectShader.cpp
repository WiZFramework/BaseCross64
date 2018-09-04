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
	//PNStatic
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBPNStatic)
	IMPLEMENT_DX11_VERTEX_SHADER(VSPNStatic, App::GetApp()->GetShadersPath() + L"VSPNStatic.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPNStatic, App::GetApp()->GetShadersPath() + L"PSPNStatic.cso")

}
//end basecross


