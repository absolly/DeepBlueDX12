#ifndef GPUINSTANCINGMATERIAL_H
#define GPUINSTANCINGMATERIAL_H

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/ShaderProgram.hpp"
#include <mge/core/Texture.hpp>
#include "mge/core/GameObject.hpp"
#include <mge/config.hpp>
#include <vector>

/**
 * Simple single color material.
 * This material uses no lights.
 * This material demonstrates how to render a single color material, and how to cache all attributes, uniforms and pass in a single MVP matrix to the shader.
 */
class GPUinstancingMaterial : public AbstractMaterial
{
    public:
		//GPUinstancingMaterial(std::vector<GameObject*> gameObjects);
		GPUinstancingMaterial(std::vector<GameObject*> pGameObjects, Texture * pTexture = Texture::load(Config::MGE_TEXTURE_PATH + "Kelpp.png"));
        virtual ~GPUinstancingMaterial();

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        //in rgb values
        void setDiffuseColor (glm::vec3 pDiffuseColor);

    private:

		int _listSize = 0;
		bool firstTry = true;

		static ShaderProgram* _shader;
		void  _lazyInitializeShader();
		std::vector<GameObject*> _gameObjects;

		Texture * _diffuseTexture;
		GLint _uMVPMatrix = 0;
		GLint _offset = 0;
		GLuint TextureID = 0;

		GLint _aVertex = 0;
		GLint _aNormal = 0;
		GLint _aUV = 0;
		GLint _aTangent = 0;
		GLint _aBitangent = 0;
        //all the static properties are shared between instances of ColorMaterial
        //note that they are all PRIVATE, we do not expose this static info to the outside world
 
};

#endif // COLORMATERIAL_H
