#include "InstancingExample.hpp"

//if(Light::activeLights != 0)
//    Light* Light::activeLights[255] = {nullptr};


InstancingExample::InstancingExample(std::string pName, glm::vec3 pPosition)
    : GameObject(pName, pPosition){


	GLfloat quadVertices[] = {
		// Positions     // Colors
		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	};
}
