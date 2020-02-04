// 定数バッファ
cbuffer cbBuffer_hull : register(b0)
{
    float g_fTessFactor : packoffset(c0.x); // パッチのエッジのテッセレーション係数
    float g_fInsideTessFactor : packoffset(c0.y); // パッチ内部のテッセレーション係数
};

cbuffer cbBuffer_domain : register(b1)
{
    column_major float4x4 g_matCameraWorldViewProj : packoffset(c0); // カメラビューでのワールドとビューと射影行列
    float4 g_vecWaveGeneratePosition[2] : packoffset(c4); // xyz:波の生成座標
    float4 g_vecWaveParameters[2] : packoffset(c6); // x:振幅、y:波長、z:角度、w:波のエッジ
};

cbuffer cbBuffer_pixel : register(b2)
{
    float4 g_vecLightPos : packoffset(c0); // ローカル座標系での平行光源の位置ベクトル
};

// 頂点シェーダーの入力パラメータ
struct VS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD;
};

// 頂点シェーダーの出力パラメータ
struct VS_OUT_HS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};

// *****************************************************************************************
// 頂点シェーダー
// *****************************************************************************************
VS_OUT_HS_IN EffectiveWaterSimulation_Pass0_VS_Main(VS_IN In)
{
    VS_OUT_HS_IN Out;

    Out.pos = In.pos;
    Out.normal = In.normal;
    Out.texel = In.texel;
    return Out;
}