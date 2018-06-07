#include <cassert>
#include <iostream>

#include "mge/core/Renderer.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/PhysicsObject.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge\core\Physics\RigidBody.hpp"
#include "mge/config.hpp"
#include "mge/core/Texture.hpp"
using namespace std;

GLuint Renderer::shadowDepthTexture = 0;

#ifdef API_DIRECTX
ID3D12Device* Renderer::device = nullptr;
ID3D12GraphicsCommandList*  Renderer::commandList = nullptr;
#endif

#ifdef API_OPENGL
Renderer::Renderer() {
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &ShadowBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, ShadowBuffer);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &shadowDepthTexture);
	glBindTexture(GL_TEXTURE_2D, shadowDepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowDepthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "framebuffer status error" << std::endl;


	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// The texture we're going to render to
	glGenTextures(1, &renderedTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y, 0, GL_RGBA, GL_FLOAT, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The texture we're going to render to
	glGenTextures(1, &brightnessTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, brightnessTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y, 0, GL_RGBA, GL_FLOAT, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The texture we're going to render to
	glGenTextures(1, &waterMaskTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, waterMaskTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The depth buffer
	//GLuint depthrenderbuffer;
	//glGenRenderbuffers(1, &depthrenderbuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	// Alternative : Depth texture. Slower, but you can sample it later in your shader
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	// Set "brightnessTexture" as our colour attachement #1
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, brightnessTexture, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, waterMaskTexture, 0);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(3, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "framebuffer status error" << std::endl;

	
	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongBuffer);
	for (GLuint i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongBuffer[i]);
		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB16F, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y, 0, GL_RGB, GL_FLOAT, NULL
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffer[i], 0
		);
	}

    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE ); // default GL_BACK
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor((float)0x2d/0xff, (float)0x6b/0xff, (float)0xce/0xff, 1.0f );

	_shadowShader = new ShaderProgram();
	_shadowShader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH + "ShadowPassthrough.vs");
	_shadowShader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH + "FragmentDepth.fs");
	_shadowShader->finalize();
}
#elif defined(API_DIRECTX)
Renderer::Renderer(HWND hwnd) {
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));

	// create the command queue //
	{
		D3D12_COMMAND_QUEUE_DESC cqDesc = {}; //using default command queue values;
		cqDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		cqDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT; //direct means the gpu can directly execute this command queue

		ThrowIfFailed(device->CreateCommandQueue(&cqDesc, IID_PPV_ARGS(&commandQueue))); // create the command queue
	}

	// display mode settings
	DXGI_MODE_DESC backBufferDesc = {};
	backBufferDesc.Width = Config::SCREEN_RESOLUTION.x;
	backBufferDesc.Height = Config::SCREEN_RESOLUTION.y;
	backBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //color format of the buffer (rgba 32 bit)

														// multi-sampling settings (not using it currently)
	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1; // 1 sample count is disables multi-sampling
						  //TODO: msaa

						  // create the swapchain (double/tripple buffering) //
	{
		// swap chain settings
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		swapChainDesc.BufferCount = frameBufferCount;
		swapChainDesc.BufferDesc = backBufferDesc;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //tells dx pipeline that we will render to this swap chain
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //dxgi will discard the buffer data after we call present
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.SampleDesc = sampleDesc;
		swapChainDesc.Windowed = !Config::FULL_SCREEN;

		IDXGISwapChain* tempSwapChain;

		dxgiFactory->CreateSwapChain(
			commandQueue, //the queue will be flushed once the swap chain is created
			&swapChainDesc, //pass it the swapchain we created above
			&tempSwapChain //store the swapchain in a temp IDXGISwapChain interface (cast to IDXGISwapChain3 later)
		);

		swapChain = static_cast<IDXGISwapChain3*>(tempSwapChain);

		frameIndex = swapChain->GetCurrentBackBufferIndex();
	}

	// create the back buffers (rtv's) discriptor heap //
	{
		// describe a rtv descriptor heap and create
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = frameBufferCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; //this is a rtv heap

														   //this heap will not be directly visible to shaders as it will store teh output from the pipeline
														   // otherwise we would set teh heap's flag to D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvDescriptorHeap)));

		//get the size of teh descriptor in this heap (this is a rtv heap so only rtv descriptors should be stored in it)
		//descriptor sizes may vary from device to device. which is where there is no set size and we must ask the device to give us the size
		// we will use the given size to increment a discriptor handle offset
		rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		//get a handle to the first descriptor in the descriptor heap. a handle is basically a pointer,
		//but we cannot literally use it like a c++ pointer. it is for the driver to use.
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

		//create a rtv for each buffer (3 for tripple buffering in this case)
		for (int i = 0; i < frameBufferCount; i++)
		{
			//first get the n'th buffer in the swap chain and store it in the n'th
			//position of out ID3D12Resource array
			ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i])));

			//then we "create" a rtv which binds the swap chain buffer (ID3D12Resource[n])
			device->CreateRenderTargetView(renderTargets[i], nullptr, rtvHandle);

			//increment the rtv handle by the rtv descriptor size
			rtvHandle.Offset(1, rtvDescriptorSize);
		}
	}

	// create the command allocators //
	{
		for (int i = 0; i < frameBufferCount; i++)
			ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator[i])));

		// create the command list with the first allocator. we only need one since we only use one thread and can reset the cpu side list right after executing
		ThrowIfFailed(device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator[frameIndex], NULL, IID_PPV_ARGS(&commandList)));
	}

	// create a fence and a fence event //
	{
		//create fences
		for (int i = 0; i < frameBufferCount; i++)
		{
			ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence[i])));
			fenceValue[i] = 0; // set initial fence value to 0
		}

		//create a handle to the fence event
		fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if (fenceEvent == nullptr) {
			MessageBox(0, L"Failed to initialize direct3d 12, could not create fence event", L"Error", MB_OK);
			return;
		}
	}


	//create a depth stencil descriptor heap so we can get a pointer to the depth stencil buffer
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	ThrowIfFailed(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsDescriptorHeap)));

	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	ThrowIfFailed(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, Config::SCREEN_RESOLUTION.x, Config::SCREEN_RESOLUTION.y, 1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(&depthStencilBuffer)
	));

	dsDescriptorHeap->SetName(L"Depth/Stencil Resource Heap");

	device->CreateDepthStencilView(depthStencilBuffer, &depthStencilDesc, dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());


	//create a resource heap, descriptor heap, and pointer to cbv for every framebuffer
	for (int i = 0; i < frameBufferCount; i++)
	{
		ThrowIfFailed(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(1024 * 64), //must be a multiple of 64kb thus 64 bytes * 1024 (4mb multiple for multi-sampled textures)
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constantBufferUploadHeaps[i])
		));


		constantBufferUploadHeaps[i]->SetName(L"Constant Buffer Upload Resource Heap");

		//this shouldn't need te be done inside the loop
		ZeroMemory(&cbPerObject, sizeof(cbPerObject));

		CD3DX12_RANGE readRange(0, 0); //read range is less then 0, indicates that we will not be reading this resource from the cpu

									   //map the resource heap to get a gpu virtual address to the beginning of the heap
		ThrowIfFailed(constantBufferUploadHeaps[i]->Map(0, &readRange, reinterpret_cast<void**>(&cbvGPUAddress[i])));

		//because of the constant read alignment requirements, constant buffer views must be 256 byte aligned. since our buffers are smaller than 256 bits
		//we just need to add the spacing between the two buffers, so the second buffer starts 256 byte from the beginning of the heap
		//
		//memcpy(cbvGPUAddress[i], &cbPerObject, sizeof(cbPerObject)); //cube1's constant buffer data
		//memcpy(cbvGPUAddress[i] + ConstantBufferPerObjectAlignedSize, &cbPerObject, sizeof(cbPerObject)); //cube2's constant buffer data
	}

	//fill out the viewport
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = Config::SCREEN_RESOLUTION.x;
	viewport.Height = Config::SCREEN_RESOLUTION.y;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	//fill out the scissor rect
	scissorRect.left = 0;
	scissorRect.right = Config::SCREEN_RESOLUTION.x;
	scissorRect.top = 0;
	scissorRect.bottom = Config::SCREEN_RESOLUTION.y;
}
#endif // API_OPENGL

