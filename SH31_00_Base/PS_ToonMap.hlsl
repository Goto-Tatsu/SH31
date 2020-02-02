Texture2D TextureBase : register(t0);
Texture2D TextureToon : register(t1);
SamplerState Sampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	float4x4 mtxWVP; // World,View,Projection
	float4x4 mtxWIT; // WorldInverseTranspose
	float4x4 world;
	float4 cameraPos;
}

struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL0;
	float4 color : COLOR0;		// モデルのテクスチャーカラー
	float2 texcoord : TEXCOORD0;

	float3 posW : POSITION1;
	float3 normalW : NORMAL1;

};

float4 main(PS_IN input) : SV_TARGET
{
	//// 2019/11/25 ライトベクトル(仮)をでっちあげる ////
	float3 lightDir = (1.0f, -1.0f, 1.0f); /// 本当はレジスタ(CPU)から送ってあげてくれ
	lightDir = normalize(lightDir);
	//float light = dot(-normalize(lightDir), input.normalW);
	//light = saturate(light);
	
	//　法線ベクトル
	input.normalW = normalize(input.normalW);

	// 視線ベクトル
	//float3 toEyeW = cameraPos.xyz - input.posW; // 視線ベクトル
	float3 toEyeW = input.posW - cameraPos.xyz; // 視線ベクトル
	toEyeW = normalize(toEyeW); // ノーマライズ

	
	
	// ハーフランバート
	//float light = (dot(-normalize(lightDir), input.normalW) + 1.0f) * 0.5f; // 旧ムラセ式
	float light = 0.5f - dot(input.normalW, lightDir.xyz) * 0.49f;	// 新ムラセ式
	light *= 0.99f;
	
	// 対抗率
	float facing = dot(-toEyeW, input.normalW) * 0.99f;
	
	// ﾃｸｽﾁｬ	
	float2 toonTexcoord;
	toonTexcoord.x = light;
	toonTexcoord.y = facing;
	
	float4 color = TextureBase.Sample(Sampler, input.texcoord);
	color.rgb *= TextureToon.Sample(Sampler, toonTexcoord).rgb;
	return color;
}