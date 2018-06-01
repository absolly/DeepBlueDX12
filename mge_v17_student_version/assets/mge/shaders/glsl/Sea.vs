//DIFFUSE COLOR VERTEX SHADER

#version 330 core// for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertex;
in vec3 normal;
in vec2 uv;
uniform float _time;

uniform	mat4 mvpMatrix;
uniform mat4 modelMatrix;
out vec2 UV;

void main( void ){
	//vec3 newvertex = vec3(vertex.x, 1 * (sin((_time*0.002) + (vertex.x * 9999)) + sin(((_time)*0.002) + (vertex.z * 9999))) + vertex.y, vertex.z);
	gl_Position = mvpMatrix * vec4(vertex, 1.f);
	UV = (modelMatrix * vec4(vertex,1)).xz;
}