Renderer::~Renderer() {
}

void Renderer::setClearColor(int pR, int pG, int pB) {
    glClearColor((float)pR/0xff, (float)pG/0xff, (float)pB/0xff, 1.0f );
}
#ifdef API_OPENGL
void Renderer::render (World* pWorld) {
	renderShadow = false;
    Camera* camera = pWorld->getMainCamera();
    render (pWorld, pWorld->getTransform(), glm::inverse(camera->getWorldTransform()), camera->getProjection(), true);
}
#elif defined(API_DIRECTX)
void Renderer::render(World* pWorld) {
	//wait for the gpu to finish with the command allocator before we reset it
	WaitForPreviousFrame();

	//resetting an allocator frees the memory that the command list was stored in
	ThrowIfFailed(commandAllocator[frameIndex]->Reset());

	//reset the command list. by resetting it it will be put into the recording state so we can start recording commands.
	//the command allocator can have multiple command lists associated with it but only one command list can recording at the time.
	//so make sure any other command list is in closed state
	//here you will pass an initial pipeline state object(pso) as the second paramter
	ThrowIfFailed(commandList->Reset(commandAllocator[frameIndex], NULL));
	
	// this is where the commands are recorded into the command list //

	//transition the 'frameIndex' render target from the present state to the render target state so the command list drawas to it starting from here
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	//get the handle to our current rtv so we can set it as the render target view for the output merger
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(), frameIndex, rtvDescriptorSize);

	//get handle for the depth/stencil buffer
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(dsDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	//set the render target for the output merger stage
	commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

	//Clear the render target to the specified clear color
	const float clearColor[] = { 0.0f,0.2f,0.4f,1.0f };
	commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//set the root signature

	//draw!
	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//set the descriptor heap
	ID3D12DescriptorHeap* descriptorHeaps[] = { Texture::GetDescriptorHeap() };
	Renderer::commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	renderShadow = false;
	Camera* camera = pWorld->getMainCamera();
	render(pWorld, pWorld->getTransform(), glm::inverse(glm::scale(camera->getWorldTransform(), glm::vec3(1, 1, -1))), camera->getProjection(), true);
	////first cube
	//mat1->Render(diveScooterMesh, constantBufferUploadHeaps[frameIndex]->GetGPUVirtualAddress() + ConstantBufferPerObjectAlignedSize * go1->_constantBufferID);

	////second cube

	////set cube2's constant buffer. we add the size of the ConstantBufferPerObject (256 bits) to the constatn buffer address.
	////this is because cube2's constant buffer data is stored after the first one(256 bits from the start of the heap)	
	//mat2->Render(mantaMesh, constantBufferUploadHeaps[frameIndex]->GetGPUVirtualAddress() + ConstantBufferPerObjectAlignedSize * go2->_constantBufferID);

	//transition the 'frameIndex' render target from the render target state to the present state.
	//if the debug layer is enabled you will receive an error if present is called on a render target that is not in present state
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	CloseCommandList();

	ThrowIfFailed(swapChain->Present(0, 0));
}
#endif // API_OPENGL

