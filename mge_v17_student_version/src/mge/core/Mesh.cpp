#include "mge/core/Mesh.hpp"
#include <iostream>
#include <map>
#include <string>
#include <mge/core/GameObject.hpp>
#include <fstream>
#include "btBulletDynamicsCommon.h"

using namespace std;


Mesh::Mesh(string pId)
	: _id(pId), _indexBufferId(0), _vertexBufferId(0), _normalBufferId(0), _uvBufferId(0), _tangentBufferId(0), _bitangentBufferId(0),
	_vertices(), _normals(), _uvs(), _indices(), _tangents(), _bitangents() {
	//ctor
}

Mesh::~Mesh() {
	//dtor
}

/**
 * Load reads the obj data into a new mesh using C++ combined with c style coding.
 * The result is an indexed mesh for use with glDrawElements.
 * Expects a obj file with following layout v/vt/vn/f eg
 *
 * For example the obj file for a simple plane describes two triangles, based on
 * four vertices, with 4 uv's all having the same vertex normals (NOT FACE NORMALS!)
 *
 * v 10.000000 0.000000 10.000000              //vertex 1
 * v -10.000000 0.000000 10.000000             //vertex 2
 * v 10.000000 0.000000 -10.000000             //vertex 3
 * v -10.000000 0.000000 -10.000000            //vertex 4
 * vt 0.000000 0.000000                        //uv 1
 * vt 1.000000 0.000000                        //uv 2
 * vt 1.000000 1.000000                        //uv 3
 * vt 0.000000 1.000000                        //uv 4
 * vn 0.000000 1.000000 -0.000000              //normal 1 (normal for each vertex is same)
 * s off
 *
 * Using these vertices, uvs and normals we can construct faces, made up of 3 triplets (vertex, uv, normal)
 * f 2/1/1 1/2/1 3/3/1                         //face 1 (triangle 1)
 * f 4/4/1 2/1/1 3/3/1                         //face 2 (triangle 2)
 *
 * So although this is a good format for blender and other tools reading .obj files, this is
 * not an index mechanism that OpenGL supports out of the box.
 * The reason is that OpenGL supports only one indexbuffer, and the value at a certain point in the indexbuffer, eg 3
 * refers to all three other buffers (v, vt, vn) at once,
 * eg if index[0] = 5, opengl will stream vertexBuffer[5], uvBuffer[5], normalBuffer[5] into the shader.
 *
 * So what we have to do after reading the file with all vertices, is construct unique indexes for
 * all pairs that are described by the faces in the object file, eg if you have
 * f 2/1/1 1/2/1 3/3/1                         //face 1 (triangle 1)
 * f 4/4/1 2/1/1 3/3/1                         //face 2 (triangle 2)
 *
 * v/vt/vn[0] will represent 2/1/1
 * v/vt/vn[1] will represent 1/2/1
 * v/vt/vn[2] will represent 3/3/1
 * v/vt/vn[3] will represent 4/4/1
 *
 * and that are all unique pairs, after which our index buffer can contain:
 *
 * 0,1,2,3,0,2
 *
 * So the basic process is, read ALL data into separate arrays, then use the faces to
 * create unique entries in a new set of arrays and create the indexbuffer to go along with it.
 *
 * Note that loading this mesh isn't cached like we do with texturing, this is an exercise left for the students.
 */
