//--------------------------------------------------------------------------------------
// File: PSPVTStatic.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"
#include "INCParameters.hlsli"

float4 main(PSPNInput input) : SV_TARGET
{
	//法線ライティング
	float3 lightdir = normalize(LightDir.xyz);
	float3 N1 = normalize(input.norm);
	float4 Light = saturate(dot(N1, -lightdir) * Diffuse + Emissive);
	Light.a = Diffuse.a;
	return Light;
}
