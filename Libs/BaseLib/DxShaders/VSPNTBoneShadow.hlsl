
//--------------------------------------------------------------------------------------
// File: VSPNTBoneShadow.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"
#include "INCParameters.hlsli"

void Skin(inout VSPNTBoneInput vin, uniform int boneCount)
{
	float4x3 skinning = 0;

	[unroll]
	for (int i = 0; i < boneCount; i++)
	{
		skinning += Bones[vin.indices[i]] * vin.weights[i];
	}

	vin.position.xyz = mul(vin.position, skinning);
	vin.norm = mul(vin.norm, (float3x3)skinning);
}


PSPNTInputShadow main(VSPNTBoneInput input)
{
	PSPNTInputShadow result;

	Skin(input, 4);

	//���_�̈ʒu��ϊ�
	float4 pos = float4(input.position.xyz, 1.0f);
	//���[���h�ϊ�
	pos = mul(pos, World);
	//�r���[�ϊ�
	pos = mul(pos, View);
	//�ˉe�ϊ�
	pos = mul(pos, Projection);
	//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
	result.position = pos;
	//���C�e�B���O
	result.norm = mul(input.norm, (float3x3)World);
	result.norm = normalize(result.norm);
	//�X�y�L�����[
	float3 H = normalize(normalize(-LightDir.xyz) + normalize(EyePos.xyz - pos.xyz));
	result.specular = Specular * dot(result.norm, H);
	//�e�N�X�`��UV
	result.tex = input.tex;
	//�e�̂��߂̕ϐ�
	float4 LightModelPos = float4(input.position.xyz, 1.0f);
	//���[���h�ϊ�
	LightModelPos = mul(LightModelPos, World);

	float4 LightSpacePos = mul(LightModelPos, LightView);
	LightSpacePos = mul(LightSpacePos, LightProjection);
	result.lightSpacePos = LightSpacePos;

	// Light ray
	result.lightRay = LightPos.xyz - LightModelPos.xyz;
	//View
	result.lightView = EyePos.xyz - LightModelPos.xyz;

	return result;


}

