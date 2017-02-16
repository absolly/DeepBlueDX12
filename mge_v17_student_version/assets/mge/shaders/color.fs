//DIFFUSE COLOR FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform vec3 diffuseColor;
layout (location = 0) out vec4 fragment_color;
layout (location = 1) out vec4 brightness_color;

void main( void ) {
	fragment_color = vec4 (diffuseColor,1);
	// Check whether fragment output is higher than threshold, if so output as brightness color
    float brightness = dot(fragment_color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        brightness_color = vec4(fragment_color.rgb, 1.0);
	else
		brightness_color = vec4(0,0,0,1);
}
