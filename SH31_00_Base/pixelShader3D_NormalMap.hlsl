Texture2D       Texture0  : register(t0);
Texture2D       Texture1  : register(t1);
Texture2D       Texture2  : register(t2);
//Texture2D Textures[2] : register(t0);

SamplerState    Sampler     : register(s0);

struct PS_IN
{
	float4 posH : SV_POSITION;
	float3 posW : POSITION0;
	float3 binormalW : BINORMAL0;
	float3 tangentW : TANGENT0;
	float3 normalW : NORMAL0;
	float2 texcoord : TEXCOORD0;
};


cbuffer ConstantBuffer:register(b0)
{
	float4x4 mtxWVP;
	float4x4 mtxWIT;
	float4x4 mtxWorld;
	float4 cameraPos;
}

float4 main(PS_IN input) : SV_TARGET
{
	float4 normalmap = Texture1.Sample(Sampler,input.texcoord) * 2.0f - 1.0f;// 法線マップからの法線
	
	// ここのxyzは目で見て+-調節してあげてください。
	float3 normalW = normalize(input.binormalW * normalmap.x + -input.tangentW * normalmap.y + input.normalW * normalmap.z);

	// 平行光の位置と明るさ
	float3 lightDirW = float3(1.0f, -1.0f, 1.0f);	// ライトベクトル
	lightDirW = normalize(lightDirW);
	float l = saturate(dot(-lightDirW, normalW));     
	
	//// スペキュラー ////
	float3 refW = reflect(lightDirW, normalW); // ライトベクトル
	refW = normalize(refW); // ノーマライズ
	
	float3 eyeW = cameraPos.xyz - input.posW; // 視線ベクトル
	eyeW = normalize(eyeW); // ノーマライズ

	// 視差マッピング ///////////////////////////
	float3 eyeT;    // 符号はnormalWと合わせてね
	eyeT.x = dot(eyeW, input.binormalW);
	eyeT.y = dot(eyeW, -input.tangentW);
	eyeT.z = dot(eyeW, input.normalW);
	float height_map = Texture1.Sample(Sampler, input.texcoord) * 2.0f - 1.0f;
	float2 parallax_texcoord = input.texcoord + (height_map * 0.2f * eyeT.xy);
	//////////////////////////////////////////////
	
	// スペキュラー設定
	float s = pow(saturate(-dot(refW, eyeT)), 10);
	float4 specular = float4(s, s, s, 1.0f);

	//l = (l + specular.rgb);
	//return float4(l, l, l, 1.0f) + specular;
	return float4(Texture0.Sample(Sampler, input.texcoord).rgb * l + specular.rgb, 1.0f);
	//return float4(Texture0.Sample(Sampler, parallax_texcoord).rgb * l + specular.rgb, 1.0f);

}