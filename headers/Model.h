#pragma once

//class from book Learn OpenGL - Graphics Programming
// by Joey de Vries

#include "Shader.h"
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <iostream> //cout
#include "stb_image.h"

unsigned int TextureFromFile(const char* path, const std::string& directory);

class Model
{
public:
	// constructor
	Model(std::string path, bool inverse_x, bool inverse_y, bool inverse_z);
	//move constructor
	Model(Model&&) = delete;
	//copy constructor
	Model(const Model& model);
	// function for drawing model
	void Draw(Shader& shader, float scale);
	// function which returns bounding box of object
	glm::mat3x2 getBoundingBox() const;
private:
	//forward declararion of struct
	struct boundingBox;
	// model data
	// vector of model's meshes
	std::vector<model_mesh::Mesh> meshes;
	// directory of model file
	std::string directory;
	// function for loading model indicated by path
	void loadModel(std::string path);
	// fuction for processing nodes
	void processNode(aiNode* node, const aiScene* scene);
	// function for processing node's meshes
	model_mesh::Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	// function for loading textures
	std::vector<model_mesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	// vector for storing loaded textures
	std::vector<model_mesh::Texture> textures_loaded;
	// function for finding bounding box of object
	void findBoundingBox(glm::vec3 vertex, float scale);
	// scale of model
	float m_scale;
	// struct for bounding box
	struct boundingBox {
		float x_min;
		float x_max;
		float y_min;
		float y_max;
		float z_min;
		float z_max;
	};
	//bounding box object
	boundingBox m_boundingBox;

	// inverse model cooridantes in X axis
	bool inverse_x;
	// inverse model cooridantes in Y axis
	bool inverse_y;
	// inverse model cooridantes in Z axis
	bool inverse_z;
};