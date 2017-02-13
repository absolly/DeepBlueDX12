#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D renderedTexture;
uniform sampler2D depthTexture;

void main(){
	color = texture( renderedTexture, UV).xyz * (1.5 - pow( texture( depthTexture, UV).x, 50)) ;
}