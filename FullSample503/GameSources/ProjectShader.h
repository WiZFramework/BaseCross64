/*!
@file ProjectShader.h
@brief プロジェクトで使用するシェーダー
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	struct CBCS
	{
		Mat4x4 m_WorldMatrix;
		Vec4 m_PlayerPosition;
		// paramf[0] == elapsedTime
		// paramf[1] == velocityPower
		// paramf[2] == gravity_y
		// paramf[3] == baseY
		float paramf[4];
	};


	DECLARE_DX11_COMPUTE_SHADER(CSPointsBall)

	DECLARE_DX11_CONSTANT_BUFFER(CBCalcPointsBall, CBCS)

	struct CBVS
	{
		Mat4x4 m_World;
		Mat4x4 m_View;
		Mat4x4 m_Proj;

	};
	DECLARE_DX11_CONSTANT_BUFFER(CBDrawPointsBall, CBVS)

	DECLARE_DX11_VERTEX_SHADER(VSPointsBall, VertexPositionColor)
	DECLARE_DX11_GEOMETRY_SHADER(GSPointsBall)
	DECLARE_DX11_PIXEL_SHADER(PSPointsBall)



}
//end basecross


