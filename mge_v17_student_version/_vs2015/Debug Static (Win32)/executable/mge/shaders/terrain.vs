//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D   heightMap;
uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
uniform vec3    lightPosition[128];
uniform vec3    lightDirection[128];
uniform int     activeLight;
uniform int     lightType[128];
in vec3 vertex;
in vec3 normal;
in vec2 uv;


out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 Position_worldspace;
out vec3 LightDirection_cameraspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;


void main( void ) {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertex + vec3(0,texture(heightMap,uv).r,0), 1.f);
    texCoord = uv;

    // Position of the vertex, in worldspace : modelMatrix * position
    Position_worldspace = (modelMatrix * vec4(vertex,1)).xyz;

    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin.
    vec3 vertexPosition_cameraspace = ( viewMatrix * modelMatrix * vec4(vertex,1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

    // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity).
    vec3 LightPosition_cameraspace = ( viewMatrix * vec4(lightPosition[activeLight],1)).xyz;
    //LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

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

    // Normal of the the vertex, in camera space
    Normal_cameraspace = ( viewMatrix * modelMatrix * vec4(normal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
}
