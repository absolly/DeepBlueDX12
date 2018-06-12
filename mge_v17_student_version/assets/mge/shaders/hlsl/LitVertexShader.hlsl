struct VS_INPUT{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VS_OUTPUT{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4 wvpMat;
}

VS_OUTPUT main(VS_INPUT i)
{
    // just pass vertex position straight through
	VS_OUTPUT o;
	o.position = mul(float4(i.pos,1.0f),wvpMat);
	o.uv = i.uv;
	o.normal = i.normal;
	o.tangent = i.tangent;
	o.bitangent = i.bitangent;
	return o;
}