#include <glm.hpp>

#include "mge/materials/TextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Renderer.hpp"

ShaderProgram* TextureMaterial::_shader = NULL;

#ifdef API_DIRECTX
ID3D12PipelineState* TextureMaterial::pipelineStateObject = nullptr;
ID3D12RootSignature* TextureMaterial::rootSignature = nullptr;
ID3D12Resource* TextureMaterial::constantBufferUploadHeaps[Renderer::frameBufferCount] = {};
UINT8* TextureMaterial::cbvGPUAddress[Renderer::frameBufferCount] = {};
ID3D12DescriptorHeap* TextureMaterial::textureDescriptorHeap = nullptr;
UINT TextureMaterial::mCbvSrvDescriptorSize = 0;

#endif // API_DIRECTX
unsigned int TextureMaterial::_materialCount = 0;

TextureMaterial::TextureMaterial(Texture* pDiffuseTexture, float pTiling, float pSpecularMultiplier, Texture* pSpecularTexture, Texture* pNormalTexture, Texture* pEmissionMap):
_diffuseTexture(pDiffuseTexture), _tiling(pTiling), _specularTexture(pSpecularTexture), _specularMultiplier(pSpecularMultiplier), _normalTexture(pNormalTexture), _emissionMap(pEmissionMap), cbPerMaterial(pSpecularMultiplier, pTiling), _id(_materialCount) {
	_lazyInitializeShader();
#ifdef API_DIRECTX
	_materialCount++;
	mCbvSrvDescriptorSize = Renderer::device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	int offsetFromHeapStart = _id * 4;

	CD3DX12_CPU_DESCRIPTOR_HANDLE hDescriptor(textureDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	hDescriptor.Offset(offsetFromHeapStart /*4 textures*/, mCbvSrvDescriptorSize);
	
	Renderer::device->CreateShaderResourceView(pDiffuseTexture->textureBuffer, &pDiffuseTexture->srvDesc, hDescriptor);
	hDescriptor.Offset(mCbvSrvDescriptorSize);
	Renderer::device->CreateShaderResourceView(pNormalTexture->textureBuffer, &pNormalTexture->srvDesc, hDescriptor);
	hDescriptor.Offset(mCbvSrvDescriptorSize);
	Renderer::device->CreateShaderResourceView(pSpecularTexture->textureBuffer, &pSpecularTexture->srvDesc, hDescriptor);
	hDescriptor.Offset(mCbvSrvDescriptorSize);
	Renderer::device->CreateShaderResourceView(pEmissionMap->textureBuffer, &pEmissionMap->srvDesc, hDescriptor);
#endif // API_DIRECTX

}

TextureMaterial::~TextureMaterial() {}

#ifdef API_OPENGL
void TextureMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, Config::MGE_SHADER_PATH+"texture.vs");
        _shader->addShader(GL_FRAGMENT_SHADER, Config::MGE_SHADER_PATH+"texture.fs");
        _shader->finalize();
    }
}
#elif defined(API_DIRECTX)
void TextureMaterial::_lazyInitializeShader() {
	if (!pipelineStateObject) {
		//create the descriptor heap that will store our shader resource views
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = 256; //max number of textures
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		ThrowIfFailed(Renderer::device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&textureDescriptorHeap)));

		mCbvSrvDescriptorSize = Renderer::device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		D3D12_ROOT_DESCRIPTOR rootCBVDescriptor;
		rootCBVDescriptor.RegisterSpace = 0;
		rootCBVDescriptor.ShaderRegister = 0;


		D3D12_ROOT_DESCRIPTOR materialCBVDescriptor;
		materialCBVDescriptor.RegisterSpace = 0;
		materialCBVDescriptor.ShaderRegister = 1;

		//create the descriptor range and fill it out
		D3D12_DESCRIPTOR_RANGE descriptorTableRanges[1];
		descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //this is the range of shader resource views
		descriptorTableRanges[0].NumDescriptors = 4; //we only have 1 texture right now
		descriptorTableRanges[0].BaseShaderRegister = 0; //start index of the shader registers in the range
		descriptorTableRanges[0].RegisterSpace = 0; //space can usually be 0 according to msdn. don't know why
		descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //appends the range to the end of the root signature descriptor tables

																										   //create the descriptor table
		D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
		descriptorTable.NumDescriptorRanges = _countof(descriptorTableRanges); //only one right now
		descriptorTable.pDescriptorRanges = &descriptorTableRanges[0]; //pointer to the start of the ranges array


																	   //create a root parameter
		D3D12_ROOT_PARAMETER rootParameters[3];
		//its a good idea to sort the root parameters by frequency of change.
		//the constant buffer will change multiple times per frame but the descriptor table won't change in this case

		//constant buffer view root descriptor
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; //this is a constant buffer view
		rootParameters[0].Descriptor = rootCBVDescriptor;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //only the vertex shader will be able to access the parameter

																			 //descriptor table
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1].DescriptorTable = descriptorTable;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //only visible to pixel since this should contain the texture.

		rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; //this is a constant buffer view
		rootParameters[2].Descriptor = materialCBVDescriptor;
		rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //visible to all stages (contains vertex and pixel data)

		//create a static sampler
		D3D12_STATIC_SAMPLER_DESC sampler = {};
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS; //used for more advanced effects like shadow mapping. leave on always for now.
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		//fill out the root signature
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(
			_countof(rootParameters),
			rootParameters, //pointer to the start of the root parameters array
			1, //we have one static sampler
			&sampler, //pointer to our static sampler (array)
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
#ifdef DEBUG
		hr = CompileShaderFromFile(L"LitVertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader, &errorBuffer);
#else
		hr = CompileShaderFromFile(L"LitVertexShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &vertexShader, &errorBuffer);
#endif
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
#ifdef DEBUG
		hr = CompileShaderFromFile(L"LitTextureShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader, &errorBuffer);
#else
		hr = CompileShaderFromFile(L"LitTextureShader.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &pixelShader, &errorBuffer);
#endif
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
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		//fill out an input layout description struct
		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};

		//we can get the number of elements in an array by "sizeof(array)/sizeof(arrayElementType)"
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
				&CD3DX12_RESOURCE_DESC::Buffer(1024 * 64 * 100), //must be a multiple of 64kb thus 64 bytes * 1024 (4mb multiple for multi-sampled textures)
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
}
#endif // API_OPENGL / API_DIRECTX

void TextureMaterial::setDiffuseTexture (Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}



void TextureMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_diffuseTexture) return;

    _shader->use();

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    glUniform1i (_shader->getUniformLocation("textureDiffuse"), 0);

    //setup texture slot 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _specularTexture->getId());
    glUniform1i (_shader->getUniformLocation("textureSpecular"), 1);

    //setup texture slot 2
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _normalTexture->getId());
    glUniform1i (_shader->getUniformLocation("textureNormal"), 2);

	//setup texture slot 3
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, _emissionMap->getId());
	glUniform1i(_shader->getUniformLocation("emissionMap"), 3);

    //glUniform1i(_shader->getUniformLocation("lightCount"), sizeof(World::activeLights));

    glm::vec3 lightPosition[16] {};
    GLfloat falloffStart[16] {}; //radius
    glm::vec3 lightColor[16] {}; // color + intensity
	GLfloat falloffEnd[16]{};
    glm::vec3 lightDirection[16]{};
    GLfloat lightSpotPower[16] {};
	
    int i = 0;
    for(Light* light : World::activeLights) {
		
        lightPosition[i] = light->getWorldPosition();
		falloffStart[i] = light->falloff.x;
        lightColor[i] = light->getColor() * light->intensity;
		falloffEnd[i] = light->falloff.y;
        lightDirection[i] = light->getWorldTransform()[2]; // * glm::vec4(0,0,1,0);
        lightSpotPower[i] = light->angle;
        i++;
    }

    glUniform3fv(_shader->getUniformLocation("lightPosition"), 16, glm::value_ptr(lightPosition[0]));
	glUniform1fv(_shader->getUniformLocation("falloffStart"), 16, falloffStart);
    glUniform3fv(_shader->getUniformLocation("lightColor"), 16, glm::value_ptr(lightColor[0]));
    glUniform1fv(_shader->getUniformLocation("falloffEnd"), 16, falloffEnd);
    glUniform3fv(_shader->getUniformLocation("lightDirection"), 16, glm::value_ptr(lightDirection[0]));
    glUniform1fv(_shader->getUniformLocation("spotPower"), 16, lightSpotPower);
    glUniform1i(_shader->getUniformLocation("tiling"), _tiling);
	glUniform3fv(_shader->getUniformLocation("eyePosW"), 1, glm::value_ptr(World::getMainCamera()->getWorldPosition()));
    glUniform1i(_shader->getUniformLocation("specularMultiplier"), _specularMultiplier);
    //pass in all MVP matrices separately
    glUniformMatrix4fv ( _shader->getUniformLocation("viewMatrix"),1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));

	//std::cout << "ID HERE FUADnwnow iawndI OawndI nawdi Nawdi Naw::::::::::" << _shader->getAttribLocation("tangent") << std::endl;
    //now inform mesh of where to stream its data
    pMesh->streamToOpenGL(
        _shader->getAttribLocation("vertex"),
        _shader->getAttribLocation("normal"),
        _shader->getAttribLocation("uv"),
        _shader->getAttribLocation("tangent"),
        _shader->getAttribLocation("bitangent")
    );
}
#ifdef API_DIRECTX
void TextureMaterial::render(Mesh* pMesh, D3D12_GPU_VIRTUAL_ADDRESS pGPUAddress)
{
	//set the descriptor heap
	ID3D12DescriptorHeap* descriptorHeaps[] = { textureDescriptorHeap };
	Renderer::commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	//todo: do this outside of render loop
	//lights could be passed once per frame instead of once per object.
	cbPerMaterial.eyePosW = World::getMainCamera()->getWorldPosition();
	int i = 0;
	for (Light* light : World::activeLights) {

		LightBase base;
		cbPerMaterial.lights[i].Position = light->getWorldPosition();
		cbPerMaterial.lights[i].falloffStart = light->falloff.x;
		cbPerMaterial.lights[i].Direction = glm::vec3(-light->getWorldTransform()[2]);
		cbPerMaterial.lights[i].falloffEnd = light->falloff.y;
		cbPerMaterial.lights[i].Color = glm::vec3(light->getColor()) * light->intensity;
		cbPerMaterial.lights[i].Angle = light->angle;
		i++;
	}
	//could be done once per material instead of per object.

	Renderer::commandList->SetGraphicsRootSignature(rootSignature);
	Renderer::commandList->SetPipelineState(pipelineStateObject);

	Renderer::commandList->SetGraphicsRootConstantBufferView(0, pGPUAddress);
	memcpy(cbvGPUAddress[0] + cbPerMatAlignedSize * _id, &cbPerMaterial, sizeof(cbPerMaterial)); //material's constant buffer data
	Renderer::commandList->SetGraphicsRootConstantBufferView(2, constantBufferUploadHeaps[0]->GetGPUVirtualAddress() + cbPerMatAlignedSize * _id);

	//textures
	CD3DX12_GPU_DESCRIPTOR_HANDLE hDescriptor(textureDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
	hDescriptor.Offset(_id * 4, mCbvSrvDescriptorSize);
	Renderer::commandList->SetGraphicsRootDescriptorTable(1, hDescriptor);
	
	//if there is an error here the descriptor heap was not set propperly at the start of the render loop.

	pMesh->streamToDirectX();

}
#endif