#pragma once

#include "TextGenerator.h"
#include "Rectangle.h"
#include "Shader.h"

class TextBox
{
public:
	//constructor
	TextBox(TextGenerator &tg, Shader& textShader, Shader& rectShader, const char* text, float x, float y, float scale, glm::vec3 color, float border = 5.0f);
	// default constructor is deleted
	TextBox() = delete;
	//copy constructor
	TextBox(const TextBox& tb);
	// destructor
	~TextBox();
	//draw TextBox with or without frame
	void draw(bool withFrame = true);
	//update textBox parameters e.g. text, position of text, color, border
	void updateTextBox(std::string text, float x, float y, float scale = 1.0f, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f), float border = 5.0f);
	//this function returns Position of TextBox (vector - x,y,w,h)
	// (x,y) - bottom left corner of text
	// w, h - width and height of whole text content
	glm::vec4 getPosition() const;
private:
	//order is important!
	// object for text rendereing
	TextGenerator m_textGenerator;
	// position of left bottom corner, width and height
	glm::vec4 m_pos; //it have to be declared before m_rectangle!
	// shaders for drawing
	Shader m_textShader;
	Shader m_rectShader;
	// rectangle around text
	shapes::Rectangle m_rectangle;
	// content
	std::string m_text;
	// vector for color (RGB)
	glm::vec3 m_color;
};

