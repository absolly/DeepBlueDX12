#ifndef RENDERER_H
#define RENDERER_H

#include <glm.hpp>
#include <GL/glew.h>
#include "mge/core/ShaderProgram.hpp"

class World;
class GameObject;
class Camera;
class Mesh;
class AbstractMaterial;

/**
 * Renderer implements a default single pass forward renderer.
 */
class Renderer
{
	public:
		Renderer();
		virtual ~Renderer();

        //utility call
        void setClearColor (int pR, int pG, int pB);

        //convenience method to render whole world
        void render (World* pWorld);

        //render specific game object within the world, and optionally all its children
        void render(GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive);
		void renderShadowMap(GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive);
        //renders a specific mesh at the given positions etc with the given material
        void renderMesh (Mesh* pMesh, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);
        void renderChildren (GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive);

        //helper method to render all the children of a gameobject
        void renderMeshDebugInfo (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);
		GLuint renderedTexture;
		GLuint brightnessTexture;
		GLuint FramebufferName = 0;
		GLuint ShadowBuffer = 0;
		static GLuint shadowDepthTexture;
		GLuint depthTexture;
		GLuint pingpongFBO[2];
		GLuint pingpongBuffer[2];
		GLuint srenderedTexture;
    private:
        //helper method to render a single gameobject
        void renderSelf (GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);
		bool renderShadow = false;
		void renderToShadowMap(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);
		ShaderProgram* _shadowShader;
};

#endif // RENDERER_H
