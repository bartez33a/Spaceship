#include "..\headers\Texture.h"
#include <iostream>

// constructor
// file -> name of file with texture
// tU -> texture unit to which this texture image will be assigned
Texture::Texture(const char* file, int tU) : m_tU{ tU }
{
	// generate texture object
	glGenTextures(1, &ID);
	
	//anti load image upside-down
	stbi_set_flip_vertically_on_load(true);

	// load texture image data
	unsigned char *data = stbi_load(file, &width, &height, &channels, 0);

	auto printTexture = [&]() {
		std::cout << "texture file: " << file << " texture unit: " << tU << '\n';
		std::cout << "width = " << width << " height = " << height << " no of channels: " << channels << "\n\n";
	};
	
	printTexture();

	//bind this texture
	bindTexture();

	//set wrapping parameters
	setWrapR(Texture::wrappingMethod::repeat);
	setWrapS(Texture::wrappingMethod::repeat);
	setWrapT(Texture::wrappingMethod::repeat);

	//set minifying and magnifying filters methods
	setMinFilter(Texture::filterMethod::linear);
	setMagFilter(Texture::filterMethod::nearest);

	//create texture
	if (data)
	{
		if (channels == 3) //RGB
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (channels == 4) //RGBA
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	//else -> couldnt load image!
	else
	{
		std::cout << "COULDNT LOAD IMAGE " << file << '\n';
		//throw()?
	}

	//free memory, delete image
	stbi_image_free(data);
	//unbind texture for now
	unbindTexture();
}

// destructor
Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

// function to prevent loading image upside-down
void Texture::flipImageVertically(bool flip)
{
	//anti load image upside-down 
	//if true -> flip
	stbi_set_flip_vertically_on_load(flip);
}

// bind texture
void Texture::bindTexture()
{
	glActiveTexture(m_tU);
	glBindTexture(GL_TEXTURE_2D, ID);
}

// unbind texture
void Texture::unbindTexture()
{
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// set minifying filter method
void Texture::setMinFilter(filterMethod method)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, method);
}

// set magnifying filter method
void Texture::setMagFilter(filterMethod method)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, method);
}

// set wrapping method for axis x
void Texture::setWrapS(wrappingMethod method)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, method);
}

// set wrapping method for axis y
void Texture::setWrapT(wrappingMethod method)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, method);
}

// set wrapping method for axis z
void Texture::setWrapR(wrappingMethod method)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, method);
}