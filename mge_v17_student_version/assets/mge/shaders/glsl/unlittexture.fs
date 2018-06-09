//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 core// for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D   textureDiffuse;

uniform float         tiling;

in vec2 texCoord;

layout (location = 0) out vec4 fragment_color;
layout (location = 1) out vec4 brightness_color;
layout (location = 2) out vec4 watermask_color;


void main( void ) {
   fragment_color = texture(textureDiffuse, texCoord * tiling);
}
