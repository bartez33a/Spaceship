#include "../headers/Model.h"

// constructor for model
// path -> path to model file
// inverse_x,y,z -> if true: inverse coordinates of indicated axis of model while loading object
Model::Model(std::string path, bool inverse_x, bool inverse_y, bool inverse_z)
{
	// first flip model
	this->inverse_x = inverse_x;
	this->inverse_y = inverse_y;
	this->inverse_z = inverse_z;
	//then load model
	loadModel(path);
	// make sure to find bounding box
	m_scale = -1.0f;
}

// copy constructor of model, so we dont have to load model from file each time
Model::Model(const Model& model):
	// vector of meshes
	meshes{model.meshes},
	// directory of model file
	directory{model.directory},
	// vector for storing loaded textures
	textures_loaded{model.textures_loaded},
	m_scale{ model.m_scale },
	m_boundingBox{model.m_boundingBox},
	inverse_x{ model.inverse_x },
	inverse_y{ model.inverse_y },
	inverse_z{ model.inverse_z }
{
}

// function for drawing model -> it draws all meshes and find bounding box of object
void Model::Draw(Shader& shader, float scale, glm::mat4& modelMatrix)
{
	// if scale has changed
	if (m_scale != scale)
	{
		m_scale = scale;
		for (unsigned int i = 0; i < meshes.size(); i++)
		{
			for (unsigned int j = 0; j < meshes[i].vertices.size(); j++)
			{
				model_mesh::Vertex vertex = meshes[i].vertices[j];
				this->findBoundingBox(vertex.Position, m_scale, modelMatrix);
			}	
		}
		glm::mat3x2 bb = this->getBoundingBox();
	}
	// draw each mesh
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		shader.setUniformFloat("scale", m_scale);
		meshes[i].Draw(shader);
	}
}

// getBoundingBox function returns bounding box of object
// as glm::mat3x2 object:
// Box[0].x, Box[1].x, Box[2].x -> minimal value of x,y,z coordinates
// Box[0].y, Box[1].y, Box[2].y -> maximal value of x,y,z coordinates
glm::mat3x2 Model::getBoundingBox() const
{
	// 3rows, 2 columns 
	// Box[0] - column 0
	glm::mat3x2 Box;
	
	// min x,y,z
	Box[0].x = m_boundingBox.min.x;
	Box[1].x = m_boundingBox.min.y;
	Box[2].x = m_boundingBox.min.z;
	// max x,y,z
	Box[0].y = m_boundingBox.max.x;
	Box[1].y = m_boundingBox.max.y;
	Box[2].y = m_boundingBox.max.z;
	return Box;
}

// fuction for loading model indicated by path
void Model::loadModel(std::string path)
{
	//importer object for loading models
	Assimp::Importer importer;
	// loaded model is stored in data type called scene object
	// primitive -> triangles
	// flip loaded textures vertically
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	//check if loaded was successfull
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << '\n';
		return;
	}

	// save directory path
	directory = path.substr(0, path.find_last_of('/'));

	// and process root node (and recursively all children!)
	processNode(scene->mRootNode, scene);
}

// function for processing nodes (and node's children)
void Model::processNode(aiNode* node, const aiScene* scene)
{
	// process all the node’s meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// get each mesh
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		// and add it to vector of meshes
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		// recursively call the same function for chilren!
		processNode(node->mChildren[i], scene);
	}

}

// function for porcessing each mesh
model_mesh::Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	// vectors for storing vertices, indices and textures
	std::vector<model_mesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<model_mesh::Texture> textures;

	// for each vertex
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		model_mesh::Vertex vertex;
		// process vertex positions, normals and texture coordinates
		
		// vertex positions
		glm::vec3 vector;
		// axis x
		if (inverse_x)
		{
			vector.x = - mesh->mVertices[i].x;
		}
		else
		{
			vector.x = mesh->mVertices[i].x;
		}
		// axis y
		if (inverse_y)
		{
			vector.y = - mesh->mVertices[i].y;
		}
		else
		{
			vector.y = mesh->mVertices[i].y;
		}
		// axis z
		if (inverse_z)
		{
			vector.z = -mesh->mVertices[i].z;
		}
		else
		{
			vector.z = mesh->mVertices[i].z;
		}
		// push position to vertex struct


		vertex.Position = vector;

		//normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		//texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else //if there isn't texture
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		// add this vertex to vertices vector
		vertices.push_back(vertex);
	}
	
	// process indices
	// for each face (triangle)
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		// get face
		aiFace face = mesh->mFaces[i];
		// and push back each index to indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);

		}
	}

	// process material
	// if there is material
	//std::cout << "mesh->mMaterialIndex = " << mesh->mMaterialIndex << '\n';
	if (mesh->mMaterialIndex >= 0)
	{
		// get this material
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::cout << material->GetName().C_Str() << '\n';

		// get diffuse textures
		std::vector<model_mesh::Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		// and add this textures to vector of textures
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		// get specular textures
		std::vector<model_mesh::Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		// and add this textures to vector of textures
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	// return Mesh object
	return model_mesh::Mesh(vertices, indices, textures);
}

// function for loading materials
std::vector<model_mesh::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<model_mesh::Texture> textures;
	
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		std::cout << "str - >" << str.C_Str() << '\n';

		//check if this structure is loaded
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(),
				str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{ // if texture hasn’t been loaded already, load it
			// Texture struct
			model_mesh::Texture texture;
			// loade texture and save to struct Texture
			texture.id = TextureFromFile(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			//add this texture to vector of textures
			textures.push_back(texture);
			// add to loaded textures
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

// fucntion for finding bounding box
// vertex is single vertex of object
// scale is scale factor of object
// modelMatrix is modelMatrix - e.g. if we rotate object -> bounding box should be bigger
void Model::findBoundingBox(glm::vec3 vertex, float scale, glm::mat4& modelMatrix)
{
	glm::vec4 vertex4 = glm::vec4(1.0f);
	vertex4.x = vertex.x;
	vertex4.y = vertex.y;
	vertex4.z = vertex.z;

	vertex = modelMatrix * vertex4 * scale;
	// x min
	if (vertex.x < m_boundingBox.min.x)
	{
		m_boundingBox.min.x = vertex.x;
	}
	// y min
	if (vertex.y < m_boundingBox.min.y)
	{
		m_boundingBox.min.y = vertex.y;
	}
	// z min
	if (vertex.z < m_boundingBox.min.z)
	{
		m_boundingBox.min.z = vertex.z;
	}
	// x max
	if (vertex.x > m_boundingBox.max.x)
	{
		m_boundingBox.max.x = vertex.x;
	}
	// y max
	if (vertex.y > m_boundingBox.max.y)
	{
		m_boundingBox.max.y = vertex.y;
	}
	// z max
	if (vertex.z > m_boundingBox.max.z)
	{
		m_boundingBox.max.z = vertex.z;
	}
}

float Model::getScale() const
{
	return m_scale;
}

// function for loading texture from file path in directory
unsigned int TextureFromFile(const char* path, const std::string& directory)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	// generate texture
	unsigned int textureID;
	glGenTextures(1, &textureID);

	// dimensions
	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	// if there is data
	if (data)
	{
		// check format of image
		GLenum format;
		if (nrComponents == 1)
		{
			format = GL_RED;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}
		// bind texture
		glBindTexture(GL_TEXTURE_2D, textureID);
		// create texture
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		// set parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// free image
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << '\n';
		stbi_image_free(data);
	}
	// return texture
	return textureID;
}