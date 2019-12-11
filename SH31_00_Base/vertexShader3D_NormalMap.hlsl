cbuffer ConstantBuffer : register(b1)
{
    float4x4 mtxWVP;
    float4x4 mtxWIT;
    float4x4 mtxWorld;
    float4 cameraPos;
}

// tangentやbinormalは3Dモデリングのソフトによってざひょうけいがことなるので、
// 
struct VS_IN
{
	float4 posL         : POSITION0;
	float4 binormalL    : BINORMAL0;
	float4 tangentL     : TANGENT0;
	float4 normalL      : NORMAL0;
	float2 texcoord     : TEXCOORD0;
};

struct VS_OUT {
	float4 posH : SV_POSITION;
	float3 posW : POSITION1;
    float3 binormalW : BINORMAL0;
    float3 tangentW : TANGENT0;
	float3 normalW : NORMAL0;
	float2 texcoord	: TEXCOORD0;
};

void main(in VS_IN input, out VS_OUT output)
{

	output.posH = mul(input.posL, mtxWVP);
    output.posW = mul(input.posL, mtxWorld).xyz;
    
    input.binormalL.w = 0.0f;
    output.binormalW = mul(input.binormalL, mtxWIT).xyz;

    input.tangentL.w = 0.0;
    output.tangentW = mul(input.tangentL, mtxWIT).xyz;

    input.normalL.w = 0.0f;
	output.normalW = mul(input.normalL, mtxWIT).xyz;
    	
    output.texcoord = input.texcoord;
    
	// ライトカラー設定もCPUから送ってくるものだからね。
	// alpha値のところは１入れといてあげてね、あんしん。
}
