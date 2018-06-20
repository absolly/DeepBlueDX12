#include <glm.hpp>

#include "mge/materials/ColorMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"

ShaderProgram* ColorMaterial::_shader = NULL;

GLint ColorMaterial::_uMVPMatrix = 0;
GLint ColorMaterial::_uDiffuseColor = 0;

GLint ColorMaterial::_aVertex = 0;
GLint ColorMaterial::_aNormal = 0;
GLint ColorMaterial::_aUV = 0;
GLint ColorMaterial::_aTangent = 0;
GLint ColorMaterial::_aBitangent = 0;
#ifdef API_DIRECTX
ID3D12PipelineState* ColorMaterial::pipelineStateObject = nullptr;
ID3D12RootSignature* ColorMaterial::rootSignature = nullptr;
ID3D12Resource* ColorMaterial::constantBufferUploadHeaps[Renderer::frameBufferCount] = {};
UINT8* ColorMaterial::cbvGPUAddress[Renderer::frameBufferCount] = {};
#endif // API_DIRECTX
ColorMaterial::ColorMaterial(glm::vec3 pDiffuseColor):_diffuseColor (pDiffuseColor)
{
    //every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
    _lazyInitializeShader();
}

#ifdef API_OPENGL
void ColorMaterial::_lazyInitializeShader() {
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH+"color.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH+"color.fs");
        _shader->finalize();

        //cache all the uniform and attribute indexes
        _uMVPMatrix= _shader->getUniformLocation("mvpMatrix");
        _uDiffuseColor = _shader->getUniformLocation("diffuseColor");

        _aVertex = _shader->getAttribLocation("vertex");
        _aNormal = _shader->getAttribLocation("normal");
        _aUV =     _shader->getAttribLocation("uv");
        _aTangent = _shader->getAttribLocation("tangent");
        _aBitangent = _shader->getAttribLocation("bitangent");
    }
}
#elif defined(API_DIRECTX)
void ColorMaterial::_lazyInitializeShader() {
	D3D12_ROOT_DESCRIPTOR rootCBVDescriptor;
	rootCBVDescriptor.RegisterSpace = 0;
	rootCBVDescriptor.ShaderRegister = 0;

	D3D12_ROOT_DESCRIPTOR materialCBVDescriptor;
	materialCBVDescriptor.RegisterSpace = 0;
	materialCBVDescriptor.ShaderRegister = 1;

	//create a root parameter
	D3D12_ROOT_PARAMETER rootParameters[2];
	//its a good idea to sort the root parameters by frequency of change.
	//the constant buffer will change multiple times per frame but the descriptor table won't change in this case

	//constant buffer view root descriptor
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; //this is a constant buffer view
	rootParameters[0].Descriptor = rootCBVDescriptor;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //only the vertex shader will be able to access the parameter

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; //this is a constant buffer view
	rootParameters[1].Descriptor = materialCBVDescriptor;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //only the vertex shader will be able to access the parameter

																		 //fill out the root signature
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(
		_countof(rootParameters), //we have 1 root parameter for now
		rootParameters, //pointer to the start of the root parameters array
		0, //we have no static sampler
		nullptr, //pointer to our static sampler (array)
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |       // we can deny shader stages here for better performance
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS
	);

	ID3DBlob* errorBuffer; //a buffer holding the error data if any
	ID3DBlob* signature;
	ThrowIfFailed(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &errorBuffer));

 	ThrowIfFailed(Renderer::device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature)));


	//Compile the vertex and pixel shaders

	//when debuging we can compile the shaders at runtime.
	//for release versions it is recommended to compile the hlsl shaders using fxc.exe
	//this creates .cso files that can be loaded in at runtime to get the shader bytecode
	//this is of course faster then compiling them at runtime

	//compile vertex shader
	ID3DBlob* vertexShader; //d3d blob for holding shader bytecode
	HRESULT hr;
	//shader file,		  defines  includes, entry,	sm		  compile flags,							efect flags, shader blob, error blob
	hr = CompileShaderFromFile(L"UnlitVertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader, &errorBuffer);
	if (FAILED(hr)) {
		OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
		ThrowIfFailed(hr); 
	}

	//fill out a shader bytecode structure, which is bascially a pointer
	//to the shader bytecode and syze of shader bytecode
	D3D12_SHADER_BYTECODE vertexShaderBytecode = {};
	vertexShaderBytecode.BytecodeLength = vertexShader->GetBufferSize();
	vertexShaderBytecode.pShaderBytecode = vertexShader->GetBufferPointer();

	// compile pixel shader
	ID3DBlob* pixelShader;
	//shader file,		  defines  includes, entry,	sm		  compile flags,							efect flags, shader blob, error blob
	hr = CompileShaderFromFile(L"ColorPixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader, &errorBuffer);
	if (FAILED(hr)) {
		OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
		ThrowIfFailed(hr);
	}

	// fill out shader bytecode structure for pixel shader
	D3D12_SHADER_BYTECODE pixelShaderBytecode = {};
	pixelShaderBytecode.BytecodeLength = pixelShader->GetBufferSize();
	pixelShaderBytecode.pShaderBytecode = pixelShader->GetBufferPointer();

	//create input layout

	//the input layout is used by the ia so it knows
	//how to read the vertex data bound to it.

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	//fill out an input layout description struct
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};

	//we can get teh number of elements in an array by "sizeof(array)/sizeof(arrayElementType)"
	inputLayoutDesc.NumElements = sizeof(inputLayout) / sizeof(D3D12_INPUT_ELEMENT_DESC);
	inputLayoutDesc.pInputElementDescs = inputLayout;

	// multi-sampling settings (not using it currently)
	DXGI_SAMPLE_DESC sampleDesc = {};
	sampleDesc.Count = 1; // 1 sample count is disables multi-sampling

						  //create a pipeline state object
						  //in real applications you will have many pso's. for each different shader
						  //or different combinations of shaders, different blend states or different rasterizer states,
						  //different topology types (point, line, triangle, patch), or a different number of render targets.

						  // the vertex shader is the only required shader for a pso

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {}; //pso description struct
	psoDesc.InputLayout = inputLayoutDesc;
	psoDesc.pRootSignature = rootSignature;
	psoDesc.VS = vertexShaderBytecode;
	psoDesc.PS = pixelShaderBytecode;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; //format of the rtv
	psoDesc.SampleDesc = sampleDesc;
	psoDesc.SampleMask = 0xffffffff; //sample mask has to do with multi - sampling. 0xffffffff means point sampling
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT); //default rasterizer state
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT); //default blend state
	psoDesc.NumRenderTargets = 1;
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT); //default values for depth and stencil settings are alright for now.
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// create the pso
	ThrowIfFailed(Renderer::device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineStateObject)));

	//create a resource heap, descriptor heap, and pointer to cbv for every framebuffer
	for (int i = 0; i < Renderer::frameBufferCount; i++)
	{
		ThrowIfFailed(Renderer::device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(1024 * 64), //must be a multiple of 64kb thus 64 bytes * 1024 (4mb multiple for multi-sampled textures)
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constantBufferUploadHeaps[i])
		));


		constantBufferUploadHeaps[i]->SetName(L"Material Constant Buffer Upload Resource Heap");

		CD3DX12_RANGE readRange(0, 0); //read range is less then 0, indicates that we will not be reading this resource from the cpu

									   //map the resource heap to get a gpu virtual address to the beginning of the heap
		ThrowIfFailed(constantBufferUploadHeaps[i]->Map(0, &readRange, reinterpret_cast<void**>(&cbvGPUAddress[i])));

	}
}
#endif // API_OPENGL

