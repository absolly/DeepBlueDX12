#version 330 core

out vec4 color;
out vec4 brightness_color;

void main()
{
    color = vec4(2.0f, 2.0f, .3f, 1.0f);
	float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        brightness_color = vec4(color.rgb, 1.0);
	else
		brightness_color = vec4(0,0,0,1);
}