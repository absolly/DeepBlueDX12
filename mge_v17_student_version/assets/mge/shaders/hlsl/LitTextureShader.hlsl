#define NUM_DIR_LIGHTS 1
#include "LightingUtil.hlsl"

struct VS_OUTPUT{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITION;
	float2 uv : TEXCOORD;
	float3 normalW : NORMAL;
	float3 tangentW : TANGENT;
};

cbuffer ConstantBuffer : register(b1)
{
	float3 eyePosW;
	float tiling;
	Light light[MaxLights];
}

Texture2D _MainTex : register(t0);
Texture2D _NormalTex : register(t1);

SamplerState _SampleState :register(s0);

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW){
	//[0,1] -> [-1,1]
	float3 normalT = 2.0f * normalMapSample - 1.0f;
	normalT *= 0.25f;
	//build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW - dot(tangentW, N)* N);
	float3 B = cross(N,T);

	float3x3 TBN = float3x3(T,B,N);

	//transform from tangent to world space
	return mul(normalT, TBN);
}

float4 main(VS_OUTPUT i) : SV_TARGET
{
	//return float4(abs(light[0].Direction),0);
	float4 diffuseAlbedo = float4(_MainTex.Sample(_SampleState, i.uv * tiling).rgb,1);
	float roughness = 0.5f;
	
	i.normalW = normalize(i.normalW);

	float4 normalMapSample = _NormalTex.Sample(_SampleState, i.uv * tiling * 2);
	//normalMapSample.y = -normalMapSample.y;
	float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample.rgb, i.normalW, i.tangentW);
	float3 toEyeW = normalize(eyePosW - i.worldPosition);

	float4 ambient = float4(.3,.3,.3,0) * diffuseAlbedo;
	//return float4(i.normalW,1);
	Material mat = {diffuseAlbedo, float3(0.05,0.05,0.05), .5};
	float4 directlight = ComputeLighting(light, mat, i.worldPosition, bumpedNormalW, toEyeW, .5f);
	float4 litcolor = ambient + directlight;

	litcolor.a = diffuseAlbedo.a;
	return litcolor;
}