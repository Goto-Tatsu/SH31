// 定数バッファ
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

// 頂点シェーダーの出力パラメータ
struct VS_OUT_HS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};

// ハルシェーダーのパッチ定数フェーズ用の出力パラメータ
struct CONSTANT_HS_OUT_DS_IN
{
    float Edges[3] : SV_TessFactor; // パッチのエッジのテッセレーション係数
    float Inside : SV_InsideTessFactor; // パッチ内部のテッセレーション係数
};

// ハルシェーダーのコントロール ポイント フェーズ用の出力パラメータ
struct CONTROL_HS_OUT_DS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};

// ドメインシェーダーの出力パラメータ
struct DS_OUT_PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};

// 波の高さを計算
float getHeight(float3 waveGeneratePos // 波の生成座標
                 , float3 localPos // 頂点座標
                 , float amplitude // 振幅
                 , float wavelength // 波長
                 , float angle // 角度
                 , float waveEdge // 波のエッジ
               )
{
   // 波の生成座標と頂点座標の距離
    float len = length(waveGeneratePos - localPos);
   // 波の高さを計算
    return amplitude * pow((sin(len / wavelength + angle) + 1.0f) * 0.5f, waveEdge);
}

// *****************************************************************************************
// ドメインシェーダー
// *****************************************************************************************
[domain("tri")]
DS_OUT_PS_IN EffectiveWaterSimulation_Pass0_DS_Main(CONSTANT_HS_OUT_DS_IN In, float3 uvw : SV_DomainLocation, const OutputPatch<CONTROL_HS_OUT_DS_IN, 3> patch)
{
    DS_OUT_PS_IN Out;
    
    float aroundh[4] = { 0, 0, 0, 0 };
    float offset = 0.1f; // 適当に設定
    float dx, dz, h, totalh = 0.0f;
    
    
    // 頂点座標
    float3 p = patch[2].pos * uvw.x + patch[1].pos * uvw.y + patch[0].pos * uvw.z;
   
   // ローカル座標系で波生成
    for (int i = 0; i < 2; i++)
    {
      // 波の高さを計算
        h = getHeight(g_vecWaveGeneratePosition[i].xyz, p, g_vecWaveParameters[i].x, g_vecWaveParameters[i].y, g_vecWaveParameters[i].z, g_vecWaveParameters[i].w);

        totalh += h;
      
      // 処理対象の頂点の周囲４箇所の波の高さを計算
        aroundh[0] += getHeight(g_vecWaveGeneratePosition[i].xyz, p + float3(offset, 0, 0.0f), g_vecWaveParameters[i].x, g_vecWaveParameters[i].y, g_vecWaveParameters[i].z, g_vecWaveParameters[i].w);
        aroundh[1] += getHeight(g_vecWaveGeneratePosition[i].xyz, p + float3(0.0f, 0, -offset), g_vecWaveParameters[i].x, g_vecWaveParameters[i].y, g_vecWaveParameters[i].z, g_vecWaveParameters[i].w);
        aroundh[2] += getHeight(g_vecWaveGeneratePosition[i].xyz, p + float3(-offset, 0, 0.0f), g_vecWaveParameters[i].x, g_vecWaveParameters[i].y, g_vecWaveParameters[i].z, g_vecWaveParameters[i].w);
        aroundh[3] += getHeight(g_vecWaveGeneratePosition[i].xyz, p + float3(0.0f, 0, offset), g_vecWaveParameters[i].x, g_vecWaveParameters[i].y, g_vecWaveParameters[i].z, g_vecWaveParameters[i].w);
    }

    p.y += totalh;
 
   // 行列変換後の頂点座標
    Out.pos = mul(float4(p, 1.0f), g_matCameraWorldViewProj);

   // 高さの差を計算
    dx = aroundh[2] - aroundh[0];
    dz = aroundh[1] - aroundh[3];

   // 法線ベクトル( yは適当に調整 )
    Out.normal = normalize(float3(dx, 0.2f, dz));

   // テクセル
    Out.texel = patch[2].texel * uvw.x + patch[1].texel * uvw.y + patch[0].texel * uvw.z;

    return Out;
}