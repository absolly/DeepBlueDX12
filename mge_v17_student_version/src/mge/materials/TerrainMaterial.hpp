#ifndef TerrainMaterial_H
#define TerrainMaterial_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/World.hpp"

/**
 * Simple single texture material, this is a sample which doesn't cache anything upfront and
 * passes in separate matrices for the MVP calculation
 */
class TerrainMaterial : public AbstractMaterial {
    public:
        TerrainMaterial (Texture* pHeightMap, Texture* pSplatMap, Texture* pDiffuseTexture1, Texture* pDiffuseTexture2, Texture* pDiffuseTexture3, Texture* pDiffuseTexture4);
        virtual ~TerrainMaterial ();

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        void setDiffuseTexture (Texture* pDiffuseTexture);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        Texture* _heightMap;
        Texture* _splatMap;
        Texture* _diffuseTexture1;
        Texture* _diffuseTexture2;
        Texture* _diffuseTexture3;
        Texture* _diffuseTexture4;

        TerrainMaterial(const TerrainMaterial&);
        TerrainMaterial& operator=(const TerrainMaterial&);

};

#endif // TerrainMaterial_H
