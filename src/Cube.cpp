#include "../headers/Cube.h"

//constructor for cube without texture
// x, y, z -> coordinates of bottom front left corner
// w, h, l -> width, height, length
// RGB -> color of cube's vertices
Cube::Cube(Shader *shader, float x, float y, float z, float w, float h, float l, float R, float G, float B): Shape(shader, x,y,z,R,G,B), w_(w), h_(h), l_(l)
{
	// initialize modelMatrix for moving body
	modelMatrix = glm::mat4(1.0f);
	//initialize position's vector
	position.x = x;
	position.y = y;
	position.z = z;
	// TODO change to const in Shape.h and add to shape's constructor
	init_pos = position;
	
	//(x,y,z) -> front, left, bottom point of cube
	float vertices[] = {
		//x, y, z, R, G, B
		//front
		x,			y,			z_,			R_, G_, B_,
		x + w,		y,			z_,			R_, G_, B_,
		x + w,		y + h,		z_,			R_, G_, B_,
		x + w,		y + h,		z_,			R_, G_, B_,
		x,			y + h,		z_,			R_, G_, B_,
		x,			y,			z_,			R_, G_, B_,
		//rear
		x,			y,			z_ + l_,		R_, G_, B_,
		x + w,		y,			z_ + l_,		R_, G_, B_,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,
		x,			y + h,		z_ + l_,		R_, G_, B_,
		x,			y,			z_ + l_,		R_, G_, B_,
		//right
		x + w,		y,			z_,				R_, G_, B_,
		x + w,		y,			z_ + l_,		R_, G_, B_,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,
		x + w,		y + h,		z_ ,			R_, G_, B_,
		x + w,		y,			z_,				R_, G_, B_,
		//left
		x,			y,			z_,				R_, G_, B_,
		x,			y,			z_ + l_,		R_, G_, B_,
		x,			y + h,		z_ + l_,		R_, G_, B_,
		x,			y + h,		z_ + l_,		R_, G_, B_,
		x,			y + h,		z_,				R_, G_, B_,
		x,			y,			z_,				R_, G_, B_,
		//top
		x ,			y + h,		z_,				R_, G_, B_,
		x + w,		y + h,		z_,				R_, G_, B_,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,
		x ,			y + h,		z_ + l_ ,		R_, G_, B_,
		x ,			y + h,		z_,				R_, G_, B_,
		//bottom
		x ,			y,			z_,				R_, G_, B_,
		x + w,		y,			z_,				R_, G_, B_,
		x + w,		y,			z_ + l_,		R_, G_, B_,
		x + w,		y,			z_ + l_,		R_, G_, B_,
		x ,			y,			z_ + l_ ,		R_, G_, B_,
		x ,			y,			z_,				R_, G_, B_	
	};

	// load vertices data to VBO
	vbo.LoadBufferData(vertices, sizeof(vertices));
	// bind VAO
	vao.BindBuffer();
	// set attributes of vertices
	vao.SetAttribPointer(vbo, 0, 3, 6, 0); //coordinates
	vao.SetAttribPointer(vbo, 1, 3, 6, 3); //color
	// unbind VAO
	vao.UnbindBuffer();
}

