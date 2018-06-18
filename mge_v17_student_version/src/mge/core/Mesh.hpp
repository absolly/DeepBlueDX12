#ifndef MESH_H
#define MESH_H

#include <vector>
#include <GL/glew.h>
#include <glm.hpp>
#include "mge/core/Renderer.hpp"

class GameObject;
class World;

struct Vertex
{
	Vertex(glm::vec3 pPos, glm::vec2 pTexCoord, glm::vec3 pNormal, glm::vec3 pTangent, glm::vec3 pBitangent)
		: pos(pPos), texCoord(pTexCoord), normal(pNormal), tangent(pTangent), bitangent(pBitangent) {}

	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

/**
 * A mesh represents an .OBJ file. It knows how it is constructed, how its data should be buffered to OpenGL
 * and how it should be streamed to OpenGL
 */
class Mesh
{
	public:
		Mesh(std::string pId);
		virtual ~Mesh();

        /**
         * Loads a mesh from an .obj file. The file has to have:
         * vertexes, uvs, normals and face indexes. See load source
         * for more format information.
         */
		static Mesh* load(std::string pFileName, bool pDoBuffer = true);
		void _sendDataToOpenGL(glm::mat4 pProjectionMatrix, glm::mat4 pViewMatrix, std::vector<GameObject*> pGameObjects);

        /**
         * Streams the mesh to opengl using the given indexes for the different attributes
         */
        void streamToOpenGL(GLint pVerticesAttrib, GLint pNormalsAttrib, GLint pUVsAttrib, GLint pTangentAttrib, GLint pBitangentAttrib);
		void streamToDirectX();

		void instanceToOpenGL(GLint pVerticesAttrib, GLint pNormalsAttrib, GLint pUVsAttrib, GLint pTangentAttrib, GLint pBitangentAttrib);

		void drawInstancedmesh();

		void DisableVertexAttribArrays();

        /**
         * Draws debug info (normals) for the mesh using the given matrices)
         */
        void drawDebugInfo(const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix);

		class btCollisionShape* getMeshCollisionShape();

		std::vector<glm::vec3>* getVerticies();
		std::vector<unsigned>* getVertextIndices();

	protected:

	    std::string _id;

        //OpenGL id's for the different buffers created for this mesh
		GLuint _indexBufferId;
		GLuint _vertexBufferId;
		GLuint _normalBufferId;
		GLuint _tangentBufferId;
		GLuint _bitangentBufferId;
		GLuint _uvBufferId;

		GLuint _uvattr;
		GLuint _normalattr;
		GLuint _verticesattrb;

	    //the actual data
		std::vector<glm::vec3> _vertices;       //vec3 with 3d coords for all vertices
		std::vector<glm::vec3> _normals;        //vec3 with 3d normal data
		std::vector<glm::vec3> _tangents;        //vec3 with 3d tangent data
		std::vector<glm::vec3> _bitangents;        //vec3 with 3d bitangent data
		std::vector<glm::vec2> _uvs;            //vec2 for uv
		std::vector<Vertex> _vertexData;
		//references to the vertices/normals & uvs in previous vectors
		std::vector<unsigned int> _indices;

		int numIndices;

		ID3D12Resource* vertexBuffer; //a default buffer in gpu memory that we will load the vertex data into

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView; //a structure containing a pointer to the vertex data in gpu memory (to be used by the driver), 
												   //the total size of the buffer, and the size of each element

		ID3D12Resource* indexBuffer; //a default buffer in gpu memory that we will load index data into

		D3D12_INDEX_BUFFER_VIEW indexBufferView; //a stucture holding info about the index buffer

        //buffer vertices, normals, and uv's
		void _buffer();

        //Please read the "load" function documentation on the .obj file format first.
        //FaceVertexTriplet  is a helper class for loading and converting to obj file to
        //indexed arrays.
        //If we list all the unique v/uv/vn triplets under the faces
        //section in an object file sequentially and assign them a number
        //it would be a map of FaceVertexTriplet. Each FaceVertexTriplet refers
        //to an index with the originally loaded vertex list, normal list and uv list
        //and is only used during conversion (unpacking) of the facevertextriplet list
        //to a format that OpenGL can handle.
        //So for a vertex index a FaceVertexTriplet contains the index for uv and n as well.
		class FaceIndexTriplet {
			public:
				unsigned v; //vertex
				unsigned uv;//uv
				unsigned n; //normal
				FaceIndexTriplet( unsigned pV, unsigned pUV, unsigned pN )
				:	v(pV),uv(pUV),n(pN) {
				}
				//needed for use as key in map
				bool operator<(const FaceIndexTriplet other) const{
					return memcmp((void*)this, (void*)&other, sizeof(FaceIndexTriplet))>0;
				}
		};

		//upload data to constant buffer
		static ID3D12Resource* CreateDefaultBuffer(
			ID3D12Device* device,
			ID3D12GraphicsCommandList* cmdList,
			const void* initData,
			UINT64 byteSize,
			ID3D12Resource*& uploadBuffer
		);
};

#endif // MESH_H
