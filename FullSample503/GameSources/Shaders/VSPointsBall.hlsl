
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

//シェーダリソースビュー
StructuredBuffer<WorldData> inMatrix : register(t0);


GSPCInput main(VSPCInput input)
{
	GSPCInput result;
	//ジオメトリシェーダに渡す変数にシェーダリソースビューの値を設定
	result.position = inMatrix[input.id].Pos;
	result.color = input.color;
	return result;
}

