struct VS_OUTPUT{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer ConstantBuffer : register(b1){
	float3 color : COLOR;
}

float4 main(VS_OUTPUT i) : SV_TARGET
{
	return float4(color,1);
}