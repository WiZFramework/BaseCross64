/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	IMPLEMENT_DX12SHADER(VSPTSprite, App::GetApp()->GetShadersPath() + L"VSPTSprite.cso")
	IMPLEMENT_DX12SHADER(PSPTSprite, App::GetApp()->GetShadersPath() + L"PSPTSprite.cso")
}
//end basecross


