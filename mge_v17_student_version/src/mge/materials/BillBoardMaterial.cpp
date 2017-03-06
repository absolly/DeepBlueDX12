#include <glm.hpp>

#include "mge/materials/BillBoardMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/GameObject.hpp"
#include "Content\GameObjects\ParticleSystem.hpp"
#include "mge/core/Mesh.hpp"


ShaderProgram* BillBoardMaterial::_shader = NULL;

GLint BillBoardMaterial::_uVPMatrix = 0;

GLint BillBoardMaterial::CameraUp = 0;
GLint BillBoardMaterial::CameraRight = 0;
GLint BillBoardMaterial::BillboardPos = 0;
GLint BillBoardMaterial::BillboardSize = 0;
GLuint BillBoardMaterial::TextureID = 0;
GLuint BillBoardMaterial::billboard_vertex_buffer = 0;

//GPUinstancingMaterial::GPUinstancingMaterial(std::vector<GameObject*> gameObjects)
//{
//    //every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
//
//}
//

BillBoardMaterial::BillBoardMaterial(ParticleSystem * pParticleSystem, Texture * pTexture) : _diffuseTexture(pTexture)
{
	_lazyInitializeShader();
	_particleSystem = pParticleSystem;

	std::cout << _gameObjects.size();

	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
}

void BillBoardMaterial::_lazyInitializeShader() {
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH +"billboard.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH+"billboard.fs");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        //_uMVPMatrix= _shader->getUniformLocation("mvpMatrix");
        ////_uDiffuseColor = _shader->getUniformLocation("diffuseColor");

        //_aVertex = _shader->getAttribLocation("vertex");
        //_aNormal = _shader->getAttribLocation("normal");
        //_aUV =     _shader->getAttribLocation("uv");
        //_aTangent = _shader->getAttribLocation("tangent");
        //_aBitangent = _shader->getAttribLocation("bitangent");

		_uVPMatrix = _shader->getUniformLocation("VP");

		CameraUp = _shader->getUniformLocation("CameraUp_worldspace");
		CameraRight = _shader->getUniformLocation("CameraRight_worldspace");
		BillboardPos = _shader->getUniformLocation("BillboardPos");
		BillboardSize = _shader->getUniformLocation("BillboardSize");
		TextureID = _shader->getUniformLocation("myTextureSampler");

    }
}
//
BillBoardMaterial::~BillBoardMaterial()
{
    //dtor
}
//
//void ColorMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) {
//    _diffuseColor = pDiffuseColor;
//}
//
void BillBoardMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	_shader->use();

	glUniform3f(CameraRight, pViewMatrix[0][0], pViewMatrix[1][0], pViewMatrix[2][0]);
	glUniform3f(CameraUp, pViewMatrix[0][1], pViewMatrix[1][1], pViewMatrix[2][1]);

	glUniform2f(BillboardSize, 1.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(TextureID, 0);

	glm::mat4 vpMatrix = pProjectionMatrix * pViewMatrix;

	glUniformMatrix4fv(_uVPMatrix, 1, GL_FALSE, glm::value_ptr(vpMatrix));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	std::vector<GameObject*> particleList = _particleSystem->GetParticles();

	for (int i = 0; i < particleList.size();i++)
	{
		glm::vec3 particlepos = particleList[i]->getWorldPosition();
		glUniform3f(BillboardPos, particlepos.x, particlepos.y, particlepos.z); // The billboard will be just above the cube

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glDisableVertexAttribArray(0);

	// Cleanup VBO and shader


	/*pMesh->instanceToOpenGL(_aVertex, _aNormal, _aUV, _aTangent, _aBitangent);*/


	//for (int i = 0; i < _listSize; i++)
	//{
		//glm::vec3 curLoc = _gameObjects.at(i)->getWorldPosition();
		//_gameObjects.at(i)->setTransform(glm::lookAt(curLoc, _camera->getWorldPosition(), CameraUpWorldSpace));
		//_gameObjects.at(i)->translate(curLoc);

		//glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * _gameObjects.at(i)->getTransform();

		//glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
		//////pass in a precalculate mvp matrix (see texture material for the opposite)
		//pMesh->drawInstancedmesh();
	//}

	//pMesh->DisableVertexAttribArrays();
	


    ////now inform mesh of where to stream its 
}
