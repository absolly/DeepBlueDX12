//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1
uniform sampler2D textureWaveMask;
uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
uniform mat4	depthBiasMVP;
uniform vec3    lightPosition[5];
uniform vec3    lightDirection[5];
uniform int     lightType[5];
uniform	int     lightCount;
uniform float	_time;
uniform float	speed;
in vec3			vertex;
in vec3			normal;
in vec2			uv;
in vec3			tangent;
in vec3			bitangent;



out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 Position_worldspace;
mat3 TBN;
out vec3 LightDirection_tangentspace[5];
out vec3 EyeDirection_tangentspace;
out vec4 ShadowCoord;
vec3 LightDirection_cameraspace[5];
vec3 EyeDirection_cameraspace;


void main( void ) {
	vec3 waveSize = vec3(texture(textureWaveMask, uv));

	vec3 newvertex = vertex + vec3(waveSize.r * cos((_time * speed *0.02)  + (vertex.z * 0.3) + (vertex.y * 0.5)), waveSize.g * cos((_time * speed*0.02) + (vertex.z * 0.0001)),waveSize.b * cos((_time * speed *0.02) + (vertex.z * 0.001)));
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(newvertex, 1.f);
	ShadowCoord = depthBiasMVP * vec4(newvertex,1);
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
