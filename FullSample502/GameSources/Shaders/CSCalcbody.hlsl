
//--------------------------------------------------------------------------------------
// コンスタントバッファ
//--------------------------------------------------------------------------------------
cbuffer CB : register(b0)
{
	row_major float4x4 g_world;
	float4 g_playerPosition;
	float4  g_paramf;
	// g_paramf[0] == elapsedTime
	// g_paramf[1] == velocityPower
	// g_paramf[2] == gravity_y
	// g_paramf[3] == baseY
};

//--------------------------------------------------------------------------------------
// 各点のデータ
//--------------------------------------------------------------------------------------
struct LocalData {
	float4 m_LocalPosition;
	float4 m_WorldPosition;
	float4 m_Velocity;
	uint4  m_State;
};


//シェーダリソースビュー
StructuredBuffer<LocalData> inPosVelo : register(t0);
//アクセスビュー
RWStructuredBuffer<LocalData> outPosVelo : register(u0);


[numthreads(64, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	//入力からデータをして出力にコピー
	outPosVelo[DTid.x].m_LocalPosition = inPosVelo[DTid.x].m_LocalPosition;
	outPosVelo[DTid.x].m_LocalPosition.w = 1.0;
	outPosVelo[DTid.x].m_State = inPosVelo[DTid.x].m_State;
	uint state = outPosVelo[DTid.x].m_State.x;
	if (state == 2) {
		//落下した時以外はワールド位置を計算
		outPosVelo[DTid.x].m_WorldPosition = inPosVelo[DTid.x].m_WorldPosition;
	}
	else {
		//落下した時以外はワールド位置を計算
		outPosVelo[DTid.x].m_WorldPosition = mul(outPosVelo[DTid.x].m_LocalPosition, g_world);
	}
	outPosVelo[DTid.x].m_Velocity = inPosVelo[DTid.x].m_Velocity;
	//ステートをチェックして場合によってはワールドポジションを変更
	float baseY = g_paramf[3];
	float velocityPower = g_paramf[1];
	float4 gravity = float4(0, g_paramf[2], 0,0);
	float elapsedTime = g_paramf[0];
	float3 playerPos = float3(g_playerPosition.xyz);
	float3 targetPos = float3(outPosVelo[DTid.x].m_WorldPosition.xyz);
	if (state == 0) {
		float len = length(playerPos - targetPos);
		if (len < 0.4f) {
			//lenが0.4未満なら衝突してると判断
			//ステートを変更
			//衝突していたら球から飛び出すように速度を設定
			outPosVelo[DTid.x].m_State.x = 1;
			float3 velo = playerPos - targetPos;
			outPosVelo[DTid.x].m_Velocity.xyz = velo.xyz;
			outPosVelo[DTid.x].m_Velocity.w = 0.0;
			normalize(outPosVelo[DTid.x].m_Velocity);
			outPosVelo[DTid.x].m_Velocity.y = 1.0f;
			outPosVelo[DTid.x].m_Velocity *= velocityPower;
		}
	}
	else if (state == 1) {
		//飛び散る状態
		if (targetPos.y <= baseY) {
			//落下終了
			outPosVelo[DTid.x].m_State.x = 2;
			outPosVelo[DTid.x].m_WorldPosition.y = baseY;
		}
		else {
			//落下中
			outPosVelo[DTid.x].m_Velocity += gravity * elapsedTime;
			outPosVelo[DTid.x].m_LocalPosition += outPosVelo[DTid.x].m_Velocity * elapsedTime;
			outPosVelo[DTid.x].m_LocalPosition.w = 1.0;
			outPosVelo[DTid.x].m_WorldPosition = mul(outPosVelo[DTid.x].m_LocalPosition, g_world);
		}
	}
}
