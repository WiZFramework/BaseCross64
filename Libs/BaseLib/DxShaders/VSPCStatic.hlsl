//--------------------------------------------------------------------------------------
// File: VSPCStatic.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"
#include "INCParameters.hlsli"


PSPCInput main(VSPCInput input)
{
	PSPCInput result;
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
	result.color = input.color;
	return result;
}
