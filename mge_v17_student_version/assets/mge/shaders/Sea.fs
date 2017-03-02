//DIFFUSE COLOR FRAGMENT SHADER

#version 330 core// for glsl version (12 is for older versions , say opengl 2.1

uniform vec3 diffuseColor;
layout (location = 0) out vec4 fragment_color;
layout (location = 1) out vec4 brightness_color;
layout (location = 2) out vec4 watermask_color;

in float distance;
void main( void ) {
	fragment_color = vec4(.05,.1,0.2,0.1);
	watermask_color = vec4(gl_FragCoord.z,0,0,1);
}
