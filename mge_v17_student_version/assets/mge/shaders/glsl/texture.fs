//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 core// for glsl version (12 is for older versions , say opengl 2.1

#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 0
#define NUM_SPOT_LIGHTS 1

uniform sampler2D   textureDiffuse;
uniform sampler2D   textureSpecular;
uniform sampler2D   textureNormal;
uniform sampler2D	emissionMap;

uniform vec3        eyePosW;
uniform int         tiling;
//light
uniform vec3        lightPosition[2];
uniform float       falloffStart[2];
uniform vec3        lightColor[2];
uniform float       falloffEnd[2];
uniform vec3        lightDirection[2];
uniform float       spotPower[2];
//
uniform int         num_dir_light;
uniform int         num_point_light;
uniform int         num_spot_light;
uniform int         specularMultiplier;

in vec3 Position_worldspace;
in vec2 texCoord;
in vec3 normalW;
in vec3 tangentW;

layout (location = 0) out vec4 fragment_color;

float CalcAttenuation(float d, float falloffStart, float falloffEnd)
{
    // Linear falloff.
    return clamp((falloffEnd-d) / (falloffEnd - falloffStart), 0.0, 1.0);
}

vec3 BlinnPhong(vec3 lightStrength, vec3 lightVec, vec3 normal, vec3 toEye, vec4 diffuseColor, float shininess)
{
	// Eye vector (towards the camera)
    vec3 E = normalize(toEye);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(lightVec,normal);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float cosAlpha = clamp( dot( E,R ), 0,1 );

    return diffuseColor.rgb * lightStrength + shininess * lightStrength * pow(cosAlpha,50);
}

//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for directional lights.
//---------------------------------------------------------------------------------------
vec3 ComputeDirectionalLight(vec3 lightPosition, float falloffStart, vec3 lightColor, float falloffEnd, vec3 lightDirection, float spotPower, vec4 diffuseColor, float shininess, vec3 normal, vec3 toEye)
{
    // The light vector aims opposite the direction the light rays travel.
    vec3 lightVec = normalize(lightDirection);

    // Scale light down by Lambert's cosine law.
    float ndotl = max(dot(normal, lightVec), 0.0f);
    vec3 lightStrength = lightColor * ndotl;
	
    return BlinnPhong(lightStrength, lightVec, normal, toEye, diffuseColor, shininess);
}

//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for point lights.
//---------------------------------------------------------------------------------------
vec3 ComputePointLight(vec3 lightPosition, float falloffStart, vec3 lightColor, float falloffEnd, vec3 lightDirection, float spotPower, vec4 diffuseColor, float shininess, vec3 pos, vec3 normal, vec3 toEye)
{
    // The vector from the surface to the light.
    vec3 lightVec = lightPosition - pos;

    // The distance from surface to light.
    float d = length(lightVec);

    // Range test.
    if(d > 1000)
        return vec3(0.0f);

    // Normalize the light vector.
    lightVec /= d;

    // Scale light down by Lambert's cosine law.
    float ndotl = max(dot(lightVec, normal), 0.0f);
    vec3 lightStrength = lightColor * ndotl;

    // Attenuate light by distance.
    float att = CalcAttenuation(d, 800, 1000); //TODO
    lightStrength *= att;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, diffuseColor, shininess);
}

//---------------------------------------------------------------------------------------
// Evaluates the lighting equation for spot lights.
//---------------------------------------------------------------------------------------
vec3 ComputeSpotLight(vec3 lightPosition, float falloffStart, vec3 lightColor, float falloffEnd, vec3 lightDirection, float spotPower, vec4 diffuseColor, float shininess, vec3 pos, vec3 normal, vec3 toEye)
{
    // The vector from the surface to the light.
    vec3 lightVec = lightPosition - pos;

    // The distance from surface to light.
    float d = length(lightVec);

    // Range test.
    if(d > falloffEnd)
        return vec3(0.0f);

    // Normalize the light vector.
    lightVec /= d;

    // Scale light down by Lambert's cosine law.
    float ndotl = max(dot(lightVec, normal), 0.0f);
    vec3 lightStrength = lightColor * ndotl;

    // Attenuate light by distance.
    float att = CalcAttenuation(d, falloffStart, falloffEnd);
    lightStrength *= att;

    // Scale by spotlight
    float spotFactor = pow(max(dot(-lightVec, lightDirection), 0.0f), spotPower);
    lightStrength *= spotFactor;

    return BlinnPhong(lightStrength, lightVec, normal, toEye, diffuseColor, shininess);
}

vec3 NormalSampleToWorldSpace(vec3 normalMapSample, vec3 unitNormalW, vec3 tangentW){
	//[0,1] -> [-1,1]
	vec3 normalT = 2.0f * normalMapSample - 1.0f;
	normalT *= 0.25f;

	//build orthonormal basis.
	vec3 N = unitNormalW;
	vec3 T = normalize(tangentW - dot(tangentW, N)* N);
	vec3 B = cross(N,T);

	mat3 TBN = mat3(T,B,N);

	//transform from tangent to world space
	return TBN * normalT;
}

void main( void ) {
    vec4 MaterialDiffuseColor = texture(textureDiffuse,texCoord * tiling);

    vec3 normal = normalize(normalW);
    vec3 normalMapSample = texture(textureNormal, texCoord * tiling).rgb;
    vec3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample.rgb, normal, tangentW);

    vec3 toEyeW = normalize(eyePosW - Position_worldspace);
    vec4 ambient = vec4(.1,.1,.1,0) * MaterialDiffuseColor;
    vec3 MaterialSpecularColor = vec3(texture(textureSpecular,texCoord * tiling)) * specularMultiplier;



    vec4 MaterialAmbientColor = vec4(.1,.1,.1,0) * MaterialDiffuseColor;
    vec3 combinedColor;
    //loop through the 3 light types seperately

    for(int i = 0; i < NUM_DIR_LIGHTS; i++){
        combinedColor += 0.5 * ComputeDirectionalLight(lightPosition[i],falloffStart[i],lightColor[i],falloffEnd[i],lightDirection[i],spotPower[i], MaterialDiffuseColor, MaterialSpecularColor.x, bumpedNormalW, toEyeW);
    }
    for(int i = NUM_DIR_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; i++){
        combinedColor += ComputePointLight(lightPosition[i],falloffStart[i],lightColor[i],falloffEnd[i],lightDirection[i],spotPower[i], MaterialDiffuseColor, MaterialSpecularColor.x,Position_worldspace, bumpedNormalW, toEyeW);
    }
    for(int i = NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; i < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; i++){        
        combinedColor += ComputeSpotLight(lightPosition[i],falloffStart[i],lightColor[i],falloffEnd[i],lightDirection[i],spotPower[i], MaterialDiffuseColor, MaterialSpecularColor.x,Position_worldspace, bumpedNormalW, toEyeW);
    }


	vec3 emissionColor = texture(emissionMap, texCoord * tiling).rgb;

    fragment_color = vec4(combinedColor + emissionColor, MaterialDiffuseColor.a);
}
