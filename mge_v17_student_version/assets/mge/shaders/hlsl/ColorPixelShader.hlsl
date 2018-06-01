struct VS_OUTPUT{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};


float4 main(VS_OUTPUT i) : SV_TARGET
{
	return float4(i.uv, 1.0f, 1.0f);
}