//--------------------------------------------------------------------------------------
// File: VSPTStaticInstance.hlsl
//
//--------------------------------------------------------------------------------------

#include "INCStructs.hlsli"
#include "INCParameters.hlsli"

PSPTInput main(VSPTInstanceInput input)
{
	PSPTInput result;
	//���_�̈ʒu��ϊ�
	float4 pos = float4(input.position.xyz, 1.0f);
	//���[���h�ϊ�
	pos = mul(pos, input.mat);
	//�r���[�ϊ�
	pos = mul(pos, View);
	//�ˉe�ϊ�
	pos = mul(pos, Projection);
	//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
	result.position = pos;
	result.tex = input.tex;

	return result;
}
