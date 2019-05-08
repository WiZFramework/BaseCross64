
struct PSPCTInput
{
	float2 tex			: TEXCOORD0;
	float4 color		: COLOR;
};


Texture2D		            g_txDiffuse : register(t0);
SamplerState g_sampler : register(s0);


float4 main(PSPCTInput input) : SV_TARGET
{
	return g_txDiffuse.Sample(g_sampler, input.tex) * input.color;
}