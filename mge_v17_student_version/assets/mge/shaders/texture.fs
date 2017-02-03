//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D   textureDiffuse;
uniform sampler2D   textureSpecular;
uniform sampler2D   textureNormal;
uniform vec3        lightPosition[24];
uniform vec3        lightColor[24];
uniform float       lightIntensity[24];
uniform int         lightType[24];
uniform vec3        lightFalloff[24];
uniform int         lightAttenuation[24];
uniform vec3        lightDirection[24];
uniform int         lightCount;
uniform int         tiling;
uniform int         specularMultiplier;

in vec2 texCoord;
in vec3 Position_worldspace;
in vec3 LightDirection_tangentspace[24];//
in vec3 EyeDirection_tangentspace;
in mat3 TBN;
out vec4 fragment_color;

vec3 FragNormal_tangentspace;

vec3 calcPointLight(float pFalloff, vec3 pLightColor, vec3 pMaterialAmbientColor, vec3 pMaterialDiffuseColor, vec3 pMaterialSpecularColor, vec3 pLightDirection_tangentspace ) {

    // Normal of the computed fragment, in camera space
    vec3 n = normalize( FragNormal_tangentspace );
    // Direction of the light (from the fragment to the light)
    vec3 l = normalize( pLightDirection_tangentspace );

    float cosTheta = clamp( dot( n,l ), 0,1 );

    // Eye vector (towards the camera)
    vec3 E = normalize(EyeDirection_tangentspace);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l,n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp( dot( E,R ), 0,1 );

    return pFalloff *(pMaterialAmbientColor +
            // Diffuse : "color" of the object
             pMaterialDiffuseColor * pLightColor * cosTheta
            // Specular : reflective highlight, like a mirror
            + pMaterialSpecularColor * pLightColor * pow(cosAlpha,50));
}

vec3 calcDirectionalLight(vec3 pLightColor, float pLightIntensity, vec3 pMaterialAmbientColor, vec3 pMaterialDiffuseColor, vec3 pMaterialSpecularColor, vec3 pLightDirection_tangentspace ) {

    // Normal of the computed fragment, in camera space
    vec3 n = normalize( FragNormal_tangentspace );
    // Direction of the light (from the fragment to the light)
    vec3 l = normalize( pLightDirection_tangentspace );

    float cosTheta = clamp( dot( n,l ), 0,1 );

    // Eye vector (towards the camera)
    vec3 E = normalize(EyeDirection_tangentspace);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l,n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp( dot( E,R ), 0,1 );

    return pMaterialAmbientColor +
           // Diffuse : "color" of the object
           pMaterialDiffuseColor * pLightColor * pLightIntensity * cosTheta
           // Specular : reflective highlight, like a mirror
           + pMaterialSpecularColor * pLightColor * pLightIntensity * pow(cosAlpha,50) ;
}

void main( void ) {
    FragNormal_tangentspace = vec3(texture(textureNormal,texCoord * tiling));

    vec3 MaterialDiffuseColor = vec3(texture(textureDiffuse,texCoord * tiling));
    vec3 MaterialSpecularColor = vec3(texture(textureSpecular,texCoord * tiling));

    vec3 combinedColor;
    for(int activeLight = 0; activeLight < lightCount; activeLight++) {
        vec3 MaterialAmbientColor = vec3(0.3,0.3,0.3) *lightColor[activeLight] * MaterialDiffuseColor;
        vec3 MaterialSpecularColor = MaterialSpecularColor * specularMultiplier * vec3(.5,.5,.5);

        float distance = length(Position_worldspace - lightPosition[activeLight]);

        float falloff = lightIntensity[activeLight]/(lightFalloff[activeLight].x + lightFalloff[activeLight].y * distance + lightFalloff[activeLight].z * distance * distance);
        switch(lightType[activeLight]) {
        case 0:
            combinedColor += calcPointLight(falloff, lightColor[activeLight], MaterialAmbientColor, MaterialDiffuseColor, MaterialSpecularColor, LightDirection_tangentspace[activeLight]);
            break;
        case 1:
            combinedColor += calcDirectionalLight(lightColor[activeLight], lightIntensity[activeLight], MaterialAmbientColor, MaterialDiffuseColor, MaterialSpecularColor, LightDirection_tangentspace[activeLight]);
            break;
//        case  2:
//            if(cosTheta > 0.0) {
//                float spotEffect = dot(normalize(-lightDirection[activeLight]), normalize((Position_worldspace - lightPosition[activeLight])));
//                float multiplier;
//                if (spotEffect < .8)
//                    multiplier=0;
//
//                multiplier = pow((spotEffect - 0.8) * 10, 2);
//
//                if (spotEffect >0.9)
//                    multiplier=1;
//
//                if (spotEffect > .8) {
//
//
//                    //spotEffect = 0;
//
//
//                    // att = spotEffect * lightIntensity[activeLight] / (lightAttenuation[activeLight].x +
//                    //    lightAttenuation[activeLight].y * distance +
//                    // lightAttenuation[activeLight].z * distance * distance);
//
//                    color =
//                        //MaterialDiffuseColor;
//                        // Ambient : simulates indirect lighting
//                        MaterialAmbientColor +
//                        // Diffuse : "color" of the object
//                        multiplier * MaterialDiffuseColor * lightColor[activeLight] * lightIntensity[activeLight] * cosTheta / falloff
//                        // Specular : reflective highlight, like a mirror
//                        + multiplier * MaterialSpecularColor * lightColor[activeLight] * lightIntensity[activeLight] * pow(cosAlpha,50) / falloff;
//
//                    fragment_color = vec4(color,1);
//                }
//
//            } else {
//                fragment_color = vec4(0,0,0,1);
//
//            }
//            break;

        }
    }
    fragment_color = vec4(combinedColor,1);
}
