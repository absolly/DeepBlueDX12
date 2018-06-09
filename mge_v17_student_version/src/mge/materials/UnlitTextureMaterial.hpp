#ifndef UNLITTEXTUREMATERIAL_H
#define UNLITTEXTUREMATERIAL_H

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
class UnlitTextureMaterial : public AbstractMaterial
{
    public:
		UnlitTextureMaterial(Texture* pDiffuseTexture, float pTiling = 1);
        virtual ~UnlitTextureMaterial();

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
        float _tiling;
        UnlitTextureMaterial(const UnlitTextureMaterial&);
		UnlitTextureMaterial& operator=(const UnlitTextureMaterial&);
#ifdef API_DIRECTX
		// drawing objects stuff //
		static ID3D12PipelineState* pipelineStateObject; //pso containing a pipeline state (in this case the vertex data for 1 object)
		static ID3D12RootSignature* rootSignature; //root signature defines data shaders will access
#endif // API_DIRECTX


};

#endif // UNLITTEXTUREMATERIAL_H
