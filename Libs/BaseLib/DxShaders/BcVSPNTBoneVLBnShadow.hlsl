
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"


// Vertex shader: vertex lighting, four bones.(biased vertex normals).
// VSSkinnedVertexLightingFourBonesBn
VSOutputTxShadow main(VSInputNmTxWeights vin)
{
	VSOutputTxShadow vout;

	float3 normal = BiasX2(vin.Normal);

	normal = Skin(vin, normal, 4);

	CommonVSOutput cout;
	if (Activeflags.x > 0) {
		cout = ComputeCommonVSOutputWithLighting(vin.Position, normal, Activeflags.x);
	}
	else {
		cout = ComputeCommonVSOutput(vin.Position);
	}
	SetCommonVSOutputParams;

	vout.TexCoord = vin.TexCoord;

	//�e�p
	vout.norm = mul(vin.Normal, (float3x3)World);
	vout.norm = normalize(vout.norm);
	float4 LightModelPos = float4(vin.Position.xyz, 1.0f);
	//���[���h�ϊ�
	LightModelPos = mul(LightModelPos, World);
	float4 LightSpacePos = mul(LightModelPos, LightView);
	LightSpacePos = mul(LightSpacePos, LightProjection);
	vout.lightSpacePos = LightSpacePos;
	// Light ray
	vout.lightRay = LightPos.xyz - LightModelPos.xyz;
	//View
	vout.lightView = EyePos.xyz - LightModelPos.xyz;


	return vout;
}


