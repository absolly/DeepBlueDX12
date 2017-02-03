//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
uniform vec3    lightPosition[24];
uniform vec3    lightDirection[24];
uniform int     lightType[24];
in vec3 vertex;
in vec3 normal;
in vec2 uv;
in vec3 tangent;
in vec3 bitangent;
uniform int         lightCount;


out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 Position_worldspace;
mat3 TBN;
out vec3 LightDirection_tangentspace[24];
out vec3 EyeDirection_tangentspace;
vec3 LightDirection_cameraspace[24];
vec3 EyeDirection_cameraspace;


void main( void ) {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex, 1.f);
    texCoord = uv;

    vec3 vertexNormal_cameraspace = (viewMatrix * modelMatrix * vec4(normalize(normal),0)).xyz;
    vec3 vertexTangent_cameraspace = (viewMatrix * modelMatrix * vec4(normalize(tangent),0)).xyz;
    vec3 vertexBitangent_cameraspace = (viewMatrix * modelMatrix * vec4(normalize(bitangent),0)).xyz;

    TBN = transpose(mat3(
                        vertexTangent_cameraspace,
                        vertexBitangent_cameraspace,
                        vertexNormal_cameraspace
                    ));

    // Position of the vertex, in worldspace : modelMatrix * position
    Position_worldspace = (modelMatrix * vec4(vertex,1)).xyz;

    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin.
    vec3 vertexPosition_cameraspace = ( viewMatrix * modelMatrix * vec4(vertex,1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
    EyeDirection_tangentspace =  TBN * EyeDirection_cameraspace;

    for(int activeLight = 0; activeLight < lightCount; activeLight++) {
        // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity).
        vec3 LightPosition_cameraspace = ( viewMatrix * vec4(lightPosition[activeLight],1)).xyz;
        //LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

        switch(lightType[activeLight]) {
        case 0:
            LightDirection_cameraspace[activeLight] = LightPosition_cameraspace + EyeDirection_cameraspace;
            break;
        case 1:
            LightDirection_cameraspace[activeLight] =  (viewMatrix * vec4(lightDirection[activeLight],0)).xyz;
            break;
        case 2:
            LightDirection_cameraspace[activeLight] = LightPosition_cameraspace + EyeDirection_cameraspace;
            break;
        }

        LightDirection_tangentspace[activeLight] = TBN * LightDirection_cameraspace[activeLight];
    }
    // Normal of the the vertex, in camera space
    //vec3 Normal_cameraspace = ( viewMatrix * modelMatrix * vec4(normal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.


}
