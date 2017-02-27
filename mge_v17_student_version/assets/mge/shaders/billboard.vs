#version 330 // for glsl version (12 is for older versions , say opengl 2.1

layout(location = 0) in vec3 squareVertices;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;
uniform mat4 VP;
uniform vec3 BillboardPos;
uniform vec2 BillboardSize;

void main( void ){

	vec3 particleCenter_wordspace = BillboardPos;
	
	vec3 vertexPosition_worldspace = 
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * BillboardSize.x
		+ CameraUp_worldspace * squareVertices.y * BillboardSize.y;
		
	gl_Position = VP * vec4(vertexPosition_worldspace, 1.0f);
}
