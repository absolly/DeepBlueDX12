//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D   textureDiffuse;
uniform vec3        lightPosition[28];
uniform vec3        lightColor[28];
uniform float       lightIntensity[28];
uniform int         lightType[28];
uniform vec3        lightFalloff[28];
uniform int         lightAttenuation[28];
uniform vec3        lightDirection[28];
uniform int         lightCount;
uniform int         tiling;

in vec2 texCoord;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace[28];//
in vec3 EyeDirection_cameraspace;

out vec4 fragment_color;

vec3 calcPointLight(float pFalloff, vec3 pLightColor, vec3 pMaterialAmbientColor, vec3 pMaterialDiffuseColor, vec3 pMaterialSpecularColor, vec3 pLightDirection_cameraspace ) {

    // Normal of the computed fragment, in camera space
    vec3 n = normalize( Normal_cameraspace );
    // Direction of the light (from the fragment to the light)
    vec3 l = normalize( pLightDirection_cameraspace );

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

    return pFalloff *(pMaterialAmbientColor +
            // Diffuse : "color" of the object
             pMaterialDiffuseColor * pLightColor * cosTheta
            // Specular : reflective highlight, like a mirror
            + pMaterialSpecularColor * pLightColor * pow(cosAlpha,50));
}

vec3 calcDirectionalLight(vec3 pLightColor, float pLightIntensity, vec3 pMaterialAmbientColor, vec3 pMaterialDiffuseColor, vec3 pMaterialSpecularColor, vec3 pLightDirection_cameraspace ) {

    // Normal of the computed fragment, in camera space
    vec3 n = normalize( Normal_cameraspace );
    // Direction of the light (from the fragment to the light)
    vec3 l = normalize( pLightDirection_cameraspace );

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

    return pMaterialAmbientColor +
           // Diffuse : "color" of the object
           pMaterialDiffuseColor * pLightColor * pLightIntensity * cosTheta
           // Specular : reflective highlight, like a mirror
           + pMaterialSpecularColor * pLightColor * pLightIntensity * pow(cosAlpha,50) ;
}

void main( void ) {


    vec3 MaterialDiffuseColor = vec3(texture(textureDiffuse,texCoord * tiling));



    vec3 combinedColor;
    for(int activeLight = 0; activeLight < lightCount; activeLight++) {
        vec3 MaterialAmbientColor = vec3(0.3,0.3,0.3) *lightColor[activeLight] * MaterialDiffuseColor;
        vec3 MaterialSpecularColor = vec3(.3,.3,.3);

        float distance = length(Position_worldspace - lightPosition[activeLight]);

        float falloff = lightIntensity[activeLight]/(lightFalloff[activeLight].x + lightFalloff[activeLight].y * distance + lightFalloff[activeLight].z * distance * distance);
        switch(lightType[activeLight]) {
        case 0:
            combinedColor += calcPointLight(falloff, lightColor[activeLight], MaterialAmbientColor, MaterialDiffuseColor, MaterialSpecularColor, LightDirection_cameraspace[activeLight]);
            break;
        case 1:
            combinedColor += calcDirectionalLight(lightColor[activeLight], lightIntensity[activeLight], MaterialAmbientColor, MaterialDiffuseColor, MaterialSpecularColor, LightDirection_cameraspace[activeLight]);
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
