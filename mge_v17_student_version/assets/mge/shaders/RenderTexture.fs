#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D renderedTexture;
uniform sampler2D bloomTexture;
uniform sampler2D depthTexture;
uniform sampler2D fogTexture;
uniform sampler2D waterMaskTexture;
uniform float _time;
uniform float pitch;
float FogDensity = 200;
vec3 fogColor = vec3(77/255.0f,190/255.0f,1);


float fogFactorExp(
  const float dist,
  const float density
) {
  return 0.999 - clamp(exp(-density * dist), 0.0, 1.0);
}

float sigmoid(float a, float f)
{
	return 1.0/(1.0+exp(-f*a));
}

void main(){

	//float depth = log(texture( depthTexture, UV).x*1 + 1) / log(texture( depthTexture, UV).x*100000 + 1);
	//float depth = (exp(texture( depthTexture, UV).x*log(1*1000+1)) - 1)/1;
	float f=100000.0;
	float n = 0.5;
	float depth = clamp((2 * n) / (f + n - texture( depthTexture, UV).x * (f - n)),0,1);

//	float edgeStrength = length(fwidth(texture(waterMaskTexture, UV)));
	
//	vec3 color2 = vec3(0,0,0);

//	vec2 texelSize = 1.0 / vec2(1600,900);
//    vec2 texelSize05 = texelSize * 0.5;
    
//    vec2 uvOffset = texelSize.xy * vec2( 10,10) + texelSize05;
//    vec2 texCoordSample;
//	texCoordSample.x = UV.x ;
//    texCoordSample.y = UV.y + uvOffset.y;

//    bool edge = false;
//	if((texture( depthTexture, texCoordSample ).x - texture( waterMaskTexture, texCoordSample ).x) <= 0.00001f)
//		edge = true;

//	edgeStrength = sigmoid(edgeStrength - 0.2, 32.0); 
	//float fogFactor = 1.0 /exp( (texture( depthTexture, UV).x * FogDensity) * (texture( depthTexture, UV).x * FogDensity) );
	//float fogFactor = (1 - texture( depthTexture, UV).x)/(1.0 - .9);
	//fogFactor = clamp( fogFactor, 0.0, 1.0);
	float distortion=(cos(UV.x*200 + _time * 10000) + sin(UV.y*200 + _time * 10000))*(0.1f * depth + 0.001);
	if(texture(waterMaskTexture, UV).r >0 && texture(waterMaskTexture, vec2(UV.x+distortion, UV.y)).r >0){
		
		const float gamma = 2.2;
		const float exposure = 1;
		vec3 hdrColor = texture(renderedTexture, vec2(UV.x+distortion, UV.y)).rgb;
		vec3 bloomColor = texture(bloomTexture, vec2(UV.x+distortion, UV.y)).rgb;
		hdrColor += bloomColor; // additive blending
		// Exposure tone mapping
		vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
		// Gamma correction 
		mapped = pow(mapped, vec3(1.0 / gamma));
		float fogFactor = fogFactorExp(depth, FogDensity);
		color = vec4(mix(mapped, texture(fogTexture, vec2(fogFactor,0)).xyz, fogFactor),1);
	}
		
	else{
		const float gamma = 2.2;
		const float exposure = 1;
		vec3 hdrColor = texture(renderedTexture, UV).rgb;
		vec3 bloomColor = texture(bloomTexture, UV).rgb;
		hdrColor += bloomColor; // additive blending
		// Exposure tone mapping
		vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
		// Gamma correction 
		mapped = pow(mapped, vec3(1.0 / gamma));
//		if(edge){
//			mapped += vec3(1,1,1);
//		}
		float fogFactor = fogFactorExp(depth, FogDensity);
		color = vec4(mix(mapped, texture(fogTexture, vec2(fogFactor,0)).xyz, fogFactor),1);
	}
}