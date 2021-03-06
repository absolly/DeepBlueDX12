#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D renderedTexture;
uniform sampler2D bloomTexture;
uniform sampler2D depthTexture;
uniform sampler2D fogTexture;
float FogDensity = 0.3;
vec3 fogColor = vec3(77/255.0f,190/255.0f,1);


float fogFactorExp(
  const float dist,
  const float density
) {
  return 1.0 - clamp(exp(-density * dist), 0.0, 1.0);
}

void main(){
	 const float gamma = 2.2;
	 const float exposure = 1;
    vec3 hdrColor = texture(renderedTexture, UV).rgb;
	vec3 bloomColor = texture(bloomTexture, UV).rgb;
    hdrColor += bloomColor; // additive blending
    // Exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));

	//float fogFactor = 1.0 /exp( (texture( depthTexture, UV).x * FogDensity) * (texture( depthTexture, UV).x * FogDensity) );
	//float fogFactor = (1 - texture( depthTexture, UV).x)/(1.0 - .9);
	//fogFactor = clamp( fogFactor, 0.0, 1.0);

	float fogFactor = fogFactorExp((texture( depthTexture, UV).x-.997)*1000, FogDensity);
	color = vec4(mix(mapped, texture(fogTexture, vec2(fogFactor,0)).xyz, fogFactor),1);
}