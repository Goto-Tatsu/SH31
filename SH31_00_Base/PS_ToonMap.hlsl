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
	float4 color : COLOR0;		// ���f���̃e�N�X�`���[�J���[
	float2 texcoord : TEXCOORD0;

	float3 posW : POSITION1;
	float3 normalW : NORMAL1;

};

float4 main(PS_IN input) : SV_TARGET
{
	//// 2019/11/25 ���C�g�x�N�g��(��)���ł��������� ////
	float3 lightDir = (1.0f, -1.0f, 1.0f); /// �{���̓��W�X�^(CPU)���瑗���Ă����Ă���
	lightDir = normalize(lightDir);
	//float light = dot(-normalize(lightDir), input.normalW);
	//light = saturate(light);
	
	//�@�@���x�N�g��
	input.normalW = normalize(input.normalW);

	// �����x�N�g��
	//float3 toEyeW = cameraPos.xyz - input.posW; // �����x�N�g��
	float3 toEyeW = input.posW - cameraPos.xyz; // �����x�N�g��
	toEyeW = normalize(toEyeW); // �m�[�}���C�Y

	
	
	// �n�[�t�����o�[�g
	//float light = (dot(-normalize(lightDir), input.normalW) + 1.0f) * 0.5f; // �������Z��
	float light = 0.5f - dot(input.normalW, lightDir.xyz) * 0.49f;	// �V�����Z��
	light *= 0.99f;
	
	// �΍R��
	float facing = dot(-toEyeW, input.normalW) * 0.99f;
	
	// ø���	
	float2 toonTexcoord;
	toonTexcoord.x = light;
	toonTexcoord.y = facing;
	
	float4 color = TextureBase.Sample(Sampler, input.texcoord);
	color.rgb *= TextureToon.Sample(Sampler, toonTexcoord).rgb;
	return color;
}