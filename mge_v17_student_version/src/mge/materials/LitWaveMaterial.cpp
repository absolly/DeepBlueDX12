#include <glm.hpp>

#include "mge/materials/LitWaveMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Light.hpp"

ShaderProgram* LitWaveMaterial::_shader = NULL;

LitWaveMaterial::LitWaveMaterial(Texture* pDiffuseTexture, Texture* pWaveMask, float pTiling, float pSpecularMultiplier, Texture* pSpecularTexture, Texture* pNormalTexture) :
_diffuseTexture(pDiffuseTexture), _waveMask(pWaveMask), _tiling(pTiling), _specularTexture(pSpecularTexture), _specularMultiplier(pSpecularMultiplier), _normalTexture(pNormalTexture) {
    _lazyInitializeShader();
	start = clock();
}

LitWaveMaterial::~LitWaveMaterial() {}

void LitWaveMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"litwave.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"texture.fs");
        _shader->finalize();
    }
}

void LitWaveMaterial::setDiffuseTexture (Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void LitWaveMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_diffuseTexture) return;

	_time = (float)difftime(clock(), start);
    _shader->use();

	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _waveMask->getId());
	glUniform1i(_shader->getUniformLocation("textureWaveMask"), 0);

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    glUniform1i (_shader->getUniformLocation("textureDiffuse"), 1);

    //setup texture slot 1
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _specularTexture->getId());
    glUniform1i (_shader->getUniformLocation("textureSpecular"), 2);

    //setup texture slot 2
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _normalTexture->getId());
    glUniform1i (_shader->getUniformLocation("textureNormal"), 3);




    glUniform1i(_shader->getUniformLocation("lightCount"), sizeof(World::activeLights));
    glUniform1i(_shader->getUniformLocation("activeLight"), 0);

    glm::vec3 lightPosition[5] {};
    glm::vec3 lightDirection[5] {};
    glm::vec3 lightColor[5] {};
    GLint lightType[5] {};
    glm::vec3 lightFalloff[5]{};
    GLfloat lightIntensity[5] {};

    int i = 0;
    for(Light* light : World::activeLights) {
        lightPosition[i] = light->getWorldPosition();
        lightDirection[i] = light->getWorldTransform()[2]; // * glm::vec4(0,0,1,0);
        lightColor[i] = light->getColor();
        lightType[i] = ((int)light->type);
        lightFalloff[i] = light->falloff;
        lightIntensity[i] = light->intensity;
        i++;
    }

    glUniform3fv(_shader->getUniformLocation("lightPosition"), 5, glm::value_ptr(lightPosition[0]));
    glUniform3fv(_shader->getUniformLocation("lightDirection"), 5, glm::value_ptr(lightDirection[0]));
    glUniform3fv(_shader->getUniformLocation("lightColor"), 5, glm::value_ptr(lightColor[0]));
    glUniform1iv(_shader->getUniformLocation("lightType"), 5, lightType);
    glUniform3fv(_shader->getUniformLocation("lightFalloff"), 5, glm::value_ptr(lightFalloff[0]));
    glUniform1fv(_shader->getUniformLocation("lightIntensity"), 5, lightIntensity);
    glUniform1i(_shader->getUniformLocation("lightCount"), i);
    glUniform1i(_shader->getUniformLocation("tiling"), _tiling);
    glUniform1i(_shader->getUniformLocation("specularMultiplier"), _specularMultiplier);
	glUniform1f(_shader->getUniformLocation("_time"), _time);
    //pass in all MVP matrices separately
    glUniformMatrix4fv ( _shader->getUniformLocation("projectionMatrix"),   1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("viewMatrix"),         1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("modelMatrix"),        1, GL_FALSE, glm::value_ptr(pModelMatrix));

    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(
        _shader->getAttribLocation("vertex"),
        _shader->getAttribLocation("normal"),
        _shader->getAttribLocation("uv"),
        _shader->getAttribLocation("tangent"),
        _shader->getAttribLocation("bitangent")
    );
}
