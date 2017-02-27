//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 core// for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D   textureDiffuse;
uniform sampler2D   textureSpecular;
uniform sampler2D   textureNormal;
uniform sampler2D	shadowMap;
uniform vec3        lightPosition[5];
uniform vec3        lightColor[5];
uniform float       lightIntensity[5];
uniform int         lightType[5];
uniform vec3        lightFalloff[5];
uniform int         lightCount;
uniform int         tiling;
uniform int         specularMultiplier;

in vec2 texCoord;
in vec4 ShadowCoord;
in vec3 Position_worldspace;
in vec3 LightDirection_tangentspace[5];
in vec3 EyeDirection_tangentspace;

layout (location = 0) out vec4 fragment_color;
layout (location = 1) out vec4 brightness_color;

vec3 FragNormal_tangentspace;
vec3 n;
vec3 l;
vec3 E;
vec3 R;
float cosTheta;
float cosAlpha;
float visibility = 1.0;
float bias = 0.005;//use if shadow acne is a problem

vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

vec3 calcPointLight(float pFalloff, vec3 pLightColor, vec3 pMaterialAmbientColor, vec3 pMaterialDiffuseColor, vec3 pMaterialSpecularColor, vec3 pLightDirection_tangentspace ) {

    // Normal of the computed fragment, in camera space
    n = normalize( FragNormal_tangentspace );
    // Direction of the light (from the fragment to the light)
    l = normalize( pLightDirection_tangentspace );

    cosTheta = clamp( dot( n,l ), 0,1 );

    // Eye vector (towards the camera)
    E = normalize(EyeDirection_tangentspace);
    // Direction in which the triangle reflects the light
    R = reflect(-l,n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    cosAlpha = clamp( dot( E,R ), 0,1 );

    return pFalloff *(pMaterialAmbientColor +
            // Diffuse : "color" of the object
             visibility * pMaterialDiffuseColor * pLightColor * cosTheta
            // Specular : reflective highlight, like a mirror
            +  visibility * pMaterialSpecularColor * pLightColor * pow(cosAlpha,50));
}

vec3 calcDirectionalLight(vec3 pLightColor, float pLightIntensity, vec3 pMaterialAmbientColor, vec3 pMaterialDiffuseColor, vec3 pMaterialSpecularColor, vec3 pLightDirection_tangentspace ) {

    // Normal of the computed fragment, in camera space
    n = normalize( FragNormal_tangentspace );
    // Direction of the light (from the fragment to the light)
    l = normalize( pLightDirection_tangentspace );

    cosTheta = clamp( dot( n,l ), 0,1 );

    // Eye vector (towards the camera)
    E = normalize(EyeDirection_tangentspace);
    // Direction in which the triangle reflects the light
    R = reflect(-l,n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    cosAlpha = clamp( dot( E,R ), 0,1 );

    return pMaterialAmbientColor +
           // Diffuse : "color" of the object
           visibility * pMaterialDiffuseColor * pLightColor * pLightIntensity * cosTheta
           // Specular : reflective highlight, like a mirror
           +  visibility * pMaterialSpecularColor * pLightColor * pLightIntensity * pow(cosAlpha,50) ;
}

void main( void ) {
    FragNormal_tangentspace = vec3(texture(textureNormal,texCoord * tiling));

    vec3 MaterialDiffuseColor = vec3(texture(textureDiffuse,texCoord * tiling));
    vec3 MaterialSpecularColor = vec3(texture(textureSpecular,texCoord * tiling));

    vec3 combinedColor;
	vec3 MaterialAmbientColor;
	float distance;
	float falloff;



	for (int i=0;i<4;i++){
		if ( texture( shadowMap, ShadowCoord.xy + poissonDisk[i]/700.0 ).z  <  ShadowCoord.z ){
			visibility-=0.2;
		}
	}

    for(int activeLight = 0; activeLight < lightCount; activeLight++) {
        MaterialAmbientColor = vec3(0.1,0.1,0.1) *lightColor[activeLight] * MaterialDiffuseColor;
        MaterialSpecularColor = MaterialSpecularColor * specularMultiplier * vec3(.5,.5,.5);

        distance = length(Position_worldspace - lightPosition[activeLight]);

        falloff = lightIntensity[activeLight]/(lightFalloff[activeLight].x + lightFalloff[activeLight].y * distance + lightFalloff[activeLight].z * distance * distance);
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
	float brightness = dot(fragment_color.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1)
        brightness_color = vec4(fragment_color.rgb, 1.0);
	else
		brightness_color = vec4(0,0,0,1);
}
