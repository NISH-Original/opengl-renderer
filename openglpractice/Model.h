#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>

#include "Mesh.h"

using json = nlohmann::json;

class Model
{
public:
	Model(const char* file);

	void Draw(Shader& shader, Camera& camera);

private:
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationMeshes;
	std::vector<glm::quat> rotationMeshes;
	std::vector<glm::vec3> scaleMeshes;
	std::vector<glm::mat4> matrixMeshes;

	std::vector<std::string> loadedTexNames;
	std::vector<Texture> loadedTexs;

	void loadMesh(unsigned int indMesh);

	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	std::vector<unsigned char> getData();
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();

	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

#endif