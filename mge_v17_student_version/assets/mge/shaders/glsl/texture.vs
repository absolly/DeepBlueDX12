//DIFFUSE TEXTURE VERTEX SHADER
#version 330 core// for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;

in vec3 vertex;
in vec3 normal;
in vec2 uv;
in vec3 tangent;
in vec3 bitangent;

out vec3 Position_worldspace;
out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 normalW;
out vec3 tangentW;

void main( void ) {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix  * vec4(vertex, 1);
    texCoord = uv;

    // Position of the vertex, in worldspace : modelMatrix * position
    Position_worldspace = (modelMatrix * vec4(vertex,1)).xyz;

	normalW = (modelMatrix * vec4(normal,0)).xyz;
	tangentW = (modelMatrix * vec4(tangent,0)).xyz;


}
