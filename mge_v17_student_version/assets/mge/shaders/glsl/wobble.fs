//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D   textureDiffuse;
uniform vec3        LightPosition = vec3(10,150,10);

in vec2 texCoord;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 EyeDirection_cameraspace;

out vec4 fragment_color;

void main( void ) {


    // Normal of the computed fragment, in camera space
    vec3 n = normalize( Normal_cameraspace );
    // Direction of the light (from the fragment to the light)
    vec3 l = normalize( LightDirection_cameraspace );

    float cosTheta = clamp( dot( n,l ), 0,1 );



    // Eye vector (towards the camera)
    vec3 E = normalize(EyeDirection_cameraspace);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l,n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp( dot( E,R ), 0,1 );

    vec3 MaterialDiffuseColor = vec3(texture(textureDiffuse,texCoord));
    vec3 MaterialAmbientColor = vec3(0.2,0.2,0.2) * MaterialDiffuseColor;
    vec3 LightColor = vec3(1,1,1);
    vec3 MaterialSpecularColor = vec3(.3,.3,.3);
    float distance = length(Position_worldspace - LightPosition);
    float LightPower = 3000;


	vec3 color =
	//MaterialDiffuseColor;
    // Ambient : simulates indirect lighting
    MaterialAmbientColor +
    // Diffuse : "color" of the object
    MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance)
    // Specular : reflective highlight, like a mirror
    + MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,50) / (distance*distance);

	fragment_color = vec4(color,1);
}
