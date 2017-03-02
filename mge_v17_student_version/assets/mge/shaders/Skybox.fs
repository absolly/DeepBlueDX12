#version 330 core
in vec3 TexCoords;
layout (location = 0) out vec4 fragment_color;
layout (location = 1) out vec4 brightness_color;
layout (location = 2) out vec4 watermask_color;
uniform samplerCube skybox;

void main()
{    
    fragment_color = texture(skybox, TexCoords);
	watermask_color = vec4(0,0,0,1);
}
  