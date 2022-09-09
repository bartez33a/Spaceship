#include "..\headers\TextGenerator.h"


// text generator contructor:
// ASCII_start -> ASCII code to start creating textures of characters
// ASCII_start -> ASCII code to end creating textures of characters
// size -> size of characters [px]
TextGenerator::TextGenerator(unsigned int ASCII_start, unsigned int ASCII_end, unsigned int size)
{
	// freeType library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library\n";
	}

	// font is called face
	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font\n";
	}

	//if you loaded face
	//change size of font
	FT_Set_Pixel_Sizes(face, 0, size);

	// no byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
	for (unsigned int c = ASCII_start; c < ASCII_end; c++)
	{
		// load character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << char(c) << '\n';
			continue;
		}
		// generate texture
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D, //texture 2D
			0, //mipmap level (0 - base)
			GL_RED, //store as RGB
			face->glyph->bitmap.width, //width of texture
			face->glyph->bitmap.rows, //height of texture
			0, // 0
			GL_RED, //source format
			GL_UNSIGNED_BYTE, //store as BYTE
			face->glyph->bitmap.buffer // data
			);
		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// now store character for later use
		Character character = {
			texture, //textureID
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), //size
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), //bearing
			face->glyph->advance.x //advance
		};
		Characters.insert(std::pair<char, Character>(c, character)); //insert character to map Characters
	} // for each character

	//free resource
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	// generate openGL buffers and reserve memory
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//destructor
TextGenerator::~TextGenerator()
{
}

//this function returns position of bottom left corner and width and height of text frame
glm::vec4 TextGenerator::render(Shader & s, std::string text, float x, float y, float scale, glm::vec3 color)
{
	static bool once = false;
	float x_start, y_start;
	if (!once)
	{
		x_start = x; 
		y_start = y;
		once = true;
	}

	// activate shader program
	s.use();
	// set uniforms
	glUniform3f(glGetUniformLocation(s.get_ID(), "textColor"),
		color.x, color.y, color.z);
	// activate texture unit
	glActiveTexture(GL_TEXTURE0);

	//binf VAO
	glBindVertexArray(VAO);

	//for remember position of top, right corner of string
	float max_x = 0.0f;
	float max_y = 0.0f;

	// iterate through all characters
	std::string::const_iterator c;
	int ctr = 0;
	for (c = text.begin(); c != text.end(); c++, ctr++)
	{
		Character ch = Characters[*c];
		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;
		// update VBO for each character
		float vertices[6][4] = {
			{ xpos, ypos + h, 0.0f, 0.0f },
			{ xpos, ypos, 0.0f, 1.0f },
			{ xpos + w, ypos, 1.0f, 1.0f },
			{ xpos, ypos + h, 0.0f, 0.0f },
			{ xpos + w, ypos, 1.0f, 1.0f },
			{ xpos + w, ypos + h, 1.0f, 0.0f }
		};
		// render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// advance cursors for next glyph (advance is 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // bitshift by 6 (2^6 = 64)

		if (max_y < (ypos + h))
			max_y = ypos + h;
		if (ctr == text.size() - 1)
		{
			max_x = xpos + w;
			once = false;
		}
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	return glm::vec4(x_start, y_start, (max_x - x_start), (max_y - y_start));
}