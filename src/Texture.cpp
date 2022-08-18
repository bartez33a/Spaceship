#include "..\headers\Texture.h"
#include <iostream>


Texture::Texture(const char* file, int tU) : m_tU{ tU }
{
	glGenTextures(1, &ID);
	
	//anti load image upside-down
	stbi_set_flip_vertically_on_load(true);

	unsigned char *data = stbi_load(file, &width, &height, &channels, 0);

	std::cout << "texture file: " << file << " texture unit: " << tU << '\n';
	std::cout << "width = " << width << " height = " << height << " no of channels: " << channels << "\n\n";

	//bind this texture
	bindTexture();

	//set parameters
	setWrapR(Texture::wrappingMethod::repeat);
	setWrapS(Texture::wrappingMethod::repeat);
	setWrapT(Texture::wrappingMethod::repeat);

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
	unbindTexture();
}


Texture::~Texture()
{
	glDeleteTextures(1, &ID);
}

void Texture::flipImageVertically(bool flip)
{
	//anti load image upside-down 
	//if true -> flip
	stbi_set_flip_vertically_on_load(flip);
}

void Texture::bindTexture()
{
	glActiveTexture(m_tU);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbindTexture()
{
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setMinFilter(filterMethod method)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, method);
}

void Texture::setMagFilter(filterMethod method)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, method);
}

void Texture::setWrapS(wrappingMethod method)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, method);
}

void Texture::setWrapR(wrappingMethod method)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, method);
}

void Texture::setWrapT(wrappingMethod method)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, method);
}