Mesh* Mesh::load(string pFileName, bool pDoBuffer) {
	//cout << "Loading " << pFileName << "...";

	Mesh* mesh = new Mesh(pFileName);

	ifstream file(pFileName, ios::in);

	if (file.is_open()) {
		//these three vectors will contains data as taken from the obj file
		//in the order it is encountered in the object file
		vector<glm::vec3> vertices;
		vector<glm::vec3> fs;
		vector<glm::vec2> uvs;
		vector<glm::vec3> normals;

		//in addition we create a map to store the triplets found under the f(aces) section in the
		//object file and map them to an index for our index buffer (just number them sequentially
		//as we encounter them and store references to the pack
		map <FaceIndexTriplet, unsigned int> mappedTriplets;

		string line; // to store each line in
		while (getline(file, line)) {

			// c-type string to store cmd read from obj file (cmd is v, vt, vn, f)
			char cmd[10];
			cmd[0] = 0;

			//get the first string in the line of max 10 chars (c-style)
			sscanf(line.c_str(), "%10s", cmd);

			//note that although the if statements below seem to imply that we can
			//read these different line types (eg vertex, normal, uv) in any order,
			//this is just convenience coding for us (instead of multiple while loops)
			//we assume the obj file to list ALL v lines first, then ALL vt lines,
			//then ALL vn lines and last but not least ALL f lines last

			//so... start processing lines
			//are we reading a vertex line? straightforward copy into local vertices vector
			if (strcmp(cmd, "v") == 0) {
				glm::vec3 vertex;
				sscanf(line.c_str(), "%10s %f %f %f ", cmd, &vertex.x, &vertex.y, &vertex.z);
				vertices.push_back(vertex);

				//or are we reading a normal line? straightforward copy into local normal vector
			}
			else if (strcmp(cmd, "vn") == 0) {
				glm::vec3 normal;
				sscanf(line.c_str(), "%10s %f %f %f ", cmd, &normal.x, &normal.y, &normal.z);
				normals.push_back(normal);

				//or are we reading a uv line? straightforward copy into local uv vector
			}
			else if (strcmp(cmd, "vt") == 0) {
				glm::vec2 uv;
				sscanf(line.c_str(), "%10s %f %f ", cmd, &uv.x, &uv.y);
				uvs.push_back(uv);

				//this is where it gets nasty. After having read all vertices, normals and uvs into
				//their own buffer
			}
			else if (strcmp(cmd, "f") == 0) {

				//an f lines looks like
				//f 2/1/1 1/2/1 3/3/1
				//in other words
				//f v1/u1/n1 v2/u2/n2 v3/u3/n3
				//for each triplet like that we need to check whether we already encountered it
				//and update our administration based on that
				glm::ivec4 vertexIndex;
				glm::ivec4 normalIndex;
				glm::ivec4 uvIndex;
				int count = sscanf(line.c_str(), "%10s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", cmd, &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2], &vertexIndex[3], &uvIndex[3], &normalIndex[3]);

				//Have we read exactly 10 elements?
				if (count == 10 || count == 13) {
					//process 3 triplets, one for each vertex (which is first element of the triplet)
					//cout << count << endl;
					int vertCount = (vertexIndex[3]) ? 6 : 3;

					glm::vec3 edge1 = vertices[vertexIndex[1] - 1] - vertices[vertexIndex[0] - 1];
					glm::vec3 edge2 = vertices[vertexIndex[2] - 1] - vertices[vertexIndex[0] - 1];
					glm::vec2 deltaUV1 = uvs[uvIndex[1] - 1] - uvs[uvIndex[0] - 1];
					glm::vec2 deltaUV2 = uvs[uvIndex[2] - 1] - uvs[uvIndex[0] - 1];


					GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

					glm::vec3 tangent;
					glm::vec3 bitangent;
					tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
					tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
					tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
					tangent = glm::normalize(tangent);

					bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
					bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
					bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
					bitangent = glm::normalize(bitangent);

					for (int i = 0; i < vertCount; ++i) {
						int newIndex = i;
#ifdef API_OPENGL
						switch (newIndex) {
						case 3:
							newIndex = 0;
							break;
						case 4:
							newIndex = 2;
							break;
						case 5:
							newIndex = 3;
							break;
						}
#elif defined(API_DIRECTX)
						//change order so the system traverses them clockwise from the front https://msdn.microsoft.com/en-us/library/windows/desktop/bb204853(v=vs.85).aspx
						switch (newIndex) {
						case 1:
							newIndex = 2;
							break;
						case 2:
							newIndex = 1;
							break;
						case 3:
							newIndex = 0;
							break;
						case 4:
							newIndex = 3;
							break;
						case 5:
							newIndex = 2;
							break;
						}
#endif

						vertexIndex[newIndex] = (vertexIndex[newIndex] < 0) ? vertices.size() + vertexIndex[newIndex] + 1 : vertexIndex[newIndex];
						uvIndex[newIndex] = (uvIndex[newIndex] < 0) ? uvs.size() + uvIndex[newIndex] + 1 : uvIndex[newIndex];
						normalIndex[newIndex] = (normalIndex[newIndex] < 0) ? normals.size() + normalIndex[newIndex] + 1 : normalIndex[newIndex];

						if (vertexIndex[newIndex] > vertices.size() || uvIndex[newIndex] > uvs.size() || normalIndex[newIndex] > normals.size()) {
							//If we read a different amount, something is wrong
							cout << "Error reading obj: cannot work with negative indices" << endl;
							delete mesh;
							return NULL;

						}
						//create key out of the triplet and check if we already encountered this before
						FaceIndexTriplet triplet(vertexIndex[newIndex], uvIndex[newIndex], normalIndex[newIndex]);
						map<FaceIndexTriplet, unsigned int>::iterator found = mappedTriplets.find(triplet);

						//if iterator points at the end, we haven't found it
						if (found == mappedTriplets.end()) {
							//so create a new index value, and map our triplet to it
							unsigned int index = mappedTriplets.size();
							mappedTriplets[triplet] = index;

							//now record this index
							mesh->_indices.push_back(index);
							//and store the corresponding vertex/normal/uv values into our own buffers
							//note the -1 is required since all values in the f triplets in the .obj file
							//are 1 based, but our vectors are 0 based
							mesh->_vertices.push_back(vertices[vertexIndex[newIndex] - 1]);
							mesh->_normals.push_back(normals[normalIndex[newIndex] - 1]);
							mesh->_tangents.push_back(tangent);
							mesh->_bitangents.push_back(bitangent);
							mesh->_uvs.push_back(uvs[uvIndex[newIndex] - 1]);
							mesh->_vertexData.push_back(Vertex(vertices[vertexIndex[newIndex] - 1], uvs[uvIndex[newIndex] - 1], normals[normalIndex[newIndex] - 1], tangent, bitangent));
						}
						else {
							//if the key was already present, get the index value for it
							unsigned int index = found->second;
							//and update our index buffer with it
							mesh->_indices.push_back(index);
						}
					}
				}
				else {
					//If we read a different amount, something is wrong
					cout << "Error reading obj, needing v,vn,vt" << endl;
					delete mesh;
					return NULL;
				}
			}

		}

		file.close();

		if (pDoBuffer)
			mesh->_buffer();

		//cout << "Mesh loaded and buffered:" << (mesh->_indices.size() / 3.0f) << " triangles." << endl;
		return mesh;
	}
	else {
		cout << "Could not read " << pFileName << endl;
		delete mesh;
		return NULL;
	}
}

