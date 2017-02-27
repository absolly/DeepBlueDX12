#ifndef LITWAVEMATERIAL_H
#define LITWAVEMATERIAL_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/World.hpp"
#include "mge/config.hpp"
#include <time.h>

/**
 * Simple single texture material, this is a sample which doesn't cache anything upfront and
 * passes in separate matrices for the MVP calculation
 */
class LitWaveMaterial : public AbstractMaterial
{
    public:
		LitWaveMaterial(Texture* pDiffuseTexture, Texture* pWaveMask, float pTiling = 1, float pSpecularMultiplier = 1, Texture* pSpecularTexture = Texture::load(Config::MGE_TEXTURE_PATH + "white.png"), Texture* pNormalTexture = Texture::load(Config::MGE_TEXTURE_PATH + "BricksNormal.png"));
        virtual ~LitWaveMaterial();

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        void setDiffuseTexture (Texture* pDiffuseTexture);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        Texture* _diffuseTexture;
        Texture* _specularTexture;
        Texture* _normalTexture;
		Texture* _waveMask;
        float _specularMultiplier;
        float _tiling;
		float _time;
		time_t start;

		LitWaveMaterial(const LitWaveMaterial&);
		LitWaveMaterial& operator=(const LitWaveMaterial&);

};

#endif // LITWAVEMATERIAL_H
