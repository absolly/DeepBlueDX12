#include <glm.hpp>

#include "mge/materials/SeaMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"

ShaderProgram* SeaMaterial::_shader = NULL;

GLint SeaMaterial::_uMVPMatrix = 0;
GLint SeaMaterial::_uDiffuseColor = 0;

GLint SeaMaterial::_aVertex = 0;
GLint SeaMaterial::_aNormal = 0;
GLint SeaMaterial::_aUV = 0;
GLint SeaMaterial::_aTangent = 0;
GLint SeaMaterial::_aBitangent = 0;

SeaMaterial::SeaMaterial(Texture* pDiffuseTexture, float pTiling) : _normalTexture(pDiffuseTexture)
{
	//every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
	_lazyInitializeShader();
	isShadowCaster = false;
	start = clock();
}

#ifdef API_OPENGL
void SeaMaterial::_lazyInitializeShader() {
	//this shader contains everything the material can do (it can render something in 3d using a single color)
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH + "sea.vs");
		_shader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH + "sea.fs");
		_shader->finalize();

		//cache all the uniform and attribute indexes
		_uMVPMatrix = _shader->getUniformLocation("mvpMatrix");
		_uDiffuseColor = _shader->getUniformLocation("diffuseColor");

		_aVertex = _shader->getAttribLocation("vertex");
		_aNormal = _shader->getAttribLocation("normal");
		_aUV = _shader->getAttribLocation("uv");
		_aTangent = _shader->getAttribLocation("tangent");
		_aBitangent = _shader->getAttribLocation("bitangent");
	}
}
#elif defined(API_DIRECTX)
void SeaMaterial::_lazyInitializeShader() {
	std::cout << "DirectX version of Material not implemented" << std::endl;
}
#endif // API_OPENGL / API_DIRECTX

SeaMaterial::~SeaMaterial()
{
	//dtor
}

void SeaMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	_shader->use();
	_time = (float)difftime(clock(), start);
	//pass in a precalculate mvp matrix (see texture material for the opposite)
	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _normalTexture->getId());
	glUniform1i(_shader->getUniformLocation("normalTexture"), 0);

	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	glUniform1f(_shader->getUniformLocation("_time"), _time);
	//set the material color
	//glUniform3fv(_uDiffuseColor, 1 , glm::value_ptr(_diffuseColor));

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV, _aTangent, _aBitangent);
}
