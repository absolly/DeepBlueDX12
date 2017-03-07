#version 330 // for glsl version (12 is for older versions , say opengl 2.1

in layout(location=0) vec3 position;
in layout(location=2) mat4 fullTransformMatrix;

void main( void ){
	gl_Position = fullTransformMatrix * vec4(position, 1.0);
}
