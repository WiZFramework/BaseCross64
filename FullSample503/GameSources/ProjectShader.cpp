/*!
@file ProjectShader.cpp
@brief プロジェクトで使用するシェーダー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	IMPLEMENT_DX11_COMPUTE_SHADER(CSPointsBall, App::GetApp()->GetShadersPath() + L"CSPointsBall.cso")
	IMPLEMENT_DX11_CONSTANT_BUFFER(CBCalcPointsBall)

	IMPLEMENT_DX11_CONSTANT_BUFFER(CBDrawPointsBall)

	IMPLEMENT_DX11_VERTEX_SHADER(VSPointsBall, App::GetApp()->GetShadersPath() + L"VSPointsBall.cso")
	IMPLEMENT_DX11_GEOMETRY_SHADER(GSPointsBall, App::GetApp()->GetShadersPath() + L"GSPointsBall.cso")
	IMPLEMENT_DX11_PIXEL_SHADER(PSPointsBall, App::GetApp()->GetShadersPath() + L"PSPointsBall.cso")


}
//end basecross


