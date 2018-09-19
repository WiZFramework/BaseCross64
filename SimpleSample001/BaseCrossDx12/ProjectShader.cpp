/*!
@file ProjectShader.cpp
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	IMPLEMENT_DX12SHADER(VSPNTStatic, App::GetApp()->GetShadersPath() + L"VSPNTStatic.cso")
	IMPLEMENT_DX12SHADER(PSPNTStatic, App::GetApp()->GetShadersPath() + L"PSPNTStatic.cso")

	IMPLEMENT_DX12SHADER(VSPCDirect, App::GetApp()->GetShadersPath() + L"VSPCDirect.cso")
	IMPLEMENT_DX12SHADER(PSPCDirect, App::GetApp()->GetShadersPath() + L"PSPCDirect.cso")

}
//end basecross


