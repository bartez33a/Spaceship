#pragma once

//class from book Learn OpenGL - Graphics Programming
// by Joey de Vries

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"

namespace model_mesh{

	// struct for vertices
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};
	// struct for textures
	struct Texture {
		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		//constructor
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
			const std::vector<Texture>& textures);
		// mesh data
		//vector of vertices
		std::vector<Vertex> vertices;
		//vector of indices
		std::vector<unsigned int> indices;
		//vector of textures
		std::vector<Texture> textures;
		// function for drawing mesh
		void Draw(Shader& shader);

	private:
		// rendering data
		//buffers
		unsigned int VAO, VBO, EBO;
		// setup mesh
		void setupMesh();
	};

}