ColorMaterial::~ColorMaterial()
{
    //dtor
}

void ColorMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) {
    _diffuseColor = pDiffuseColor;
}

void ColorMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    _shader->use();

    //pass in a precalculate mvp matrix (see texture material for the opposite)
    glm::mat4 mvpMatrix = pProjectionMatrix * pViewMatrix * pModelMatrix;
    glUniformMatrix4fv ( _uMVPMatrix, 1, GL_FALSE, glm::value_ptr(mvpMatrix));

    //set the material color
    glUniform3fv (_uDiffuseColor, 1, glm::value_ptr(_diffuseColor/255));

    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV, _aTangent, _aBitangent);
	
}
#ifdef API_DIRECTX
void ColorMaterial::render(Mesh* pMesh, D3D12_GPU_VIRTUAL_ADDRESS pGPUAddress)
{
	Renderer::commandList->SetGraphicsRootSignature(rootSignature);
	Renderer::commandList->SetPipelineState(pipelineStateObject);

	Renderer::commandList->SetGraphicsRootConstantBufferView(0, pGPUAddress);

	memcpy(cbvGPUAddress[0], &(_diffuseColor/255), sizeof(_diffuseColor)); //constant buffer is only one vec3 in this case
	Renderer::commandList->SetGraphicsRootConstantBufferView(1, constantBufferUploadHeaps[0]->GetGPUVirtualAddress() /*+ cbvsize * _id TODO!!!*/);


	pMesh->streamToDirectX();

}
#endif // API_DIRECTX
