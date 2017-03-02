//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 core// for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D   textureDiffuse;
uniform sampler2D   textureSpecular;
uniform sampler2D   textureNormal;
uniform sampler2DShadow shadowMap;
uniform vec3        lightPosition[5];
uniform vec3        lightColor[5];
uniform float       lightIntensity[5];
uniform int         lightType[5];
uniform vec3        lightFalloff[5];
uniform int         lightCount;
uniform int         tiling;
uniform int         specularMultiplier;
uniform bool		depthTest;
in vec2 texCoord;
in vec4 ShadowCoord;
in vec3 Position_worldspace;
in vec3 LightDirection_tangentspace[5];
in vec3 EyeDirection_tangentspace;

layout (location = 0) out vec4 fragment_color;
layout (location = 1) out vec4 brightness_color;
layout (location = 2) out vec4 watermask_color;

vec3 FragNormal_tangentspace;
vec3 n;
vec3 l;
vec3 E;
vec3 R;
float cosTheta;
float cosAlpha;
float bias = 0.005;//use if shadow acne is a problem
float visibility;

vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
); 

// Returns a random number based on a vec3 and an int.
float random(vec3 seed, int i){
	vec4 seed4 = vec4(seed,i);
	float dot_product = dot(seed4, vec4(12.9898,78.233,45.164,94.673));
	return fract(sin(dot_product) * 43758.5453);
}

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



    for(int activeLight = 0; activeLight < lightCount; activeLight++) {
		visibility = 1.0;
        MaterialAmbientColor = vec3(0.01,0.01,0.01) *lightColor[activeLight] * MaterialDiffuseColor;
        MaterialSpecularColor = MaterialSpecularColor * specularMultiplier * vec3(.5,.5,.5);

        distance = length(Position_worldspace - lightPosition[activeLight]);

        falloff = lightIntensity[activeLight]/(lightFalloff[activeLight].x + lightFalloff[activeLight].y * distance + lightFalloff[activeLight].z * distance * distance);
        switch(lightType[activeLight]) {
        case 0:
            combinedColor += calcPointLight(falloff, lightColor[activeLight], MaterialAmbientColor, MaterialDiffuseColor, MaterialSpecularColor, LightDirection_tangentspace[activeLight]);
            break;
        case 1:
			vec3 coord;
			for (int i=0;i<4;i++){
				// use either :
				//  - Always the same samples.
				//    Gives a fixed pattern in the shadow, but no noise
				int index = i;
				//  - A random sample, based on the pixel's screen location. 
				//    No banding, but the shadow moves with the camera, which looks weird.
				// int index = int(16.0*random(gl_FragCoord.xyy, i))%16;
				//  - A random sample, based on the pixel's position in world space.
				//    The position is rounded to the millimeter to avoid too much aliasing
				 //int index = int(16.0*random(floor(Position_worldspace.xyz*80.0), i))%16;
		
				// being fully in the shadow will eat up 4*0.2 = 0.8
				// 0.2 potentially remain, which is quite dark.
				coord = vec3( ShadowCoord.xy + poissonDisk[index]/1200.0,  (ShadowCoord.z)/ShadowCoord.w );
				if(coord.x > 0 && coord.x < 1 && coord.y > 0 && coord.y < 1 && coord.z > 0 && coord.z < 1)
					visibility -= 0.2*(1.0-texture( shadowMap, coord));
			}
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
	
	watermask_color = vec4(0,0,0,1);
	
	depthTest?gl_FragDepth=gl_FragCoord.z:gl_FragDepth=0;
}
