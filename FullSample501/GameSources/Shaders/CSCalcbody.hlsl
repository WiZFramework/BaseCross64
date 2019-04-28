
//--------------------------------------------------------------------------------------
// �R���X�^���g�o�b�t�@
//--------------------------------------------------------------------------------------
cbuffer CB : register(b0)
{
	float4  g_paramf;
};

//--------------------------------------------------------------------------------------
// �G�������g�f�[�^
//--------------------------------------------------------------------------------------
struct ElemData
{
	float4 pos;
};

//���o�͗p�o�b�t�@
RWStructuredBuffer<ElemData> posData : register(u0);
//���L�ϐ�
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
