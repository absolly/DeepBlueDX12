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
#endif // API_DIRECTX

TextureMaterial::TextureMaterial(Texture* pDiffuseTexture, float pTiling, float pSpecularMultiplier, Texture* pSpecularTexture, Texture* pNormalTexture, Texture* pEmissionMap):
_diffuseTexture(pDiffuseTexture), _tiling(pTiling), _specularTexture(pSpecularTexture), _specularMultiplier(pSpecularMultiplier), _normalTexture(pNormalTexture), _emissionMap(pEmissionMap) {
    _lazyInitializeShader();
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
	D3D12_ROOT_DESCRIPTOR rootCBVDescriptor;
	rootCBVDescriptor.RegisterSpace = 0;
	rootCBVDescriptor.ShaderRegister = 0;

	//create the descriptor range and fill it out
	D3D12_DESCRIPTOR_RANGE descriptorTableRanges[1];
	descriptorTableRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //this is the range of shader resource views
	descriptorTableRanges[0].NumDescriptors = 1; //we only have 1 texture right now
	descriptorTableRanges[0].BaseShaderRegister = 0; //start index of the shader registers in the range
	descriptorTableRanges[0].RegisterSpace = 0; //space can usually be 0 according to msdn. don't know why
	descriptorTableRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //appends the range to the end of the root signature descriptor tables

																									   //create the descriptor table
	D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
	descriptorTable.NumDescriptorRanges = _countof(descriptorTableRanges); //only one right now
	descriptorTable.pDescriptorRanges = &descriptorTableRanges[0]; //pointer to the start of the ranges array

																   //create a root parameter
	D3D12_ROOT_PARAMETER rootParameters[2];
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

																		//create a static sampler
	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
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
		_countof(rootParameters), //we have 1 root parameter for now
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
	hr = CompileShaderFromFile(L"VertexShader.hlsl", nullptr, nullptr, "main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader, &errorBuffer);
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
	hr = CompileShaderFromFile(L"PixelShader.hlsl", nullptr, nullptr, "main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader, &errorBuffer);
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

}
#endif // API_OPENGL / API_DIRECTX

void TextureMaterial::setDiffuseTexture (Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void TextureMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_diffuseTexture) return;

	glm::mat3 MVMatrix;

	MVMatrix = glm::mat3(pViewMatrix * pModelMatrix);

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
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, Renderer::shadowDepthTexture);
	glUniform1i(_shader->getUniformLocation("shadowMap"), 3);

	//setup texture slot 4
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, _emissionMap->getId());
	glUniform1i(_shader->getUniformLocation("emissionMap"), 4);

    glUniform1i(_shader->getUniformLocation("lightCount"), sizeof(World::activeLights));

    glm::vec3 lightPosition[2] {};
    glm::vec3 lightDirection[2] {};
    glm::vec3 lightColor[2] {};
    GLint lightType[2] {};
    glm::vec3 lightFalloff[2]{};
    GLfloat lightIntensity[2] {};

	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-600, 600, -600, 600, -800, 800);
	glm::mat4 depthViewMatrix;

    int i = 0;
    for(Light* light : World::activeLights) {
		
        lightPosition[i] = light->getWorldPosition();
		if (light->type == Light::DIRECTIONAL) {
			depthViewMatrix = glm::inverse(light->getWorldTransform());
			lightPosition[i].y = 900;
		}
        lightDirection[i] = light->getWorldTransform()[2]; // * glm::vec4(0,0,1,0);
        lightColor[i] = light->getColor();
        lightType[i] = ((int)light->type);
        lightFalloff[i] = light->falloff;
        lightIntensity[i] = light->intensity;
        i++;
    }

	glm::mat4 depthBiasMVP = biasMatrix * depthProjectionMatrix * depthViewMatrix * pModelMatrix;




    glUniform3fv(_shader->getUniformLocation("lightPosition"), 2, glm::value_ptr(lightPosition[0]));
    glUniform3fv(_shader->getUniformLocation("lightDirection"), 2, glm::value_ptr(lightDirection[0]));
    glUniform3fv(_shader->getUniformLocation("lightColor"), 2, glm::value_ptr(lightColor[0]));
    glUniform1iv(_shader->getUniformLocation("lightType"), 2, lightType);
    glUniform3fv(_shader->getUniformLocation("lightFalloff"), 2, glm::value_ptr(lightFalloff[0]));
    glUniform1fv(_shader->getUniformLocation("lightIntensity"), 2, lightIntensity);
    glUniform1i(_shader->getUniformLocation("lightCount"), i);
    glUniform1i(_shader->getUniformLocation("tiling"), _tiling);
    glUniform1i(_shader->getUniformLocation("specularMultiplier"), _specularMultiplier);
    //pass in all MVP matrices separately
	glUniformMatrix3fv(_shader->getUniformLocation("MVMatrix"),	1, GL_FALSE, glm::value_ptr(MVMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("viewMatrix"),1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_shader->getUniformLocation("depthBiasMVP"), 1, GL_FALSE, glm::value_ptr(depthBiasMVP));

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
	Renderer::commandList->SetGraphicsRootSignature(rootSignature);
	Renderer::commandList->SetPipelineState(pipelineStateObject);

	Renderer::commandList->SetGraphicsRootConstantBufferView(0, pGPUAddress);

	Renderer::commandList->SetGraphicsRootDescriptorTable(1, _diffuseTexture->GetGPUDescriptorHandle());
	//if there is an error here the descriptor heap was not set propperly at the start of the render loop.

	pMesh->streamToDirectX();

}
#endif