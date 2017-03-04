#include <cassert>
#include <iostream>
using namespace std;



#include "mge/core/Renderer.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/PhysicsObject.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge\core\Physics\RigidBody.hpp"
#include "mge/config.hpp"
GLuint Renderer::shadowDepthTexture = 0;

Renderer::Renderer() {
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &ShadowBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, ShadowBuffer);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &shadowDepthTexture);
	glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowDepthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "framebuffer status error" << std::endl;


	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture we're going to render to
	glGenTextures(1, &renderedTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y, 0, GL_RGBA, GL_FLOAT, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The texture we're going to render to
	glGenTextures(1, &brightnessTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, brightnessTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y, 0, GL_RGBA, GL_FLOAT, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The texture we're going to render to
	glGenTextures(1, &waterMaskTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, waterMaskTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The depth buffer
	//GLuint depthrenderbuffer;
	//glGenRenderbuffers(1, &depthrenderbuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// Alternative : Depth texture. Slower, but you can sample it later in your shader
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	// Set "brightnessTexture" as our colour attachement #1
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, brightnessTexture, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, waterMaskTexture, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "framebuffer status error" << std::endl;

	
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
		);
	}

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE ); // default GL_BACK
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor((float)0x2d/0xff, (float)0x6b/0xff, (float)0xce/0xff, 1.0f );

	_shadowShader = new ShaderProgram();
	_shadowShader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH + "ShadowPassthrough.vs");
	_shadowShader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH + "FragmentDepth.fs");
	_shadowShader->finalize();
}

Renderer::~Renderer() {
}

void Renderer::setClearColor(int pR, int pG, int pB) {
    glClearColor((float)pR/0xff, (float)pG/0xff, (float)pB/0xff, 1.0f );
}

void Renderer::render (World* pWorld) {
	renderShadow = false;
    Camera* camera = pWorld->getMainCamera();
    render (pWorld, pWorld->getTransform(), glm::inverse(camera->getWorldTransform()), camera->getProjection(), true);
}

void Renderer::render(GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive) {
    renderSelf(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix);
    if (pRecursive) renderChildren(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix, pRecursive);
}

void Renderer::renderShadowMap(GameObject * pGameObject, const glm::mat4 & pModelMatrix, const glm::mat4 & pViewMatrix, const glm::mat4 & pProjectionMatrix, bool pRecursive)
{
	renderShadow = true;
	renderSelf(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix);
	if (pRecursive) renderChildren(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix, pRecursive);
}

void Renderer::renderSelf (GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    renderMesh(pGameObject->getMesh(), pGameObject->getMaterial(), pModelMatrix, pViewMatrix, pProjectionMatrix);
    //renderMeshDebugInfo(pGameObject->getMesh(), pModelMatrix, pViewMatrix, pProjectionMatrix);
}

void Renderer::renderToShadowMap(Mesh * pMesh, const glm::mat4 & pModelMatrix, const glm::mat4 & pViewMatrix, const glm::mat4 & pProjectionMatrix)
{
	_shadowShader->use();

	glm::mat4 depthMVP = pProjectionMatrix * pViewMatrix * pModelMatrix;

	//pass in all MVP matrices separately
	glUniformMatrix4fv(_shadowShader->getUniformLocation("depthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(
		_shadowShader->getAttribLocation("vertex"),
		_shadowShader->getAttribLocation("normal"),
		_shadowShader->getAttribLocation("uv"),
		_shadowShader->getAttribLocation("tangent"),
		_shadowShader->getAttribLocation("bitangent")
	);
}

void Renderer::renderMesh (Mesh* pMesh, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!renderShadow && pMesh != NULL && pMaterial!= NULL && pMaterial->allowedToRender) pMaterial->render(pMesh, pModelMatrix, pViewMatrix, pProjectionMatrix);
	else if (renderShadow && pMesh != NULL && pMaterial != NULL && pMaterial->isShadowCaster) renderToShadowMap(pMesh, pModelMatrix, pViewMatrix, pProjectionMatrix);
}

void Renderer::renderMeshDebugInfo (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (pMesh != NULL) pMesh->drawDebugInfo(pModelMatrix, pViewMatrix, pProjectionMatrix);
}

void Renderer::renderChildren (GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive) {
    int childCount = pGameObject->getChildCount();
    if (childCount < 1) return;

    //note that with a loop like this, deleting children during rendering is not a good idea :)
    GameObject* child = 0;
    for (int i = 0; i < childCount; i++) {
        child = pGameObject->getChildAt(i);

		/*RigidBody* rigidBody = child->getBehaviour<RigidBody>();
		
        if(rigidBody != nullptr) {
            glm::vec3 childScale;
            childScale.x = glm::length( child->getTransform()[0]);
            childScale.y = glm::length( child->getTransform()[1]);
            childScale.z = glm::length( child->getTransform()[2]);
            btTransform trans;
			rigidBody->getMotionState()->getWorldTransform(trans);

            glm::mat4 glmTrans;
            trans.getOpenGLMatrix(glm::value_ptr(glmTrans));
            child->setTransform(glmTrans);
            child->scale(childScale);
        }*/
        render (child, pModelMatrix * child->getTransform(), pViewMatrix, pProjectionMatrix, pRecursive);
    }
}


