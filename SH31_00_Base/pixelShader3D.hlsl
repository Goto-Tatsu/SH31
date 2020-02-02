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
	//// 2019/11/25 ���C�g�x�N�g��(��)���ł��������� ////
	float3 lightDir = (1.0f, -1.0f, 1.0f);		/// �{���̓��W�X�^(CPU)���瑗���Ă����Ă���
	//float light = dot(-normalize(lightDir), input.normalW);
	//light = saturate(light);

	// �n�[�t�����o�[�g
	//float light = (dot(-normalize(lightDir), input.normalW) + 1.0f) * 0.5f;
	/////////////////////////////////////////////////

	// �����x�N�g��
	float3 toEyeW = cameraPos.xyz - input.posW; // �����x�N�g��
	toEyeW = normalize(toEyeW); // �m�[�}���C�Y

	// ���˃x�N�g��
	float3 refW = reflect(toEyeW,input.normalW);
	refW = normalize(refW);
	
	// ���˃x�N�g������X�t�B�A�}�b�v�̂ǂ�UV���ʂ肱��ł��邩���߂�
	// refW.xy * ���̔��a + UV��0��
	float2 envTexcoord = refW.xy * 0.3f + 0.5f;	
	return TextureEnv.Sample(Sampler, envTexcoord);
	
}