#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D renderedTexture;
uniform sampler2D bloomTexture;
uniform sampler2D depthTexture;
float FogDensity = 0.1f;
vec3 fogColor = vec3(.3,.6,1);


float fogFactorExp(
  const float dist,
  const float density
) {
  return 1.0 - clamp(exp(-density * dist), 0.0, 1.0);
}

void main(){
	 const float gamma = 2.2;
	 const float exposure = .1;
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

	float fogFactor = fogFactorExp((texture( depthTexture, UV).x-.997)*1000 , FogDensity);
	color = vec4(mix(mapped, fogColor, fogFactor),1);
}