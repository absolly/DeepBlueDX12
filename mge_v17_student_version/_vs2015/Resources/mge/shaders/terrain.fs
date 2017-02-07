//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D textureDiffuse0;
uniform sampler2D textureDiffuse1;
uniform sampler2D textureDiffuse2;
uniform sampler2D textureDiffuse3;
uniform sampler2D splatmapTexture;

in vec2 texCoord;
out vec4 fragment_color;

void main( void ) {
	vec4 diffuseRColor = texture(textureDiffuse0,texCoord);
	vec4 diffuseGColor = texture(textureDiffuse1,texCoord);
	vec4 diffuseBColor = texture(textureDiffuse2,texCoord);
	vec4 diffuseAColor = texture(textureDiffuse3,texCoord);
	
    vec4 splatmapColor = texture(splatmapTexture,texCoord);;
	
	diffuseRColor *= splatmapColor.r;
	diffuseGColor *= splatmapColor.g;
	diffuseBColor *= splatmapColor.b;
	diffuseAColor *= splatmapColor.a;
	
	
	
	
	
	fragment_color = diffuseRColor+diffuseGColor+diffuseBColor+diffuseAColor;
}
