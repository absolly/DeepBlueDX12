#include <glm.hpp>

#include "mge/materials/GPUinstancingMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"


ShaderProgram* GPUinstancingMaterial::_shader = NULL;

GLint _uMVPMatrix = 0;
GLint _Projection = 0;
GLint _View = 0;

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

GPUinstancingMaterial::GPUinstancingMaterial(std::vector<GameObject*> pGameObjects, Mesh * pMesh)
{
	_gameObjects = pGameObjects;
	_mesh = pMesh;
	_listSize = _gameObjects.size();

	_lazyInitializeShader();

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, pGameObjects.size() * sizeof(glm::mat4), &pGameObjects[0]->getTransform(), GL_STATIC_DRAW);

	GLuint VAO = _mesh->GetVertexBuffer();
	glBindVertexArray(VAO);
	// Set attribute pointers for matrix (4 times vec4)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);

	std::cout << _gameObjects.size();
}

void GPUinstancingMaterial::_lazyInitializeShader() {
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH+"GPUinstancingDemo.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH+"GPUinstancingDemo.fs");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        //_uMVPMatrix= _shader->getUniformLocation("mvpMatrix");

		_Projection = _shader->getUniformLocation("projection");
		_View = _shader->getUniformLocation("view");
        //_uDiffuseColor = _shader->getUniformLocation("diffuseColor");

        _aVertex = _shader->getAttribLocation("vertex");
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

	//pMesh->instanceToOpenGL(_aVertex, _aNormal, _aUV, _aTangent, _aBitangent);

	glUniformMatrix4fv(_Projection, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_View, 1, GL_FALSE, glm::value_ptr(pViewMatrix));


	//for (int i = 0; i < _listSize; i++)
	//{

	//	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * _gameObjects.at(i)->getTransform();

	//	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	//	////pass in a precalculate mvp matrix (see texture material for the opposite)
	//	pMesh->drawInstancedmesh();
	//}

	//pMesh->DisableVertexAttribArrays();
	//std::cout << "I'm rendering " << _listSize << std::endl;

	glBindVertexArray(_mesh->GetVertexBuffer());
	glDrawElementsInstanced(GL_TRIANGLES, _mesh->getVertextIndices()->size(), GL_UNSIGNED_INT, 0, _listSize);
	glBindVertexArray(0);


    ////now inform mesh of where to stream its 
}
