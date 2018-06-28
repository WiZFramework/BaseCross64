//--------------------------------------------------------------------------------------
// File: PSPCSprite.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	row_major float4x4 MatrixTransform : packoffset(c0);
	float4 Emissive : packoffset(c4);
	float4 Diffuse : packoffset(c5);
};


float4 main(PSPCInput input) : SV_TARGET
{
	//���_�F���烉�C�e�B���O���쐬
	float4 Light = (saturate(input.color) * Diffuse) + Emissive;
	return Light;
}
