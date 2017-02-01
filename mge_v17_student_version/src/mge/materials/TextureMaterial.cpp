#include <glm.hpp>

#include "mge/materials/TextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"
#include "mge/core/Light.hpp"

ShaderProgram* TextureMaterial::_shader = NULL;

TextureMaterial::TextureMaterial(Texture * pDiffuseTexture, float pTiling):_diffuseTexture(pDiffuseTexture), _tiling(pTiling) {
    _lazyInitializeShader();
}

TextureMaterial::~TextureMaterial() {}

void TextureMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"texture.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"texture.fs");
        _shader->finalize();
    }
}

void TextureMaterial::setDiffuseTexture (Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void TextureMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_diffuseTexture) return;

    _shader->use();

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    glUniform1i (_shader->getUniformLocation("textureDiffuse"), 0);


    glUniform1i(_shader->getUniformLocation("lightCount"), sizeof(World::activeLights));
    glUniform1i(_shader->getUniformLocation("activeLight"), 0);

    glm::vec3 lightPosition[28] {};
    glm::vec3 lightDirection[28] {};
    glm::vec3 lightColor[28] {};
    GLint lightType[28] {};
    glm::vec3 lightFalloff[28]{};
    GLfloat lightIntensity[28] {};

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

    glUniform3fv(_shader->getUniformLocation("lightPosition"), 28, glm::value_ptr(lightPosition[0]));
    glUniform3fv(_shader->getUniformLocation("lightDirection"), 28, glm::value_ptr(lightDirection[0]));
    glUniform3fv(_shader->getUniformLocation("lightColor"), 28, glm::value_ptr(lightColor[0]));
    glUniform1iv(_shader->getUniformLocation("lightType"), 28, lightType);
    glUniform3fv(_shader->getUniformLocation("lightFalloff"), 28, glm::value_ptr(lightFalloff[0]));
    glUniform1fv(_shader->getUniformLocation("lightIntensity"), 28, lightIntensity);
    glUniform1i(_shader->getUniformLocation("lightCount"), i);
    glUniform1i(_shader->getUniformLocation("tiling"), _tiling);
    //pass in all MVP matrices separately
    glUniformMatrix4fv ( _shader->getUniformLocation("projectionMatrix"),   1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("viewMatrix"),         1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("modelMatrix"),        1, GL_FALSE, glm::value_ptr(pModelMatrix));

    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(
        _shader->getAttribLocation("vertex"),
        _shader->getAttribLocation("normal"),
        _shader->getAttribLocation("uv")
    );
}
