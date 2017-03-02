#ifndef SeaMaterial_H
#define SeaMaterial_H

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
class SeaMaterial : public AbstractMaterial
{
    public:
		SeaMaterial(Texture* pDiffuseTexture, float pTiling = 1);
        virtual ~SeaMaterial();

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        void setDiffuseTexture (Texture* pDiffuseTexture);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        Texture* _diffuseTexture;
        Texture* _normalTexture;
        float _tiling;
		float _time;
		time_t start;

		static GLint _uMVPMatrix;
		static GLint _uDiffuseColor;

		static GLint _aVertex;
		static GLint _aNormal;
		static GLint _aUV;
		static GLint _aTangent;
		static GLint _aBitangent;

		SeaMaterial(const SeaMaterial&);
		SeaMaterial& operator=(const SeaMaterial&);

};

#endif // SeaMaterial_H
