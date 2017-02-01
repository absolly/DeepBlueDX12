#include <glm.hpp>

#include "mge/materials/TerrainMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"
#include "mge/core/Light.hpp"

ShaderProgram* TerrainMaterial::_shader = NULL;

TerrainMaterial::TerrainMaterial(Texture* pHeightMap, Texture* pSplatMap, Texture* pDiffuseTexture1, Texture* pDiffuseTexture2, Texture* pDiffuseTexture3, Texture* pDiffuseTexture4):
    _heightMap(pHeightMap), _splatMap(pSplatMap), _diffuseTexture1(pDiffuseTexture1), _diffuseTexture2(pDiffuseTexture2), _diffuseTexture3(pDiffuseTexture3), _diffuseTexture4(pDiffuseTexture4) {
    _lazyInitializeShader();
}

TerrainMaterial::~TerrainMaterial() {}

void TerrainMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"terrain.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"terrain.fs");
        _shader->finalize();
    }
}

void TerrainMaterial::setDiffuseTexture (Texture* pDiffuseTexture) {
    //_diffuseTexture = pDiffuseTexture;
}

void TerrainMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_heightMap) return;
    if (!_splatMap) return;
    if (!_diffuseTexture1) return;
    if (!_diffuseTexture2) return;
    if (!_diffuseTexture3) return;
    if (!_diffuseTexture4) return;
    _shader->use();

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _heightMap->getId());
    glUniform1i (_shader->getUniformLocation("heightMap"), 0);
    //setup texture slot 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _splatMap->getId());
    glUniform1i (_shader->getUniformLocation("splatMap"), 1);

    //setup texture slot 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture1->getId());
    glUniform1i (_shader->getUniformLocation("textureDiffuse1"), 2);
    //setup texture slot 3
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture2->getId());
    glUniform1i (_shader->getUniformLocation("textureDiffuse2"), 3);
    //setup texture slot 4
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture3->getId());
    glUniform1i (_shader->getUniformLocation("textureDiffuse3"), 4);
    //setup texture slot 5
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture4->getId());
    glUniform1i (_shader->getUniformLocation("textureDiffuse4"), 5);


    glUniform1i(_shader->getUniformLocation("lightCount"), sizeof(World::activeLights));
    glUniform1i(_shader->getUniformLocation("activeLight"), 0);

    glm::vec3 lightPosition[128] {};
    glm::vec3 lightDirection[128] {};
    glm::vec3 lightColor[128] {};
    GLint lightType[128] {};
    GLint lightFalloff[128] {};
    GLfloat lightIntensity[128] {};

    int i = 0;
    for(Light* light : World::activeLights) {
        lightPosition[i] = light->getWorldPosition();
        lightDirection[i] = light->getWorldTransform() * glm::vec4(0,0,1,0);
        lightColor[i] = light->getColor();
        lightType[i] = ((int)light->type);
        lightFalloff[i] = ((int)light->falloff);
        lightIntensity[i] = light->intensity;
        i++;
    }
    glUniform3fv(_shader->getUniformLocation("lightPosition"), 128, glm::value_ptr(lightPosition[0]));
    glUniform3fv(_shader->getUniformLocation("lightDirection"), 128, glm::value_ptr(lightDirection[0]));
    glUniform3fv(_shader->getUniformLocation("lightColor"), 128, glm::value_ptr(lightColor[0]));
    glUniform1iv(_shader->getUniformLocation("lightType"), 128, lightType);
    glUniform1iv(_shader->getUniformLocation("lightFalloff"), 128, lightFalloff);
    glUniform1fv(_shader->getUniformLocation("lightIntensity"), 128, lightIntensity);

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
