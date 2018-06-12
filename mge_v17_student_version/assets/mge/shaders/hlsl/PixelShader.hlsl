struct VS_OUTPUT{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

cbuffer ConstantBuffer : register(b1)
{
	float3 color;
	float tiling;
}

Texture2D _MainTex : register(t0);
SamplerState _SampleState :register(s0);

float4 main(VS_OUTPUT i) : SV_TARGET
{
    // return vertex color
    return float4(_MainTex.Sample(_SampleState, i.uv * tiling).rgb * color,1);
}