#include <glm.hpp>

#include "mge/materials/GPUinstancingMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"

ShaderProgram* GPUinstancingMaterial::_shader = NULL;

GLint _uMVPMatrix = 0;

GLint _aVertex = 0;
GLint _aNormal = 0;
GLint _aUV = 0;
GLint _aTangent = 0;
GLint _aBitangent = 0;

//GPUinstancingMaterial::GPUinstancingMaterial(std::vector<GameObject*> gameObjects)
//{
//    //every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
//
//}
//

GPUinstancingMaterial::GPUinstancingMaterial(std::vector<GameObject*> pGameObjects)
{
	_gameObjects = pGameObjects;
	_listSize = _gameObjects.size();

	_lazyInitializeShader();

	std::cout << _gameObjects.size();
}

void GPUinstancingMaterial::_lazyInitializeShader() {
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"GPUinstancing.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"GPUinstancing.fs");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        _uMVPMatrix= _shader->getUniformLocation("mvpMatrix");
        //_uDiffuseColor = _shader->getUniformLocation("diffuseColor");

        _aVertex = _shader->getAttribLocation("vertex");
        _aNormal = _shader->getAttribLocation("normal");
        _aUV =     _shader->getAttribLocation("uv");
        _aTangent = _shader->getAttribLocation("tangent");
        _aBitangent = _shader->getAttribLocation("bitangent");
    }
}
//
GPUinstancingMaterial::~GPUinstancingMaterial()
{
    //dtor
}
//
//void ColorMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) {
//    _diffuseColor = pDiffuseColor;
//}
//
void GPUinstancingMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	_shader->use();

	pMesh->instanceToOpenGL(_aVertex, _aNormal, _aUV, _aTangent, _aBitangent);

	std::cout << pModelMatrix << std::endl;


	for (int i = 0; i < _listSize; i++)
	{

		glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * _gameObjects.at(i)->getTransform();

		glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
		////pass in a precalculate mvp matrix (see texture material for the opposite)
		pMesh->drawInstancedmesh();
	}

	pMesh->DisableVertexAttribArrays();
	


    ////now inform mesh of where to stream its 
}
