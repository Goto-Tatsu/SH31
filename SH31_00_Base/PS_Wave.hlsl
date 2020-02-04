// テクスチャー
Texture2D g_Tex : register(t0);

// サンプラーステート
SamplerState g_Sampler;

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

// ドメインシェーダーの出力パラメータ
struct DS_OUT_PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};

// *****************************************************************************************
// ピクセルシェーダ
// *****************************************************************************************
float4 EffectiveWaterSimulation_Pass0_PS_Main(DS_OUT_PS_IN In) : SV_TARGET
{
   // ハーフランバート
    float lambert = dot(g_vecLightPos.xyz, In.normal);
    lambert = lambert * 0.5f + 0.5f;

//   return float4( In.normal.zzz * 0.5f + 0.5f, 1 );
    return g_Tex.Sample(g_Sampler, In.texel).bgra * lambert;
}