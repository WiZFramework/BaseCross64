
//--------------------------------------------------------------------------------------
// コンスタントバッファ
//--------------------------------------------------------------------------------------
cbuffer CB : register(b0)
{
	float4  g_paramf;
};

//--------------------------------------------------------------------------------------
// エレメントデータ
//--------------------------------------------------------------------------------------
struct ElemData
{
	float4 pos;
};

//入出力用バッファ
RWStructuredBuffer<ElemData> posData : register(u0);
//共有変数
groupshared float4 sharedPos;


[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	sharedPos = posData[DTid.x].pos;
	GroupMemoryBarrierWithGroupSync();
	sharedPos.x = sin(g_paramf.x);
	GroupMemoryBarrierWithGroupSync();
	posData[DTid.x].pos = sharedPos;
}
