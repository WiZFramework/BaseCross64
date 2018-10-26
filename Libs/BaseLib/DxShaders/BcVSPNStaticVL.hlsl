
#include "BcINCParameters.hlsli"
#include "BcINCStructs.hlsli"
#include "BcINCCommon.hlsli"
#include "BcINCLighting.hlsli"

// Vertex shader: vertex lighting
// VSBasicVertexLighting
VSOutput main(VSInputNm vin)
{

	VSOutput vout;

	CommonVSOutput cout;

	if (Activeflags.x > 0) {
		cout = ComputeCommonVSOutputWithLighting(vin.Position, vin.Normal, Activeflags.x);
	}
	else {
		cout = ComputeCommonVSOutput(vin.Position);
	}

    SetCommonVSOutputParams;


    return vout;
}

