#include <cassert>
#include <iostream>
using namespace std;

#include <GL/glew.h>

#include "mge/core/Renderer.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/PhysicsObject.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/behaviours/RigidBody.hpp"

Renderer::Renderer() {
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE ); // default GL_BACK
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor((float)0x2d/0xff, (float)0x6b/0xff, (float)0xce/0xff, 1.0f );
}

Renderer::~Renderer() {
}

void Renderer::setClearColor(int pR, int pG, int pB) {
    glClearColor((float)pR/0xff, (float)pG/0xff, (float)pB/0xff, 1.0f );
}

void Renderer::render (World* pWorld) {
    Camera* camera = pWorld->getMainCamera();
    render (pWorld, pWorld->getTransform(), glm::inverse(camera->getWorldTransform()), camera->getProjection(), true);
}

void Renderer::render(GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive) {
    renderSelf(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix);
    if (pRecursive) renderChildren(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix, pRecursive);
}

void Renderer::renderSelf (GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    renderMesh(pGameObject->getMesh(), pGameObject->getMaterial(), pModelMatrix, pViewMatrix, pProjectionMatrix);
    //renderMeshDebugInfo(pGameObject->getMesh(), pModelMatrix, pViewMatrix, pProjectionMatrix);
}

void Renderer::renderMesh (Mesh* pMesh, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (pMesh != NULL && pMaterial!= NULL) pMaterial->render(pMesh, pModelMatrix, pViewMatrix, pProjectionMatrix);
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
		/*
		RigidBody* rigidBody = child->getBehaviour<RigidBody>();

        if(rigidBody != nullptr) {
            glm::vec3 childScale;
            childScale.x = glm::length( child->getTransform()[0]);
            childScale.y = glm::length( child->getTransform()[1]);
            childScale.z = glm::length( child->getTransform()[2]);
            btTransform trans;
			rigidBody->rigidBody->getMotionState()->getWorldTransform(trans);

            glm::mat4 glmTrans;
            trans.getOpenGLMatrix(glm::value_ptr(glmTrans));
			child->setTransform(glm::scale(glmTrans, childScale));
            //child->scale(childScale);
        }*/
        render (child, pModelMatrix * child->getTransform(), pViewMatrix, pProjectionMatrix, pRecursive);
    }
}