void Mesh::_sendDataToOpenGL(glm::mat4 pProjectionMatrix, glm::mat4 pViewMatrix, std::vector<GameObject*> pGameObjects)
{
	glGenBuffers(1, &_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (char*)(sizeof(float) * 3));

	glGenBuffers(1, &_indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

	GLuint transformationMatrixBufferID;
	glGenBuffers(1, &transformationMatrixBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, transformationMatrixBufferID);


	glm::mat4 fullTransforms[] =
	{
		pProjectionMatrix * pViewMatrix * pGameObjects.at(0)->getTransform(),
		pProjectionMatrix * pViewMatrix * pGameObjects.at(1)->getTransform(),
		pProjectionMatrix * pViewMatrix * pGameObjects.at(2)->getTransform()
	};

	//const int fulltransformSize = pGameObjects.size();

	//glm::mat4 fullTransforms[fulltransformSize] = {};

	glBufferData(GL_ARRAY_BUFFER, sizeof(fullTransforms), fullTransforms, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 0));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 4));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 8));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 12));
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(2, 1);
	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
}

#ifdef API_OPENGL
void Mesh::_buffer() {
	glGenBuffers(1, &_indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_normalBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _normalBufferId);
	glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(glm::vec3), &_normals[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_tangentBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _tangentBufferId);
	glBufferData(GL_ARRAY_BUFFER, _tangents.size() * sizeof(glm::vec3), &_tangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_bitangentBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _bitangentBufferId);
	glBufferData(GL_ARRAY_BUFFER, _bitangents.size() * sizeof(glm::vec3), &_bitangents[0], GL_STATIC_DRAW);

	glGenBuffers(1, &_uvBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _uvBufferId);
	glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(glm::vec2), &_uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
#elif defined(API_DIRECTX)
void Mesh::_buffer() {
	//std::vector<Vertex> vList = _vertexData;

	int vBufferSize = _vertexData.size() * sizeof(Vertex);
	ID3D12Resource* vBufferUploadHeap;

	//create the default buffer for the vertex data and upload the data using an upload buffer.
	vertexBuffer = CreateDefaultBuffer(Renderer::device, Renderer::commandList, &_vertexData[0], vBufferSize, vBufferUploadHeap);

	////transition the vertex buffer data from copy destination state to vertex buffer state
	Renderer::commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(vertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

	//create index buffer
	//std::vector <DWORD> iList = _indices;

	int iBufferSize = sizeof(DWORD) * _indices.size();

	numIndices = _indices.size(); //the number of indeces we want to draw (size of the (iList)/(size of one float3) i think)

	ID3D12Resource* iBufferUploadHeap;
	indexBuffer = CreateDefaultBuffer(Renderer::device, Renderer::commandList, &_indices[0], iBufferSize, iBufferUploadHeap);

	//transition index buffer data from copy to index buffer state
	Renderer::commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(indexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));

	//create a vertex buffer view for the triangle. we get the gpu memory address to the vertex pointer using the GetGPUVirtualAddress() method
	vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
	vertexBufferView.StrideInBytes = sizeof(Vertex);
	vertexBufferView.SizeInBytes = vBufferSize;

	//create a index buffer view for the triangle. gets the gpu memory address to the pointer.
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = iBufferSize;
}
#endif // API_OPENGL

void Mesh::streamToOpenGL(GLint pVerticesAttrib, GLint pNormalsAttrib, GLint pUVsAttrib, GLint pTangentAttrib, GLint pBitangentAttrib) {

	if (pVerticesAttrib != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
		glEnableVertexAttribArray(pVerticesAttrib);
		glVertexAttribPointer(pVerticesAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (pNormalsAttrib != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, _normalBufferId);
		glEnableVertexAttribArray(pNormalsAttrib);
		glVertexAttribPointer(pNormalsAttrib, 3, GL_FLOAT, GL_TRUE, 0, 0);
	}

	if (pUVsAttrib != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferId);
		glEnableVertexAttribArray(pUVsAttrib);
		glVertexAttribPointer(pUVsAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (pTangentAttrib != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, _tangentBufferId);
		glEnableVertexAttribArray(pTangentAttrib);
		glVertexAttribPointer(pTangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (pBitangentAttrib != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, _bitangentBufferId);
		glEnableVertexAttribArray(pBitangentAttrib);
		glVertexAttribPointer(pBitangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);

	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);

	// no current buffer, to avoid mishaps, very important for performance

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//fix for serious performance issue
	if (pUVsAttrib != -1) glDisableVertexAttribArray(pUVsAttrib);
	if (pNormalsAttrib != -1) glDisableVertexAttribArray(pNormalsAttrib);
	if (pVerticesAttrib != -1) glDisableVertexAttribArray(pVerticesAttrib);
	if (pTangentAttrib != -1) glDisableVertexAttribArray(pTangentAttrib);
	if (pBitangentAttrib != -1) glDisableVertexAttribArray(pBitangentAttrib);
}

void Mesh::streamToDirectX()
{
#ifdef API_DIRECTX
	Renderer::commandList->IASetVertexBuffers(0, 1, &vertexBufferView);
	Renderer::commandList->IASetIndexBuffer(&indexBufferView);
	Renderer::commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
#endif
}

void Mesh::instanceToOpenGL(GLint pVerticesAttrib, GLint pNormalsAttrib, GLint pUVsAttrib, GLint pTangentAttrib, GLint pBitangentAttrib) {
	if (pVerticesAttrib != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);
		glEnableVertexAttribArray(pVerticesAttrib);
		glVertexAttribPointer(pVerticesAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	if (pNormalsAttrib != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, _normalBufferId);
		glEnableVertexAttribArray(pNormalsAttrib);
		glVertexAttribPointer(pNormalsAttrib, 3, GL_FLOAT, GL_TRUE, 0, 0);
	}

	if (pUVsAttrib != -1) {
		glBindBuffer(GL_ARRAY_BUFFER, _uvBufferId);
		glEnableVertexAttribArray(pUVsAttrib);
		glVertexAttribPointer(pUVsAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);

	//if (pTangentAttrib != -1) {
	//	glBindBuffer(GL_ARRAY_BUFFER, _tangentBufferId);
	//	glEnableVertexAttribArray(pTangentAttrib);
	//	glVertexAttribPointer(pTangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//}

	//if (pBitangentAttrib != -1) {
	//	glBindBuffer(GL_ARRAY_BUFFER, _bitangentBufferId);
	//	glEnableVertexAttribArray(pBitangentAttrib);
	//	glVertexAttribPointer(pBitangentAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//}

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferId);

	//glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);

	// no current buffer, to avoid mishaps, very important for performance

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//fix for serious performance issue
	/*if (pUVsAttrib != -1) glDisableVertexAttribArray(pUVsAttrib);
	if (pNormalsAttrib != -1) glDisableVertexAttribArray(pNormalsAttrib);
	if (pVerticesAttrib != -1) glDisableVertexAttribArray(pVerticesAttrib);*/

	_uvattr = pUVsAttrib;
	_normalattr = pNormalsAttrib;
	_verticesattrb = pVerticesAttrib;
}

void Mesh::drawInstancedmesh()
{
	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
}

void Mesh::DisableVertexAttribArrays()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (_uvattr != -1) glDisableVertexAttribArray(_uvattr);
	if (_normalattr != -1) glDisableVertexAttribArray(_normalattr);
	if (_verticesattrb != -1) glDisableVertexAttribArray(_verticesattrb);
}




void Mesh::drawDebugInfo(const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	//demo of how to render some debug info using the good ol' direct rendering mode...
	glUseProgram(0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(pProjectionMatrix));
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(pViewMatrix * pModelMatrix));

	glBegin(GL_LINES);
	//for each index draw the normal starting at the corresponding vertex
	for (size_t i = 0; i < _indices.size(); i++) {
		//draw normal for vertex
		if (true) {
			//now get normal end
			glm::vec3 normal = _normals[_indices[i]];
			glColor3fv(glm::value_ptr(normal));

			glm::vec3 normalStart = _vertices[_indices[i]];
			glVertex3fv(glm::value_ptr(normalStart));
			glm::vec3 normalEnd = normalStart + normal * 0.2f;
			glVertex3fv(glm::value_ptr(normalEnd));
		}

	}
	glEnd();
}

btCollisionShape* Mesh::getMeshCollisionShape()
{
	std::vector<unsigned>& indices2 = *getVertextIndices();
	std::vector<glm::vec3>& verticies = *getVerticies();

	unsigned int index_count = indices2.size();
	unsigned int vertex_count = verticies.size();
	unsigned *indices = &indices2[0];
	glm::vec3 *vertices = &verticies[0];

	unsigned int numFaces = index_count / 3;
	int vertStride = sizeof(glm::vec3);
	int indexStride = 3 * sizeof(unsigned);

	btTriangleIndexVertexArray* va = new btTriangleIndexVertexArray(
		numFaces,
		(int*)indices,
		indexStride,
		vertex_count,
		(btScalar*)vertices,
		vertStride);
	return new btBvhTriangleMeshShape(va, true);
}

std::vector<glm::vec3>* Mesh::getVerticies()
{
	return &_vertices;
}

std::vector<unsigned>* Mesh::getVertextIndices()
{
	return &_indices;
}

ID3D12Resource* Mesh::CreateDefaultBuffer(
	ID3D12Device* device,
	ID3D12GraphicsCommandList* cmdList,
	const void* initData,
	UINT64 byteSize,
	ID3D12Resource*& uploadBuffer)
{
	ID3D12Resource* defaultBuffer;

	// Create the actual default buffer resource.
	ThrowIfFailed(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&defaultBuffer)));

	// In order to copy CPU memory data into our default buffer, we need to create
	// an intermediate upload heap. 
	ThrowIfFailed(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(byteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&uploadBuffer)));


	// Describe the data we want to copy into the default buffer.
	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = initData;
	subResourceData.RowPitch = byteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	// Schedule to copy the data to the default buffer resource.  At a high level, the helper function UpdateSubresources
	// will copy the CPU memory into the intermediate upload heap.  Then, using ID3D12CommandList::CopySubresourceRegion,
	// the intermediate upload heap data will be copied to mBuffer.
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	UpdateSubresources<1>(cmdList, defaultBuffer, uploadBuffer, 0, 0, 1, &subResourceData);
	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

	// Note: uploadBuffer has to be kept alive after the above function calls because
	// the command list has not been executed yet that performs the actual copy.
	// The caller can Release the uploadBuffer after it knows the copy has been executed.


	return defaultBuffer;
}

