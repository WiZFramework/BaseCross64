
struct VSPCInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	uint    id      : SV_VERTEXID;
};

struct GSPCInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

struct WorldData {
	float4 Pos;
};

//�V�F�[�_���\�[�X�r���[
StructuredBuffer<WorldData> inMatrix : register(t0);


GSPCInput main(VSPCInput input)
{
	GSPCInput result;
	//�W�I���g���V�F�[�_�ɓn���ϐ��ɃV�F�[�_���\�[�X�r���[�̒l��ݒ�
	result.position = inMatrix[input.id].Pos;
	result.color = input.color;
	return result;
}

