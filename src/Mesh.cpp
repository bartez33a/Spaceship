#include "..\headers\Mesh.h"

//constructor 
model_mesh::Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
	const std::vector<Texture>& textures)
{
	// save data from arguments
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	// settings for mesh
	setupMesh();
}

// function for drawing mesh
void model_mesh::Mesh::Draw(Shader& shader)
{
	// counters for textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	if (textures.size())
	{
		// iterate over texture vector
		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first

			// retrieve texture number (the N in diffuse_textureN)
			std::string number;
			std::string name = textures[i].type;
			if (name == "texture_diffuse")
			{
				number = std::to_string(diffuseNr++);
				// for selecting sampler in fragment shader
				shader.setUniformInt("tex", diffuseNr - 1);
			}
			/*
			else if (name == "texture_specular")
			{
				number = std::to_string(specularNr++);
				// for selecting sampler in fragment shader
				shader.setUniformInt("tex", specularNr - 1 + 9);
			}
			*/
			shader.setUniformInt((name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
	}
	else // no textures
	{
		shader.setUniformInt("tex", 0);
	}

	// draw mesh
	// bind VAO
	glBindVertexArray(VAO);
	// draw elements
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// unbind vao
	glBindVertexArray(0);

	//activate texture 0
	glActiveTexture(GL_TEXTURE0);
}

// function for setting Mesh
void model_mesh::Mesh::setupMesh()
{
	// generate buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// send data to buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
		&vertices[0], GL_STATIC_DRAW);

	// bind EBO and load data to buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() *
		sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// set attributes
	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)0);

	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, Normal));

	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
		(void*)offsetof(Vertex, TexCoords));

	// unbind VAO
	glBindVertexArray(0);
}
