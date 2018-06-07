#ifndef TEXTUREMATERIAL_H
#define TEXTUREMATERIAL_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/World.hpp"
#include "mge/config.hpp"
#include "mge/core/Renderer.hpp"
/**
 * Simple single texture material, this is a sample which doesn't cache anything upfront and
 * passes in separate matrices for the MVP calculation
 */
class TextureMaterial : public AbstractMaterial
{
    public:
        TextureMaterial (Texture* pDiffuseTexture, float pTiling = 1, float pSpecularMultiplier = 1, Texture* pSpecularTexture = Texture::load(Config::MGE_TEXTURE_PATH + "black.png"), Texture* pNormalTexture = Texture::load(Config::MGE_TEXTURE_PATH + "BricksNormal.png"), Texture* pEmissionMap = Texture::load(Config::MGE_TEXTURE_PATH + "Black.png"));
        virtual ~TextureMaterial ();

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;
#ifdef API_DIRECTX
		virtual void render(Mesh* pMesh, D3D12_GPU_VIRTUAL_ADDRESS pGPUAddress) override;
#endif // API_DIRECTX

        void setDiffuseTexture (Texture* pDiffuseTexture);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        Texture* _diffuseTexture;
        Texture* _specularTexture;
        Texture* _normalTexture;
		Texture* _emissionMap;
        float _specularMultiplier;
        float _tiling;
        TextureMaterial(const TextureMaterial&);
        TextureMaterial& operator=(const TextureMaterial&);
#ifdef API_DIRECTX
		// drawing objects stuff //
		static ID3D12PipelineState* pipelineStateObject; //pso containing a pipeline state (in this case the vertex data for 1 object)
		static ID3D12RootSignature* rootSignature; //root signature defines data shaders will access
#endif // API_DIRECTX


};

#endif // TEXTUREMATERIAL_H
