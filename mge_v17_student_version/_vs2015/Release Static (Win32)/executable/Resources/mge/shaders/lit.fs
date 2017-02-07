//DIFFUSE COLOR FRAGMENT SHADER

#version 330 // for glsl version (12 is for older versions , say opengl 2.1

//uniform vec3 diffuseColor;
//out vec4 fragment_color;

uniform vec3 globalAmbient;
uniform vec3 diffuseColor;
uniform vec3 directionalLightColor;

in float diffuseIntensity;
out vec4 sColor;

void main( void ) {
	//fragment_color = vec4 (diffuseColor,1);
	
	vec3 ambientTerm = globalAmbient * diffuseColor;
    vec3 diffuseTerm = diffuseIntensity * directionalLightColor * diffuseColor;
    vec3 specularTerm = vec3(0,0,0);

    sColor = vec4 (ambientTerm + diffuseTerm + specularTerm, 1);
}