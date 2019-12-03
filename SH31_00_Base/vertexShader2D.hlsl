cbuffer ConstantBuffer:register(b0)
{
	matrix Projection;
	float4 value;
}

struct VS_IN
{
	float4 pos : POSITION0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct VS_OUT {
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

VS_OUT main(VS_IN input)
{
	VS_OUT output;
	output.pos = mul(input.pos, Projection);
	//output.pos.x += sin(output.pos.y + value.r) * 0.5;
	output.color = input.color;
	output.texcoord = input.texcoord;

	return output;
}
