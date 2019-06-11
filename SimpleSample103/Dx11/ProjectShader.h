/*!
@file ProjectShader.h
@brief �v���W�F�N�g�Ŏg�p����V�F�[�_�[
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//PNStatic
	struct PNStaticConstantBuffer
	{
		Mat4x4 World;
		Mat4x4 View;
		Mat4x4 Projection;
		Vec4 LightDir;
		Col4 Emissive;
		Col4 Diffuse;
		PNStaticConstantBuffer() {
			memset(this, 0, sizeof(PNStaticConstantBuffer));
		};
	};

	DECLARE_DX11_CONSTANT_BUFFER(CBPNStatic, PNStaticConstantBuffer)
	DECLARE_DX11_VERTEX_SHADER(VSPNStatic, VertexPositionNormal)
	DECLARE_DX11_PIXEL_SHADER(PSPNStatic)


}
//end basecross

