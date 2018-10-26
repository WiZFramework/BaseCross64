#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Pixel shader: vertex lighting
// PSBasicVertexLighting
float4 main(PSInput pin) : SV_Target0
{
	float4 color = float4(1,1,1,1);
	color *= pin.Diffuse;

    AddSpecular(color, pin.Specular.rgb);
    ApplyFog(color, pin.Specular.w);

    return color;
}
