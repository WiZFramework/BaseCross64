//--------------------------------------------------------------------------------------
// File: PSPVTStaticShadow.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"
#include "INCParameters.hlsli"

Texture2D<float4> g_texture : register(t0);
// �[�x�}�b�v
Texture2D g_DepthMap : register(t1);
SamplerState g_sampler : register(s0);
SamplerState g_SamplerDepthMap : register(s1);


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PSPNTInputShadow input) : SV_TARGET
{

	float shadowColor = 1.0f;
	// �e�N�Z�����v�Z
	float2 texel = float2(
		input.lightSpacePos.x / input.lightSpacePos.w * 0.5f + 0.5f,
		input.lightSpacePos.y / input.lightSpacePos.w * -0.5f + 0.5f
		);
	float depth = 0;
	float sm = 0;
	depth = input.lightSpacePos.z / input.lightSpacePos.w - 0.00005f;
	sm = g_DepthMap.Sample(g_SamplerDepthMap, texel).x;
	if (texel.x >= 0 && texel.x <= 1 && texel.y >= 0 && texel.y <= 1){
		// ���݂̐[�x�l�Ɛ[�x�}�b�v��̐[�x�l���r
		if (sm > depth){
			shadowColor = 1.0f;
		}
		else{
			shadowColor = 0.7f;
		}
	}

	//�@�����C�e�B���O
	float3 lightdir = normalize(LightDir.xyz);
	float3 N1 = normalize(input.norm);
	float4 RetColor = (saturate(dot(N1, -lightdir)) * Diffuse) + Emissive;
	RetColor += input.specular;
	RetColor.a = Diffuse.a;
	if (Activeflags.x){
		//�e�N�X�`���ƃf�t�B�[�Y���烉�C�e�B���O���쐬
		RetColor = g_texture.Sample(g_sampler, input.tex) * RetColor;
	}
	RetColor = saturate(RetColor);
	//�e�𑫂�
	RetColor.rgb = RetColor.rgb * shadowColor;
	return RetColor;
}