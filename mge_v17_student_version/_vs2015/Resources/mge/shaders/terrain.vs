//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
uniform sampler2D heightMap;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ){
	
	vec4 heightMapColor = texture(heightMap,uv);
	vec3 vertexPosition = vec3(vertex.x, vertex.y + heightMapColor.r , vertex.z);

	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.f);

	texCoord = uv;
}