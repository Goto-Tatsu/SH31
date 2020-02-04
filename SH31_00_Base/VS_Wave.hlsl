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

// ���_�V�F�[�_�[�̓��̓p�����[�^
struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD;
};

// ���_�V�F�[�_�[�̏o�̓p�����[�^
struct VS_OUT_HS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};

// *****************************************************************************************
// ���_�V�F�[�_�[
// *****************************************************************************************
VS_OUT_HS_IN EffectiveWaterSimulation_Pass0_VS_Main(VS_IN In)
{
    VS_OUT_HS_IN Out;

    Out.pos = In.pos;
    Out.normal = In.normal;
    Out.texel = In.texel;
    return Out;
}