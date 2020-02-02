Texture2D TextureEnv : register(t0);
Texture2D TextureBase : register(t1);
SamplerState Sampler : register(s0);

cbuffer ConstantBuffer:register(b0)
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

	// ハーフランバート
	//float light = (dot(-normalize(lightDir), input.normalW) + 1.0f) * 0.5f;
	/////////////////////////////////////////////////

	// 視線ベクトル
	float3 toEyeW = cameraPos.xyz - input.posW; // 視線ベクトル
	toEyeW = normalize(toEyeW); // ノーマライズ

	// 反射ベクトル
	float3 refW = reflect(toEyeW,input.normalW);
	refW = normalize(refW);
	
	// 反射ベクトルからスフィアマップのどのUVが写りこんでいるか求める
	// refW.xy * 球の半径 + UVを0に
	float2 envTexcoord = refW.xy * 0.3f + 0.5f;	
	return TextureEnv.Sample(Sampler, envTexcoord);
	
}