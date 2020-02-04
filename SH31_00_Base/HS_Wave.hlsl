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

// ハルシェーダーのパッチ定数フェーズ用の出力パラメータ
struct CONSTANT_HS_OUT_DS_IN
{
    float Edges[3] : SV_TessFactor;		// パッチのエッジのテッセレーション係数
    float Inside : SV_InsideTessFactor; // パッチ内部のテッセレーション係数
};


// 頂点シェーダーの出力パラメータ
struct VS_OUT_HS_IN
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texel : TEXCOORD0;
};


// *****************************************************************************************
// ハルシェーダーのコントロール ポイント フェーズ
// *****************************************************************************************
[domain("tri")] // テッセレートするメッシュの形状を指定する
[partitioning("integer")] // パッチの分割に使用するアルゴリズムを指定する
[outputtopology("triangle_ccw")] // メッシュの出力方法を指定する
[outputcontrolpoints(3)] // ハルシェーダーのコントロール ポイント フェーズがコールされる回数
[patchconstantfunc("EffectiveWaterSimulation_Pass0_ConstantHS")] // 対応するハルシェーダーのパッチ定数フェーズのメイン関数
CONTROL_HS_OUT_DS_IN EffectiveWaterSimulation_Pass0_HS_Main(InputPatch<VS_OUT_HS_IN, 3> In, uint i : SV_OutputControlPointID, uint PatchID : SV_PrimitiveID)
{
    CONTROL_HS_OUT_DS_IN Out;

    Out.pos = In[i].pos;
    Out.normal = In[i].normal;
    Out.texel = In[i].texel;
    return Out;
}

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