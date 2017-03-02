//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D   splatMap;
uniform sampler2D   textureDiffuse1;
uniform sampler2D   textureDiffuse2;
uniform sampler2D   textureDiffuse3;
uniform sampler2D   textureDiffuse4;

uniform vec3        lightPosition[128];
uniform vec3        lightColor[128];
uniform float       lightIntensity[128];
uniform int         activeLight;
uniform int         lightType[128];
uniform int         lightFalloff[128];
uniform int         lightAttenuation[128];
uniform vec3         lightDirection[128];

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

    vec4 SplatMapColor = texture(splatMap, texCoord);
    vec3 DiffuseColor1 = vec3(texture(textureDiffuse1,texCoord));
    vec3 DiffuseColor2 = vec3(texture(textureDiffuse2,texCoord));
    vec3 DiffuseColor3 = vec3(texture(textureDiffuse3,texCoord));
    vec3 DiffuseColor4 = vec3(texture(textureDiffuse4,texCoord));

    vec3 MaterialDiffuseColor = SplatMapColor.r * DiffuseColor1 + SplatMapColor.g * DiffuseColor2 + SplatMapColor.b * DiffuseColor3 + SplatMapColor.a * DiffuseColor4;
    vec3 MaterialAmbientColor = vec3(0.2,0.2,0.2) * MaterialDiffuseColor;

    vec3 MaterialSpecularColor = vec3(.3,.3,.3);

    float distance = length(Position_worldspace - lightPosition[activeLight]);
    vec3 color;

    float falloff;

    switch(lightFalloff[activeLight]) {
    //constant falloff
    case 0:
        falloff = 0;
        break;

    //linear falloff
    case 1:
        falloff = distance;
        break;
    //quadratic falloff
    case 2:
        falloff = distance*distance;
        break;
    }

    switch(lightType[activeLight]) {
    case 0:
        color =
            //MaterialDiffuseColor;
            // Ambient : simulates indirect lighting
            MaterialAmbientColor +
            // Diffuse : "color" of the object
            MaterialDiffuseColor * lightColor[activeLight] * lightIntensity[activeLight] * cosTheta / (1 + falloff)
            // Specular : reflective highlight, like a mirror
            + MaterialSpecularColor * lightColor[activeLight] * lightIntensity[activeLight] * pow(cosAlpha,50) / (1 + falloff);

        fragment_color = vec4(color,1);
        break;

    case 1:
        color =
            //MaterialDiffuseColor;
            // Ambient : simulates indirect lighting
            MaterialAmbientColor +
            // Diffuse : "color" of the object
            MaterialDiffuseColor * lightColor[activeLight] * lightIntensity[activeLight] * cosTheta
            // Specular : reflective highlight, like a mirror
            + MaterialSpecularColor * lightColor[activeLight] * lightIntensity[activeLight] * pow(cosAlpha,50) ;

        fragment_color = vec4(color,1);
        break;
    case  2:
        if(cosTheta > 0.0) {
            float spotEffect = dot(normalize(-lightDirection[activeLight]), normalize((Position_worldspace - lightPosition[activeLight])));
            if (spotEffect > .99) {
            spotEffect = pow(spotEffect, 2);
            //att = spotEffect / (lightAttenuation[activeLight].x +
               //     lightAttenuation[activeLight].y * distance +
                 //   lightAttenuation[activeLight].z * distance * distance);
                color =
                    //MaterialDiffuseColor;
                    // Ambient : simulates indirect lighting
                    MaterialAmbientColor +
                    // Diffuse : "color" of the object
                    MaterialDiffuseColor * lightColor[activeLight] * lightIntensity[activeLight] * cosTheta / falloff
                    // Specular : reflective highlight, like a mirror
                    + MaterialSpecularColor * lightColor[activeLight] * lightIntensity[activeLight] * pow(cosAlpha,50) / falloff;

                fragment_color = vec4(color,1);
            }

        } else {
            fragment_color = vec4(0,0,0,1);

        }
        break;

    }
}
