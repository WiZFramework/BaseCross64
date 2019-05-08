
struct GSPCInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

struct GSPCOutput
{
	float2 tex			: TEXCOORD0;
	float4 color		: COLOR;
	float4 pos			: SV_POSITION;
};


cbuffer CBVS : register(b0)
{
	float4x4 g_World	: packoffset(c0);
	float4x4 g_View	: packoffset(c4);
	float4x4 g_Projection	: packoffset(c8);
};



cbuffer cbImmutable
{
	static float4 g_positions4[4] =
	{
		float4(-0.1, 0.1, 0,0),
		float4(0.1, 0.1, 0,0),
		float4(-0.1, -0.1, 0,0),
		float4(0.1, -0.1, 0,0),
	};
	static float2 g_texcoords[4] =
	{
		float2(0,0),
		float2(1,0),
		float2(0,1),
		float2(1,1),
	};
};

[maxvertexcount(4)]
void main(
	point GSPCInput input[1],
	inout TriangleStream< GSPCOutput > output
)
{
	GSPCOutput element;
	for (int i = 0; i < 4; i++)
	{
		float4 pos = g_positions4[i] + input[0].position;
		pos.w = 1.0;
		//ƒrƒ…[•ÏŠ·
		pos = mul(pos, g_View);
		//ŽË‰e•ÏŠ·
		pos = mul(pos, g_Projection);
		element.color = input[0].color;
		element.tex = g_texcoords[i];
		element.pos = pos;
		output.Append(element);
	}
	output.RestartStrip();
}