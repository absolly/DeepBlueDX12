#version 330 core

in vec2 UV;

// Ouput data
layout (location = 0) out vec4 color;
layout (location = 1) out vec4 brightness_color;
layout (location = 2) out vec4 watermask_color;

uniform sampler2D myTextureSampler;

void main()
{
    color = texture( myTextureSampler, UV );
	
	if (color.a < 0.2)
	{
		discard;
	}

	brightness_color = vec4(0,0,0,1);
	watermask_color = vec4(0,0,0,1);
}