void Renderer::render(GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive) {
    renderSelf(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix);
    if (pRecursive) renderChildren(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix, pRecursive);
}

void Renderer::renderShadowMap(GameObject * pGameObject, const glm::mat4 & pModelMatrix, const glm::mat4 & pViewMatrix, const glm::mat4 & pProjectionMatrix, bool pRecursive)
{
	renderShadow = true;
	renderSelf(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix);
	if (pRecursive) renderChildren(pGameObject, pModelMatrix, pViewMatrix, pProjectionMatrix, pRecursive);
}

#ifdef API_DIRECTX
void Renderer::CloseCommandList()
{
	//new we execute the command list and upload the initial assets (triangle data)
	ThrowIfFailed(commandList->Close());
	ID3D12CommandList* ppCommandLists[] = { commandList };
	commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	//increment the fence value now, otherwise the buffer might not be uploaded by the time we start drawing
	fenceValue[frameIndex]++;
	ThrowIfFailed(commandQueue->Signal(fence[frameIndex], fenceValue[frameIndex]));
}
#endif

void Renderer::renderSelf (GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    renderMesh(pGameObject->constantBufferID, pGameObject->getMesh(), pGameObject->getMaterial(), pModelMatrix, pViewMatrix, pProjectionMatrix);
    //renderMeshDebugInfo(pGameObject->getMesh(), pModelMatrix, pViewMatrix, pProjectionMatrix);
}

