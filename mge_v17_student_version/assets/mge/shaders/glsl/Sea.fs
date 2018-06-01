//DIFFUSE COLOR FRAGMENT SHADER

#version 330 core// for glsl version (12 is for older versions , say opengl 2.1

uniform vec3 diffuseColor;
uniform sampler2D normalTexture;
uniform float _time;

layout (location = 0) out vec4 fragment_color;
layout (location = 1) out vec4 brightness_color;
layout (location = 2) out vec4 watermask_color;

in vec2 UV;

float blendOverlay(float base, float blend) {
	return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend));
}
vec3 blendOverlay(vec3 base, vec3 blend) {
	return vec3(blendOverlay(base.r,blend.r),blendOverlay(base.g,blend.g),blendOverlay(base.b,blend.b));
}

void main( void ) {
	vec3 a = texture(normalTexture, vec2((UV.x *0.02) + (_time * 0.0002f), (UV.y *0.02) + (_time * 0.0004f))).xyz;
	vec3 b = texture(normalTexture, vec2((UV.x *0.02) + (_time * -0.0003f), (UV.y *0.02) + (_time * -0.0003f))).xyz;
	float dotvalue = dot(blendOverlay(a,b), vec3(0.1,0.1,1));
	if(dotvalue < 1.2 && dotvalue > -1.2)
		fragment_color = vec4(1) * (((dotvalue * dotvalue) -1));
	else
		fragment_color = vec4(0,0,0,0);
	brightness_color = vec4(0,0,0,1);
	watermask_color = vec4(gl_FragCoord.z,0,0,1);
}
