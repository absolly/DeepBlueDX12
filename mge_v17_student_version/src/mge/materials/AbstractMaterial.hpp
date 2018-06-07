#ifndef ABSTRACTMATERIAL_H
#define ABSTRACTMATERIAL_H
#include <glm.hpp>
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <windows.h>
#include "mge\Config.hpp"

class GameObject;
class Mesh;

/**
 * AbstractMaterial should be subclassed for all materials, for example ColorMaterial, TextureMaterial.
 *
 * Basically a material is responsible for setting up the correct shader,
 * filling all the correct parameters and actually rendering a mesh by streaming all mesh data through a
 * specific shader. Subclasses often use a static shader variable which is initialized once upon first use,
 * but that is fully optional (!).
 *
 * This works like the Unity Material, where you have to select a shader for a material and the shader chosen
 * determines which properties actually become available for the material itself.
 */
class AbstractMaterial
{
    public:
        AbstractMaterial();
        virtual ~AbstractMaterial();

		bool isShadowCaster = true;
		bool allowedToRender = true;
		bool isDoubleSided = false;
        /**
         * Render the given mesh.
         */

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pPerspectiveMatrix) = 0;
		virtual void render(Mesh* pMesh, D3D12_GPU_VIRTUAL_ADDRESS pGPUAddress) {
			std::cout << "directx version of material not implemented" << std::endl;
		};

#ifdef API_DIRECTX
		static HRESULT CompileShaderFromFile(_In_ std::wstring pFileName,
			_In_reads_opt_(_Inexpressible_(pDefines->Name != NULL)) CONST D3D_SHADER_MACRO* pDefines,
			_In_opt_ ID3DInclude* pInclude,
			_In_ LPCSTR pEntrypoint,
			_In_ LPCSTR pTarget,
			_In_ UINT Flags1,
			_In_ UINT Flags2,
			_Out_ ID3DBlob** ppCode,
			_Always_(_Outptr_opt_result_maybenull_) ID3DBlob** ppErrorMsgs) {
			return D3DCompileFromFile((Config::MGE_SHADER_PATH + pFileName).c_str(), pDefines, pInclude, pEntrypoint, pTarget, Flags1, Flags2, ppCode, ppErrorMsgs);
		};
#endif
};

#endif // ABSTRACTMATERIAL_H
