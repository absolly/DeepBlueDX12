#ifndef COLORMATERIAL_H
#define COLORMATERIAL_H

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Renderer.hpp"

/**
 * Simple single color material.
 * This material uses no lights.
 * This material demonstrates how to render a single color material, and how to cache all attributes, uniforms and pass in a single MVP matrix to the shader.
 */
class ColorMaterial : public AbstractMaterial
{
    public:
        ColorMaterial(glm::vec3 pColor = glm::vec3(1,0,0));
        virtual ~ColorMaterial();

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;
#ifdef API_DIRECTX
		virtual void render(Mesh* pMesh, D3D12_GPU_VIRTUAL_ADDRESS pGPUAddress) override;
#endif // API_DIRECTX
		static unsigned int _materialCount;
		unsigned int _id;

        //in rgb values
        void setDiffuseColor (glm::vec3 pDiffuseColor);

    private:

        //all the static properties are shared between instances of ColorMaterial
        //note that they are all PRIVATE, we do not expose this static info to the outside world
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        //in this example we cache all identifiers for uniforms & attributes
        static GLint _uMVPMatrix;
        static GLint _uDiffuseColor;

        static GLint _aVertex ;
        static GLint _aNormal;
        static GLint _aUV ;
        static GLint _aTangent;
        static GLint _aBitangent;

#ifdef API_DIRECTX
		static ID3D12PipelineState* pipelineStateObject; //pso containing a pipeline state (in this case the vertex data for 1 object)
		static ID3D12RootSignature* rootSignature; //root signature defines data shaders will access
		static ID3D12Resource* constantBufferUploadHeaps[Renderer::frameBufferCount]; //this is the memory where the constant buffer will be placed TODO

		static UINT8* cbvGPUAddress[Renderer::frameBufferCount]; // pointers to the memory locations we get when we map the constant buffers
																 // constant buffers must be 256 byte aligned
		int cbPerMatAlignedSize = (sizeof(glm::vec3) + 255) & ~255;

#endif // API_DIRECTX

        //this one is unique per instance of color material
        glm::vec3 _diffuseColor;
};

#endif // COLORMATERIAL_H
