// �e�N�X�`���[
Texture2D g_Tex : register(t0);

// �T���v���[�X�e�[�g
SamplerState g_Sampler;

// �萔�o�b�t�@
cbuffer cbBuffer_hull : register(b0)
{
    float g_fTessFactor : packoffset(c0.x); // �p�b�`�̃G�b�W�̃e�b�Z���[�V�����W��
    float g_fInsideTessFactor : packoffset(c0.y); // �p�b�`�����̃e�b�Z���[�V�����W��
};

cbuffer cbBuffer_domain : register(b1)
{
    column_major float4x4 g_matCameraWorldViewProj : packoffset(c0); // �J�����r���[�ł̃��[���h�ƃr���[�Ǝˉe�s��
    float4 g_vecWaveGeneratePosition[2] : packoffset(c4); // xyz:�g�̐������W
    float4 g_vecWaveParameters[2] : packoffset(c6); // x:�U���Ay:�g���Az:�p�x�Aw:�g�̃G�b�W
};

cbuffer cbBuffer_pixel : register(b2)
{
    float4 g_vecLightPos : packoffset(c0); // ���[�J�����W�n�ł̕��s�����̈ʒu�x�N�g��
};

// �h���C���V�F�[�_�[�̏o�̓p�����[�^
struct DS_OUT_PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};

// *****************************************************************************************
// �s�N�Z���V�F�[�_
// *****************************************************************************************
float4 EffectiveWaterSimulation_Pass0_PS_Main(DS_OUT_PS_IN In) : SV_TARGET
{
   // �n�[�t�����o�[�g
    float lambert = dot(g_vecLightPos.xyz, In.normal);
    lambert = lambert * 0.5f + 0.5f;

//   return float4( In.normal.zzz * 0.5f + 0.5f, 1 );
    return g_Tex.Sample(g_Sampler, In.texel).bgra * lambert;
}