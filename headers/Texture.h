#pragma once

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>
#include "stb_image.h"

#include "Shader.h"

class Texture
{
	//parameters of image
	int width, height, channels;
	//generating texture object
	unsigned int ID;
	// texture units
public:
	enum texUnit {
		tU0 = GL_TEXTURE0,
		tU1 = GL_TEXTURE1,
		tU2 = GL_TEXTURE2,
		tU3 = GL_TEXTURE3,
		tU4 = GL_TEXTURE4,
		tU5 = GL_TEXTURE5,
		tU6 = GL_TEXTURE6,
		tU7 = GL_TEXTURE7,
		tU8 = GL_TEXTURE8,
		tU9 = GL_TEXTURE9,
		tU10 = GL_TEXTURE10,
		tU11 = GL_TEXTURE11,
		tU12 = GL_TEXTURE12,
		tU13 = GL_TEXTURE13,
		tU14 = GL_TEXTURE14,
		tU15 = GL_TEXTURE15
	};

	enum filterMethod {
		nearest = GL_NEAREST,
		linear = GL_LINEAR
	};

	enum wrappingMethod {
		repeat = GL_REPEAT,
		mirror_repeat = GL_MIRRORED_REPEAT,
		clamp_to_edge = GL_CLAMP_TO_EDGE,
		clamp_to_border = GL_CLAMP_TO_BORDER
	};

	int m_tU;

public:
	Texture(const char* file, int tU);
	//Texture(const Texture &t) = delete;
	~Texture();
	void flipImageVertically(bool flip);
	void bindTexture();
	void unbindTexture();

	void setMinFilter(filterMethod method = filterMethod::nearest);
	void setMagFilter(filterMethod method = filterMethod::linear);
	
	void setWrapS(wrappingMethod method = wrappingMethod::repeat);
	void setWrapR(wrappingMethod method = wrappingMethod::repeat);
	void setWrapT(wrappingMethod method = wrappingMethod::repeat);
};