//Cube with texture, remember to use shaders which constit textures
//constructor for cube with texture
// x, y, z -> coordinates of bottom front left corner
// w, h, l -> width, height, length
// rep -> how many repeats of texture?
// RGB -> color of cube
Cube::Cube(Shader *shader, float x, float y, float z, float w, float h, float l, int rep, float R, float G, float B) : Shape(shader, x,y,z,R,G,B), w_(w), h_(h), l_(l)
{
	// initialize modelMatrix for moving body
	modelMatrix = glm::mat4(1.0f);
	// initialize position's vector
	position.x = x;
	position.y = y;
	position.z = z;

	//(x,y,z) -> front, left, bottom point of cube
	float vertices[] = {
		//x, y, z, R, G, B
		//front
		x,			y,			z_ + l_,		R_, G_, B_,		0.0f,			0.0f,
		x + w,		y,			z_ + l_,		R_, G_, B_,		rep * 1.0f,		0.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f,		rep * 1.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f,		rep * 1.0f,
		x,			y + h,		z_ + l_,		R_, G_, B_,		0.0f,			rep * 1.0f,
		x,			y,			z_ + l_,		R_, G_, B_,		0.0f,			0.0f,
		//rear
		x,			y,			z_,				R_, G_, B_,		0.0f,			0.0f,
		x + w,		y,			z_,				R_, G_, B_,		rep * 1.0f,		0.0f,
		x + w,		y + h,		z_,				R_, G_, B_,		rep * 1.0f,		rep * 1.0f,
		x + w,		y + h,		z_,				R_, G_, B_,		rep * 1.0f,		rep * 1.0f,
		x,			y + h,		z_,				R_, G_, B_,		0.0f,			rep * 1.0f,
		x,			y,			z_,				R_, G_, B_,		0.0f,			0.0f,
		//right
		x + w,		y,			z_,				R_, G_, B_,		rep * 1.0f,		0.0f,
		x + w,		y,			z_ + l_,		R_, G_, B_,		0.0f,			0.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		0.0f,			rep * 1.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		0.0f,			rep * 1.0f,
		x + w,		y + h,		z_ ,			R_, G_, B_,		rep * 1.0f,		rep * 1.0f,
		x + w,		y,			z_,				R_, G_, B_,		rep * 1.0f,		0.0f,
		//left
		x,			y,			z_,				R_, G_, B_,		0.0f,			0.0f,
		x,			y,			z_ + l_,		R_, G_, B_,		rep * 1.0f,		0.0f,
		x,			y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f,		rep * 1.0f,
		x,			y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f,		rep * 1.0f,
		x,			y + h,		z_,				R_, G_, B_,		0.0f,			rep * 1.0f,
		x,			y,			z_,				R_, G_, B_,		0.0f,			0.0f,
		//top
		x ,			y + h,		z_,				R_, G_, B_,		0.0f,			rep * 1.0f,
		x + w,		y + h,		z_,				R_, G_, B_,		rep * 1.0f,		rep * 1.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f,		0.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f,		0.0f,
		x ,			y + h,		z_ + l_ ,		R_, G_, B_,		0.0f,			0.0f,
		x ,			y + h,		z_,				R_, G_, B_,		0.0f,			rep * 1.0f,
		//bottom
		x ,			y,			z_,				R_, G_, B_,		0.0f,			0.0f,
		x + w,		y,			z_,				R_, G_, B_,		rep * 1.0f,		0.0f,
		x + w,		y,			z_ + l_,		R_, G_, B_,		rep * 1.0f,		rep * 1.0f,
		x + w,		y,			z_ + l_,		R_, G_, B_,		rep * 1.0f,		rep * 1.0f,
		x ,			y,			z_ + l_ ,		R_, G_, B_,		0.0f,			rep * 1.0f,
		x ,			y,			z_,				R_, G_, B_,		0.0f,			0.0f
	};

	vbo.LoadBufferData(vertices, sizeof(vertices));
	vao.BindBuffer();
	vao.SetAttribPointer(vbo, 0, 3, 8, 0); //coordinates
	vao.SetAttribPointer(vbo, 1, 3, 8, 3); //color
	vao.SetAttribPointer(vbo, 2, 2, 8, 6); //texture
	vao.UnbindBuffer();
}

// destructor
Cube::~Cube()
{
	// unbind buffers
	vao.UnbindBuffer();
	vbo.UnbindBuffer();
}

// fuction for drawing Cube
void Cube::draw()
{
	
	//use shader and set modelMatrix
	shader_->use();
	shader_->setUniformMatrix(shader_->getModelMatrixLocation(), modelMatrix);
	//bind vao
	vao.BindBuffer();
	//draw object -> bind buffer and draw all vertices
	glDrawArrays(GL_TRIANGLES, 0, 36);
	vao.UnbindBuffer();
}

void Cube::draw_tex()
{
	// use shader program
	shader_->use();
	// select texture
	shader_->setUniformInt("tex", m_tex_no);
	// set model matrix
	shader_->setUniformMatrix(shader_->getModelMatrixLocation(), modelMatrix); //translate object
	// and draw object
	vao.BindBuffer();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	vao.UnbindBuffer();
}

// get actual position of Cube
glm::vec3 Cube::getPosition() const
{
	return position;
}

// get Cube's dimentions
glm::vec3 Cube::getDimensions() const
{
	glm::vec3 dimensions;
	dimensions.x = w_;
	dimensions.y = h_;
	dimensions.z = l_;
	return dimensions;
}

// get model matrix of cube
glm::mat4 Cube::getModelMatrix() const
{
	return modelMatrix;
}

// function for printing Cube's position and dimensions
std::ostream & operator<<(std::ostream& str, Cube const &c)
{
	// TODO: insert return statement here
	std::string str_ = "Position: (";
	str_ += c.getPosition().x;
	str_ += ", ";
	str_ += c.getPosition().y;
	str_ += ", ";
	str_ += c.getPosition().z;
	str_ += ") Dim: (";
	str_ += c.getDimensions().x;
	str_ += ", ";
	str_ += c.getDimensions().y;
	str_ += ", ";
	str_ += c.getDimensions().z;
	str_ += ")";
	return str << str_;
}