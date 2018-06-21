#ifndef TEXTUREMATERIAL_H
#define TEXTUREMATERIAL_H

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/World.hpp"
#include "mge/config.hpp"
#include "mge/core/Renderer.hpp"
#include "mge/core/Camera.hpp"
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

		struct LightBase // 64 bytes, 1024 lights possible
		{
			LightBase() {
				ZeroMemory(this, sizeof(LightBase));
			}
			glm::vec3 Position;
			float falloffStart;
			glm::vec3 Color; //color / intensity
			float falloffEnd;
			glm::vec3 Direction;
			float Angle;
		};
		struct ConstantBufferPerMaterial {
			ConstantBufferPerMaterial(float pSpecularMultiplier, float pTiling) {
				specularMultiplier = pSpecularMultiplier;
				tiling = pTiling;
			}
			glm::vec3 eyePosW;
			float tiling;
			LightBase lights[16];
			float specularMultiplier;
			int numDirLight;
			int numPointLight;
			int numSpotLight;
		};
		ConstantBufferPerMaterial cbPerMaterial;

        TextureMaterial(const TextureMaterial&);
        TextureMaterial& operator=(const TextureMaterial&);
		static unsigned int _materialCount;
		unsigned int _id = 0; 
#ifdef API_DIRECTX
		// drawing objects stuff //
		static ID3D12PipelineState* pipelineStateObject; //pso containing a pipeline state (in this case the vertex data for 1 object)
		static ID3D12RootSignature* rootSignature; //root signature defines data shaders will access

		static ID3D12Resource* constantBufferUploadHeaps[Renderer::frameBufferCount]; //this is the memory where the constant buffer will be placed TODO

		static UINT8* cbvGPUAddress[Renderer::frameBufferCount]; // pointers to the memory locations we get when we map the constant buffers
																 // constant buffers must be 256 byte aligned
		static ID3D12DescriptorHeap* textureDescriptorHeap;
		static UINT mCbvSrvDescriptorSize;
		int cbPerMatAlignedSize = (sizeof(cbPerMaterial) + 255) & ~255;

#endif // API_DIRECTX


};

#endif // TEXTUREMATERIAL_H
