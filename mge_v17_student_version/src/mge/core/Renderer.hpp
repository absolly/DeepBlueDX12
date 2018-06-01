#ifndef RENDERER_H
#define RENDERER_H

#include <glm.hpp>
#include <GL/glew.h>
#include "mge/core/ShaderProgram.hpp"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include "d3dx12.h"

class World;
class GameObject;
class Camera;
class Mesh;
class AbstractMaterial;

/**
 * Renderer implements a default single pass forward renderer.
 */
class Renderer
{
	public:
#ifdef API_OPENGL
		Renderer();
#elif defined(API_DIRECTX)
		Renderer(HWND hwnd);
#endif // API_OPENGL

		virtual ~Renderer();

        //utility call
        void setClearColor (int pR, int pG, int pB);

        //convenience method to render whole world
        void render (World* pWorld);

        //render specific game object within the world, and optionally all its children
        void render(GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive);
		void renderShadowMap(GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive);
        //renders a specific mesh at the given positions etc with the given material
        void renderMesh (int pConstantBufferId, Mesh* pMesh, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);
        void renderChildren (GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive);

        //helper method to render all the children of a gameobject
        void renderMeshDebugInfo (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);
		GLuint renderedTexture;
		GLuint waterMaskTexture;
		GLuint brightnessTexture;
		GLuint FramebufferName = 0;
		GLuint ShadowBuffer = 0;
		static GLuint shadowDepthTexture;
		GLuint depthTexture;
		GLuint pingpongFBO[2];
		GLuint pingpongBuffer[2];
		GLuint srenderedTexture;

#ifdef API_DIRECTX
		IDXGIFactory4* dxgiFactory;

		static const int frameBufferCount = 3; // 2 for double buffering, 3 for tripple buffering

		static ID3D12Device* device; //specifies the device we use to render

		IDXGISwapChain3* swapChain; //used to switch between render targets

		ID3D12CommandQueue* commandQueue; //container for the command lists

		ID3D12DescriptorHeap* rtvDescriptorHeap; //holds resources like render targets

		ID3D12Resource* renderTargets[frameBufferCount]; //render target per frame buffer count

		ID3D12CommandAllocator* commandAllocator[frameBufferCount]; // command allocator per thread per frame buffer

		static ID3D12GraphicsCommandList* commandList; //command list which commands can be recorded into and then be executed to render the frame

		ID3D12Fence* fence[frameBufferCount]; //an object that is locked while the command list is eing executed by the gpu. 

		HANDLE fenceEvent; //a handle to an event for when the fence is unlocked by the gpu

		UINT64 fenceValue[frameBufferCount]; //this value is incremented each frame. each fence will have its one value

		int frameIndex; //current rtv

		int rtvDescriptorSize; //size of the rtv descriptor on the device (all front and back buffers will be the same size)

		ID3D12Resource* constantBufferUploadHeaps[frameBufferCount]; //this is the memory where the constant buffer will be placed TODO

		UINT8* cbvGPUAddress[frameBufferCount]; // pointers to the memory locations we get when we map the constant buffers
												//constant buffers must be 256 byte aligned
		struct ConstantBufferPerObject {
			glm::mat4 wvpMat;
			glm::mat4 wMat;
			glm::mat4 vMat;
			glm::mat4 pMat;
		};

		int ConstantBufferPerObjectAlignedSize = (sizeof(ConstantBufferPerObject) + 255) & ~255;

		ConstantBufferPerObject cbPerObject;

		D3D12_VIEWPORT viewport; //area that the rasterizer will be streched to.

		D3D12_RECT scissorRect; //the area of the window that can be drawn in. pixels outside the area will not be drawn

		ID3D12Resource* depthStencilBuffer; //the memory for out depth buffer, will also be used for the stencil buffer later on

		ID3D12DescriptorHeap* dsDescriptorHeap; //this is a heap fo the depth/stencil descriptor

		void CloseCommandList();

#endif
    private:
        //helper method to render a single gameobject
        void renderSelf (GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);
		bool renderShadow = false;
		void renderToShadowMap(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);
		ShaderProgram* _shadowShader;
#ifdef API_DIRECTX
		void WaitForPreviousFrame();
#endif
};

#endif // RENDERER_H
