//DIFFUSE TEXTURE VERTEX SHADER
#version 330 core// for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
uniform	mat3 	MVMatrix;
uniform mat4	depthBiasMVP;
uniform vec3    lightPosition[5];
uniform vec3    lightDirection[5];
uniform int     lightType[5];
in vec3 vertex;
in vec3 normal;
in vec2 uv;
in vec3 tangent;
in vec3 bitangent;
uniform int         lightCount;


out vec2 texCoord; //make sure the texture coord is interpolated
out vec4 ShadowCoord;
out vec3 Position_worldspace;
mat3 TBN;
out vec3 LightDirection_tangentspace[5];
out vec3 EyeDirection_tangentspace;
vec3 EyeDirection_cameraspace;


void main( void ) {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix  * vec4(vertex, 1);
    texCoord = uv;
	ShadowCoord = depthBiasMVP * vec4(vertex,1);

    vec3 vertexNormal_cameraspace = MVMatrix * normal;
    vec3 vertexTangent_cameraspace = MVMatrix * tangent;
    vec3 vertexBitangent_cameraspace = MVMatrix * bitangent;

    TBN = transpose(mat3(
                        vertexTangent_cameraspace,
                        vertexBitangent_cameraspace,
                        vertexNormal_cameraspace
                    ));

    // Position of the vertex, in worldspace : modelMatrix * position
    Position_worldspace = (modelMatrix * vec4(vertex,1)).xyz;

    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin.
    EyeDirection_cameraspace = vec3(0,0,0) - ( viewMatrix * modelMatrix * vec4(vertex,1)).xyz;
    EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;
	
	vec3 LightPosition_cameraspace;
	vec3 LightDirection_cameraspace;

    for(int activeLight = 0; activeLight < lightCount; activeLight++) {
        // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity).
        LightPosition_cameraspace = ( viewMatrix * vec4(lightPosition[activeLight],1)).xyz;

        switch(lightType[activeLight]) {
        case 0:
            LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
            break;
        case 1:
            LightDirection_cameraspace =  (viewMatrix * vec4(lightDirection[activeLight],0)).xyz;
            break;
        case 2:
            LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
            break;
		}

        LightDirection_tangentspace[activeLight] = TBN * LightDirection_cameraspace;
    }


}
