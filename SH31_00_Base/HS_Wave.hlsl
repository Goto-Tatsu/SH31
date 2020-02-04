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

// �n���V�F�[�_�[�̃p�b�`�萔�t�F�[�Y�p�̏o�̓p�����[�^
struct CONSTANT_HS_OUT_DS_IN
{
    float Edges[3] : SV_TessFactor;		// �p�b�`�̃G�b�W�̃e�b�Z���[�V�����W��
    float Inside : SV_InsideTessFactor; // �p�b�`�����̃e�b�Z���[�V�����W��
};


// ���_�V�F�[�_�[�̏o�̓p�����[�^
struct VS_OUT_HS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};


// *****************************************************************************************
// �n���V�F�[�_�[�̃R���g���[�� �|�C���g �t�F�[�Y
// *****************************************************************************************
[domain("tri")] // �e�b�Z���[�g���郁�b�V���̌`����w�肷��
[partitioning("integer")] // �p�b�`�̕����Ɏg�p����A���S���Y�����w�肷��
[outputtopology("triangle_ccw")] // ���b�V���̏o�͕��@���w�肷��
[outputcontrolpoints(3)] // �n���V�F�[�_�[�̃R���g���[�� �|�C���g �t�F�[�Y���R�[��������
[patchconstantfunc("EffectiveWaterSimulation_Pass0_ConstantHS")] // �Ή�����n���V�F�[�_�[�̃p�b�`�萔�t�F�[�Y�̃��C���֐�
CONTROL_HS_OUT_DS_IN EffectiveWaterSimulation_Pass0_HS_Main(InputPatch<VS_OUT_HS_IN, 3> In, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID)
{
    CONTROL_HS_OUT_DS_IN Out;

    Out.pos = In[i].pos;
    Out.normal = In[i].normal;
    Out.texel = In[i].texel;
    return Out;
}

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