Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

cbuffer ConstantBuffer:register(b1)
{
	float4 mtxWVP;
	float4 mtxWIT;
	float4 mtxWorld;
	float4 cameraPos;
}

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;

	float3 posW : POSITION1;
	float3 normalW : NORMAL1;

};

float4 main(PS_IN input) : SV_TARGET
{
	//// 2019/11/25 ライトベクトル(仮)をでっちあげる ////
	float3 lightDir = (1.0f, -1.0f, 1.0f);		/// 本当はレジスタ(CPU)から送ってあげてくれ
	//float light = dot(-normalize(lightDir), input.normalW);
	//light = saturate(light);

	float light = (dot(-normalize(lightDir), input.normalW) + 1.0f) * 0.5f;
	/////////////////////////////////////////////////

	/* スペキュラー計算 */
	float4 posWorld = mul(input.posW, mtxWorld);			// 物体のWorld座標
	float3 refv = reflect(lightDir, input.normalW);	// ライトベクトル
	float3 eyev = posWorld.xyz - cameraPos.xyz;			// 視線ベクトル
	refv = normalize(refv);		// ノーマライズ
	eyev = normalize(eyev);		// ノーマライズ
	float s = dot(refv, eyev);	// スペキュラー設定
	s = saturate(s);
	s = pow(s, 10);
	float4 specular = float4(s, s, s, 1.0f);


	return g_Texture.Sample(g_Sampler, input.texcoord) * light + specular;
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}