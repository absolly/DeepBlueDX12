#version 330 core
out vec4 FragColor;
in vec2 UV;
uniform int iteration;

uniform sampler2D brightnessTexture;

vec4 blur5(sampler2D image, vec2 uv, vec2 resolution, vec2 direction) {
  vec4 color = vec4(0.0);
  vec2 off1 = vec2(1.411764705882353) * direction;
  vec2 off2 = vec2(3.2941176470588234) * direction;
  vec2 off3 = vec2(5.176470588235294) * direction;
  color += texture2D(image, uv) * 0.1964825501511404;
  color += texture2D(image, uv + (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv - (off1 / resolution)) * 0.2969069646728344;
  color += texture2D(image, uv + (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv - (off2 / resolution)) * 0.09447039785044732;
  color += texture2D(image, uv + (off3 / resolution)) * 0.010381362401148057;
  color += texture2D(image, uv - (off3 / resolution)) * 0.010381362401148057;
  return color;
}

vec4 kawaseBloom( sampler2D texture, vec2 uv, vec2 texturesize, float iteration ) {
    vec2 texelSize = 1.0 / texturesize;
    vec2 texelSize05 = texelSize * 0.5;
    
    vec2 uvOffset = texelSize.xy * vec2( iteration) + texelSize05;
    
    vec2 texCoordSample;
    vec4 color;
    
    texCoordSample.x = uv.x - uvOffset.x;
    texCoordSample.y = uv.y + uvOffset.y;
    color = texture2D( texture, texCoordSample );

    texCoordSample.x = uv.x + uvOffset.x;
    texCoordSample.y = uv.y + uvOffset.y;
    color += texture2D( texture, texCoordSample );
    
    texCoordSample.x = uv.x + uvOffset.x;
    texCoordSample.y = uv.y - uvOffset.y;
    color += texture2D( texture, texCoordSample );
    
    texCoordSample.x = uv.x - uvOffset.x;
    texCoordSample.y = uv.y - uvOffset.y;
    color += texture2D( texture, texCoordSample );
    
    return color * 0.25;
}

void main()
{             
	//vec2 direction = horizontal?vec2(1,0):vec2(0,1);
	FragColor = kawaseBloom(brightnessTexture, UV, textureSize(brightnessTexture, 0), iteration/3.0f);
}
