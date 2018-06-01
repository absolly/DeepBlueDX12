#include <glm.hpp>

#include "mge/materials/LitWaveMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Renderer.hpp"


ShaderProgram* LitWaveMaterial::_shader = NULL;

LitWaveMaterial::LitWaveMaterial(Texture* pDiffuseTexture, Texture* pWaveMask, float pTiling, float pSpecularMultiplier, Texture* pSpecularTexture, Texture* pNormalTexture, Texture* pEmissionMap) :
_diffuseTexture(pDiffuseTexture), _waveMask(pWaveMask), _tiling(pTiling), _specularTexture(pSpecularTexture), _specularMultiplier(pSpecularMultiplier), _normalTexture(pNormalTexture), _emissionMap(pEmissionMap) {
    _lazyInitializeShader();
	start = clock();
}

LitWaveMaterial::~LitWaveMaterial() {}

#ifdef API_OPENGL
void LitWaveMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH+"litwave.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH+"texture.fs");
        _shader->finalize();
    }
}
#elif defined(API_DIRECTX)
void LitWaveMaterial::_lazyInitializeShader() {
	std::cout << "DirectX version of Material not implemented" << std::endl;
}
#endif // API_OPENGL / API_DIRECTX

void LitWaveMaterial::setDiffuseTexture (Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void LitWaveMaterial::setSpeed(float pSpeed)
{
	speed = pSpeed;
}

void LitWaveMaterial::setWaveMultiplier(float pWaveMultiplier)
{
	waveMultiplier = pWaveMultiplier;
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

	//setup texture slot 4
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, Renderer::shadowDepthTexture);
	glUniform1i(_shader->getUniformLocation("shadowMap"), 4);

	//setup texture slot 5
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, _emissionMap->getId());
	glUniform1i(_shader->getUniformLocation("emissionMap"), 5);


    glUniform1i(_shader->getUniformLocation("lightCount"), sizeof(World::activeLights));
    glUniform1i(_shader->getUniformLocation("activeLight"), 0);

    glm::vec3 lightPosition[2] {};
    glm::vec3 lightDirection[2] {};
    glm::vec3 lightColor[2] {};
    GLint lightType[2] {};
    glm::vec3 lightFalloff[2]{};
    GLfloat lightIntensity[2] {};

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-300, 300, -300, 300, -800, 800);
	glm::mat4 depthViewMatrix;

	int i = 0;
	for (Light* light : World::activeLights) {

		lightPosition[i] = light->getWorldPosition();
		if (light->type == Light::DIRECTIONAL) {
			depthViewMatrix = glm::inverse(light->getWorldTransform());
			lightPosition[i].y = 900;
		}
		lightDirection[i] = light->getWorldTransform()[2]; // * glm::vec4(0,0,1,0);
		lightColor[i] = light->getColor();
		lightType[i] = ((int)light->type);
		lightFalloff[i] = light->falloff;
		lightIntensity[i] = light->intensity;
		i++;
	}

	glm::mat4 depthBiasMVP = biasMatrix * depthProjectionMatrix * depthViewMatrix * pModelMatrix;


    glUniform3fv(_shader->getUniformLocation("lightPosition"), 2, glm::value_ptr(lightPosition[0]));
    glUniform3fv(_shader->getUniformLocation("lightDirection"), 2, glm::value_ptr(lightDirection[0]));
    glUniform3fv(_shader->getUniformLocation("lightColor"), 2, glm::value_ptr(lightColor[0]));
    glUniform1iv(_shader->getUniformLocation("lightType"), 2, lightType);
    glUniform3fv(_shader->getUniformLocation("lightFalloff"), 2, glm::value_ptr(lightFalloff[0]));
    glUniform1fv(_shader->getUniformLocation("lightIntensity"), 2, lightIntensity);
    glUniform1i(_shader->getUniformLocation("lightCount"), i);
    glUniform1i(_shader->getUniformLocation("tiling"), _tiling);
    glUniform1i(_shader->getUniformLocation("specularMultiplier"), _specularMultiplier);
	glUniform1f(_shader->getUniformLocation("_time"), _time);
	glUniform1f(_shader->getUniformLocation("speed"), speed);
	glUniform1f(_shader->getUniformLocation("waveMultiplier"), waveMultiplier);
    //pass in all MVP matrices separately
    glUniformMatrix4fv ( _shader->getUniformLocation("projectionMatrix"),   1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("viewMatrix"),         1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("modelMatrix"),        1, GL_FALSE, glm::value_ptr(pModelMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("depthBiasMVP"), 1, GL_FALSE, glm::value_ptr(depthBiasMVP));

    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(
        _shader->getAttribLocation("vertex"),
        _shader->getAttribLocation("normal"),
        _shader->getAttribLocation("uv"),
        _shader->getAttribLocation("tangent"),
        _shader->getAttribLocation("bitangent")
    );
}