void Renderer::renderToShadowMap(Mesh * pMesh, const glm::mat4 & pModelMatrix, const glm::mat4 & pViewMatrix, const glm::mat4 & pProjectionMatrix)
{
	_shadowShader->use();

	glm::mat4 depthMVP = pProjectionMatrix * pViewMatrix * pModelMatrix;

	//pass in all MVP matrices separately
	glUniformMatrix4fv(_shadowShader->getUniformLocation("depthMVP"), 1, GL_FALSE, glm::value_ptr(depthMVP));

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(
		_shadowShader->getAttribLocation("vertex"),
		_shadowShader->getAttribLocation("normal"),
		_shadowShader->getAttribLocation("uv"),
		_shadowShader->getAttribLocation("tangent"),
		_shadowShader->getAttribLocation("bitangent")
	);
}

void Renderer::renderMesh (int pConstantBufferId, Mesh* pMesh, AbstractMaterial* pMaterial, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
#ifdef API_OPENGL
	if (!renderShadow && pMesh != NULL && pMaterial!= NULL && pMaterial->allowedToRender) pMaterial->render(pMesh, pModelMatrix, pViewMatrix, pProjectionMatrix);
	else if (renderShadow && pMesh != NULL && pMaterial != NULL && pMaterial->isShadowCaster && pMaterial->allowedToRender) renderToShadowMap(pMesh, pModelMatrix, pViewMatrix, pProjectionMatrix);
#elif defined(API_DIRECTX)
	ConstantBufferPerObject cb;
	cb.wvpMat = glm::transpose(pProjectionMatrix * pViewMatrix * pModelMatrix);
	// copy our ConstantBuffer instance to the mapped constant buffer resource
	memcpy(cbvGPUAddress[frameIndex] + ConstantBufferPerObjectAlignedSize * pConstantBufferId, &cb, sizeof(cb));
	if (!renderShadow && pMesh != NULL && pMaterial != NULL && pMaterial->allowedToRender) pMaterial->render(pMesh, constantBufferUploadHeaps[frameIndex]->GetGPUVirtualAddress() + ConstantBufferPerObjectAlignedSize * pConstantBufferId);
#endif // API_OPENGL
}

void Renderer::renderMeshDebugInfo (Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (pMesh != NULL) pMesh->drawDebugInfo(pModelMatrix, pViewMatrix, pProjectionMatrix);
}

void Renderer::renderChildren (GameObject* pGameObject, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix, bool pRecursive) {
    int childCount = pGameObject->getChildCount();
    if (childCount < 1) return;

    //note that with a loop like this, deleting children during rendering is not a good idea :)
    GameObject* child = 0;
    for (int i = 0; i < childCount; i++) {
        child = pGameObject->getChildAt(i);

		/*RigidBody* rigidBody = child->getBehaviour<RigidBody>();
		
        if(rigidBody != nullptr) {
            glm::vec3 childScale;
            childScale.x = glm::length( child->getTransform()[0]);
            childScale.y = glm::length( child->getTransform()[1]);
            childScale.z = glm::length( child->getTransform()[2]);
            btTransform trans;
			rigidBody->getMotionState()->getWorldTransform(trans);

            glm::mat4 glmTrans;
            trans.getOpenGLMatrix(glm::value_ptr(glmTrans));
            child->setTransform(glmTrans);
            child->scale(childScale);
        }*/
        render (child, pModelMatrix * child->getTransform(), pViewMatrix, pProjectionMatrix, pRecursive);
    }
}

#ifdef API_DIRECTX
void Renderer::WaitForPreviousFrame() {
	HRESULT hr;

	//if the current fence value is still less than 'fenceValue' then we know the gpu has not finished executing
	//the command queue since it has not reached the 'commandQueue->Signal()' command
	if (fence[frameIndex]->GetCompletedValue() < fenceValue[frameIndex]) {
		//we have the fence create an event which is signaled once the fence's value is 'fenceValue'
		ThrowIfFailed(fence[frameIndex]->SetEventOnCompletion(fenceValue[frameIndex], fenceEvent));

		//we will wait untill the fence has triggered the event once the fence has reached 'fenceValue'
		WaitForSingleObject(fenceEvent, INFINITE);

	}
	//increment fenceValue for next frame
	fenceValue[frameIndex]++;

	//swap the current rtv buffer index so we draw on the correct buffer
	frameIndex = swapChain->GetCurrentBackBufferIndex();
}

#endif // API_DIRECTX
