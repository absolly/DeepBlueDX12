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
		UnlitTextureMaterial(Texture* pDiffuseTexture, float pTiling = 1, glm::vec3 pColor = glm::vec3(1));
        virtual ~UnlitTextureMaterial();

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;
#ifdef API_DIRECTX
		virtual void render(Mesh* pMesh, D3D12_GPU_VIRTUAL_ADDRESS pGPUAddress) override;
#endif // API_DIRECTX
		static unsigned int _materialCount;
		unsigned int _id;
        void setDiffuseTexture (Texture* pDiffuseTexture);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        Texture* _diffuseTexture;
		struct ConstantBufferPerMaterial {
			ConstantBufferPerMaterial(glm::vec3 pColor, float pTiling) {
				color = pColor;
				tiling = pTiling;
			}
			glm::vec3 color;
			float tiling;
		};
		ConstantBufferPerMaterial cbPerMaterial;

        UnlitTextureMaterial(const UnlitTextureMaterial&);
		UnlitTextureMaterial& operator=(const UnlitTextureMaterial&);
#ifdef API_DIRECTX
		// drawing objects stuff //
		static ID3D12PipelineState* pipelineStateObject; //pso containing a pipeline state (in this case the vertex data for 1 object)
		static ID3D12RootSignature* rootSignature; //root signature defines data shaders will access

		const int ConstantBufferPerMaterialAlignedSize = (sizeof(ConstantBufferPerMaterial) + 255) & ~255;

		static ID3D12Resource* constantBufferUploadHeaps[Renderer::frameBufferCount]; //this is the memory where the constant buffer will be placed TODO

		static UINT8* cbvGPUAddress[Renderer::frameBufferCount]; // pointers to the memory locations we get when we map the constant buffers
																 // constant buffers must be 256 byte aligned
		static ID3D12DescriptorHeap* mainDescriptorHeap;
		static UINT mCbvSrvDescriptorSize;
#endif // API_DIRECTX


};

#endif // UNLITTEXTUREMATERIAL_H
