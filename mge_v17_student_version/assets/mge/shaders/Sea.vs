//DIFFUSE COLOR VERTEX SHADER

#version 330 core// for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertex;
in vec3 normal;
in vec2 uv;
uniform float _time;

uniform	mat4 	mvpMatrix;
out float distance;
void main( void ){
	vec3 newvertex = vec3(vertex.x, 1 * (sin((_time*0.002) + (vertex.x * 99)) + sin(((_time)*0.002) + (vertex.z * 99))) + vertex.y, vertex.z);
	gl_Position = mvpMatrix * vec4(newvertex, 1.f);
	distance = (mvpMatrix * vec4(newvertex, 1.f)).z;
}
