struct VS_INPUT{
	float3 pos : POSITION;
	float2 uv : TEXCOORD;
	float3 normalU : NORMAL;
	float3 tangentU : TANGENT;
};

struct VS_OUTPUT{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float2 uv : TEXCOORD;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
};

cbuffer ConstantBuffer : register(b0)
{
	float4x4 wvpMat;
	float4x4 wMat;
	float4x4 vMat;
	float4x4 pMat;
}

VS_OUTPUT main(VS_INPUT i)
{
    // just pass vertex position straight through
	VS_OUTPUT o;
	o.worldPosition = mul(float4(i.pos,1.0f),wMat);
	o.position = mul(float4(i.pos,1.0f),wvpMat);

	o.uv = i.uv;
	o.normalW = mul(i.normalU,(float3x3)wMat);
	o.tangentW = mul(i.tangentU,(float3x3)wMat);
	return o;
}