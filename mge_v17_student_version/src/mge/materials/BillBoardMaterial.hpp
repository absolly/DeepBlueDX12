#ifndef BILLBOARDGMATERIAL_H
#define BILLBOARDMATERIAL_H

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Camera.hpp"
#include "Content\GameObjects\ParticleSystem.hpp"
#include <vector>

/**
 * Simple single color material.
 * This material uses no lights.
 * This material demonstrates how to render a single color material, and how to cache all attributes, uniforms and pass in a single MVP matrix to the shader.
 */
class BillBoardMaterial : public AbstractMaterial
{
    public:

	//GPUinstancingMaterial(std::vector<GameObject*> gameObjects);
	BillBoardMaterial(ParticleSystem * pParticleSystem);
	virtual ~BillBoardMaterial();

	virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;


    private:

	int _listSize = 0;
	
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();
	std::vector<GameObject*> _gameObjects;
	ParticleSystem * _particleSystem;
	glm::vec3 CameraUpWorldSpace;
	Camera * _camera;

	static GLint _uVPMatrix;
	static GLuint billboard_vertex_buffer;

	//static GLint _aVertex;
	//static GLint _aNormal;
	//static GLint _aUV;
	//static GLint _aTangent;
	//static GLint _aBitangent;

	static GLint CameraUp;
	static GLint CameraRight;
	static GLint BillboardPos;
	static GLint BillboardSize;
 
};

#endif // COLORMATERIAL_H
