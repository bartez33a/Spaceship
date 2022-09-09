#pragma once

// openGL
#include <glad\glad.h>
#include <GLFW\glfw3.h>
// for fonts
#include <ft2build.h>
#include FT_FREETYPE_H
//Shader
#include "Shader.h"

// std::map
#include <map>
//cout
#include <iostream>
// std::string
#include <string>
//glm
#include <glm\glm.hpp>

//this class generate font textures based on FreeType library and TrueType fonts

class TextGenerator
{
public:
	//constructor
	TextGenerator(unsigned int ASCII_start,unsigned  int ASCII_end, unsigned int size = 48);
	//copy constructor
	TextGenerator(const TextGenerator& tg);
	//destructor
	~TextGenerator();
	// function for rendering text in position (x,y) - bottom, left corner
	// we can change scale and color of text
	// s -> shader for rendering text
	glm::vec4 render(Shader &s, std::string text, float x, float y, float scale, glm::vec3 color);
	glm::vec2 getDimensions(std::string text, float scale);

private:
	//structure for collecting character texture and metrics
	struct Character {
		unsigned int TextureID; // ID handle of the glyph texture
		glm::ivec2 Size; // Size of glyph
		glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
		unsigned int Advance; // Offset to advance to next glyph
	};
	//for rendering
	unsigned int VAO, VBO;
	// map of Characters
	std::map<char, Character> Characters;
};

