#include "..\headers\TextBox.h"

TextBox::TextBox(TextGenerator &tg, Shader& textShader, Shader& rectShader, const char* text, float x, float y, float scale, glm::vec3 color, float border) :
	m_textGenerator{tg},
	m_text{ text },
	m_textShader{ textShader },
	m_color{ color },
	m_pos {m_textGenerator.render(textShader, text, x, y, scale, color)},
	//rectangle
	m_rectShader{ rectShader },
	m_rectangle(m_rectShader, (m_pos.x - border), (m_pos.y - border), 0.0f, (m_pos.z + border*2), (m_pos.a + border*2), 0.0f, 0.0f, 1.0f)
{
}

// copy constructor for TextBox
// we create m_rectangle with 0 coordinates and dimensions
TextBox::TextBox(const TextBox & tb):
	m_textGenerator{ tb.m_textGenerator },
	m_text{ tb.m_text },
	m_textShader{ tb.m_textShader },
	m_color{ tb.m_color },
	m_pos{ tb.m_pos},
	//rectangle
	m_rectShader{ tb.m_rectShader },
	m_rectangle(m_rectShader, 0,0,0,0,0, 0.0f, 0.0f, 1.0f)
{
}

// destructor
TextBox::~TextBox()
{
}

// draw TextBox
void TextBox::draw(bool withFrame)
{
	//if TextBox have frame
	if (withFrame)
	{
		// draw rectangle without fill
		m_rectangle.drawLines();
	}
	// render text
	m_textGenerator.render(m_textShader, m_text, m_pos.x, m_pos.y, 1.0f, m_color);
}

// update parameters of textbox
void TextBox::updateTextBox(std::string text, float x, float y, float scale, glm::vec3 color, float border)
{
	m_text = text;
	m_pos.x = x;
	m_pos.y = y;
	m_color = color;
	
	//render text outside screen to get width and height of TextBox - it's inefficient and should be changed! 
	glm::vec4 tmp_pos = m_textGenerator.render(m_textShader, m_text, 10000, 10000, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	m_pos.z = tmp_pos.z;
	m_pos.a = tmp_pos.a;
	//update rectangle position
	m_rectangle.updateDimensions(m_pos.x - border, m_pos.y - border, m_pos.z + (border * 2), m_pos.a + (border * 2));
}

glm::vec4 TextBox::getPosition() const
{
	return m_pos;
}
