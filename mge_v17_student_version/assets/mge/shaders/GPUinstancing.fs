#version 330 core

// Ouput data
layout (location = 0) out vec4 color;
layout (location = 1) out vec4 brightness_color;
layout (location = 2) out vec4 watermask_color;

uniform sampler2D myTextureSampler;
in vec2 texCoord;

void main()
{
    color = texture( myTextureSampler, texCoord );
	
	if (color.a < 0.9)
	{
		discard;
	}

	brightness_color = vec4(0,0,0,1);
	watermask_color = vec4(0,0,0,1);
}