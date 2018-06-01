#include <glm.hpp>

#include "mge/materials/DebugRenderMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "glm.hpp"
#include "mge\core\Camera.hpp"
#include "mge\core\World.hpp"

ShaderProgram* DebugRenderMaterial::_shader = NULL;

GLint DebugRenderMaterial::_uMVPMatrix = 0;
GLint DebugRenderMaterial::_uDiffuseColor = 0;

GLint DebugRenderMaterial::_aVertex = 0;
GLint DebugRenderMaterial::_aNormal = 0;
GLint DebugRenderMaterial::_aUV = 0;
GLint DebugRenderMaterial::_aTangent = 0;
GLint DebugRenderMaterial::_aBitangent = 0;

DebugRenderMaterial::DebugRenderMaterial(glm::vec3 pDiffuseColor):_diffuseColor (pDiffuseColor)
{
    //every time we create an instance of DebugRenderMaterial we check if the corresponding shader has already been loaded
    _lazyInitializeShader();
}

#ifdef API_OPENGL
void DebugRenderMaterial::_lazyInitializeShader() {
	
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH+"color.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH+"color.fs");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        _uMVPMatrix= _shader->getUniformLocation("mvpMatrix");
        _uDiffuseColor = _shader->getUniformLocation("diffuseColor");

        _aVertex = _shader->getAttribLocation("vertex");
        _aNormal = _shader->getAttribLocation("normal");
        _aUV =     _shader->getAttribLocation("uv");
        _aTangent = _shader->getAttribLocation("tangent");
        _aBitangent = _shader->getAttribLocation("bitangent");
    }
}
#elif defined(API_DIRECTX)
void DebugRenderMaterial::_lazyInitializeShader() {
	std::cout << "DirectX version of Material not implemented" << std::endl;
}
#endif // API_OPENGL / API_DIRECTX

DebugRenderMaterial::~DebugRenderMaterial()
{
    //dtor
}

void DebugRenderMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) {
    _diffuseColor = pDiffuseColor;
}

void DebugRenderMaterial::render(World& world) {

	_shader->use();
	//pass in a precalculate mvp matrix (see texture material for the opposite)
	

	glm::mat4 pModelMatrix = world.getTransform();
	glm::mat4 pViewMatrix = glm::inverse(world.getMainCamera()->getWorldTransform());
	glm::mat4 pProjectionMatrix = world.getMainCamera()->getProjection();

	glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix;
	
	glUniformMatrix4fv(_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	//glm::value_ptr ptr;
	//set the material color
	glUniform3fv(_uDiffuseColor, 1, glm::value_ptr(_diffuseColor));
	
}
