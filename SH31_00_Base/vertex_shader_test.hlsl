//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}
cbuffer ViewBuffer : register(b1)
{
    matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}

struct VS_IN
{
    float4 pos : POSITION0;
    float4 normal : NORMAL0;
    float4 color : COLOR0;
    float2 texcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 pos : SV_POSITION;
    float4 normal : NORMAL0;
    float4 color : COLOR0;
    float2 texcoord : TEXCOORD0;

    float3 posW : POSITION1;
    float3 normalW : NORMAL1;
};

void main(in VS_IN input, out VS_OUT output)
{
    input.normal.w = 0.0f;
    output.normalW = mul(input.normal, mtxWIT).xyz;

    output.pos = mul(input.pos, mtxWVP);
    output.color = input.color;
    output.texcoord = input.texcoord;

    output.posW = mul(input.pos, world);

	// ライトカラー設定もCPUから送ってくるものだからね。
	// alpha値のところは１入れといてあげてね、あんしん。
}
