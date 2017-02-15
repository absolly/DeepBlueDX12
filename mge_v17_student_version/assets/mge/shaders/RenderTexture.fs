#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D renderedTexture;
uniform sampler2D depthTexture;
float FogDensity = 1;
vec3 fogColor = vec3(.3,.6,1);


float fogFactorExp(
  const float dist,
  const float density
) {
  return 1.0 - clamp(exp(-density * dist), 0.0, 1.0);
}

void main(){
	//float fogFactor = 1.0 /exp( (texture( depthTexture, UV).x * FogDensity) * (texture( depthTexture, UV).x * FogDensity) );
	//float fogFactor = (1 - texture( depthTexture, UV).x)/(1.0 - .9);
	//fogFactor = clamp( fogFactor, 0.0, 1.0);

	float fogFactor = fogFactorExp((texture( depthTexture, UV).x-.997)*1000 , FogDensity);
	color = vec4(mix(texture( renderedTexture, UV).xyz, fogColor, fogFactor),1);
}