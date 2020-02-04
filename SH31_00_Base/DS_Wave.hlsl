// �萔�o�b�t�@
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

// ���_�V�F�[�_�[�̏o�̓p�����[�^
struct VS_OUT_HS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};

// �n���V�F�[�_�[�̃p�b�`�萔�t�F�[�Y�p�̏o�̓p�����[�^
struct CONSTANT_HS_OUT_DS_IN
{
    float Edges[3] : SV_TessFactor; // �p�b�`�̃G�b�W�̃e�b�Z���[�V�����W��
    float Inside : SV_InsideTessFactor; // �p�b�`�����̃e�b�Z���[�V�����W��
};

// �n���V�F�[�_�[�̃R���g���[�� �|�C���g �t�F�[�Y�p�̏o�̓p�����[�^
struct CONTROL_HS_OUT_DS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};

// �h���C���V�F�[�_�[�̏o�̓p�����[�^
struct DS_OUT_PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};

// �g�̍������v�Z
float getHeight(float3 waveGeneratePos // �g�̐������W
                 , float3 localPos // ���_���W
                 , float amplitude // �U��
                 , float wavelength // �g��
                 , float angle // �p�x
                 , float waveEdge // �g�̃G�b�W
               )
{
   // �g�̐������W�ƒ��_���W�̋���
    float len = length(waveGeneratePos - localPos);
   // �g�̍������v�Z
    return amplitude * pow((sin(len / wavelength + angle) + 1.0f) * 0.5f, waveEdge);
}

// *****************************************************************************************
// �h���C���V�F�[�_�[
// *****************************************************************************************
[domain("tri")]
DS_OUT_PS_IN EffectiveWaterSimulation_Pass0_DS_Main(CONSTANT_HS_OUT_DS_IN In, float3 uvw : SV_DomainLocation, const OutputPatch<CONTROL_HS_OUT_DS_IN, 3> patch)
{
    DS_OUT_PS_IN Out;
    
    float aroundh[4] = { 0, 0, 0, 0 };
    float offset = 0.1f; // �K���ɐݒ�
    float dx, dz, h, totalh = 0.0f;
    
    
    // ���_���W
    float3 p = patch[2].pos * uvw.x + patch[1].pos * uvw.y + patch[0].pos * uvw.z;
   
   // ���[�J�����W�n�Ŕg����
    for (int i = 0; i < 2; i++)
    {
      // �g�̍������v�Z
        h = getHeight(g_vecWaveGeneratePosition[i].xyz, p, g_vecWaveParameters[i].x, g_vecWaveParameters[i].y, g_vecWaveParameters[i].z, g_vecWaveParameters[i].w);

        totalh += h;
      
      // �����Ώۂ̒��_�̎��͂S�ӏ��̔g�̍������v�Z
        aroundh[0] += getHeight(g_vecWaveGeneratePosition[i].xyz, p + float3(offset, 0, 0.0f), g_vecWaveParameters[i].x, g_vecWaveParameters[i].y, g_vecWaveParameters[i].z, g_vecWaveParameters[i].w);
        aroundh[1] += getHeight(g_vecWaveGeneratePosition[i].xyz, p + float3(0.0f, 0, -offset), g_vecWaveParameters[i].x, g_vecWaveParameters[i].y, g_vecWaveParameters[i].z, g_vecWaveParameters[i].w);
        aroundh[2] += getHeight(g_vecWaveGeneratePosition[i].xyz, p + float3(-offset, 0, 0.0f), g_vecWaveParameters[i].x, g_vecWaveParameters[i].y, g_vecWaveParameters[i].z, g_vecWaveParameters[i].w);
        aroundh[3] += getHeight(g_vecWaveGeneratePosition[i].xyz, p + float3(0.0f, 0, offset), g_vecWaveParameters[i].x, g_vecWaveParameters[i].y, g_vecWaveParameters[i].z, g_vecWaveParameters[i].w);
    }

    p.y += totalh;
 
   // �s��ϊ���̒��_���W
    Out.pos = mul(float4(p, 1.0f), g_matCameraWorldViewProj);

   // �����̍����v�Z
    dx = aroundh[2] - aroundh[0];
    dz = aroundh[1] - aroundh[3];

   // �@���x�N�g��( y�͓K���ɒ��� )
    Out.normal = normalize(float3(dx, 0.2f, dz));

   // �e�N�Z��
    Out.texel = patch[2].texel * uvw.x + patch[1].texel * uvw.y + patch[0].texel * uvw.z;

    return Out;
}