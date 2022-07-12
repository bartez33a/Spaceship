#include "../headers/Cube.h"


//constructor for cube without texture
// x, y, z -> coordinates of bottom front left corner
// w, h, l -> width, height, length
// RGB -> color of cube
Cube::Cube(float x, float y, float z, float w, float h, float l, float R, float G, float B): x_(x), y_(y), z_(z), w_(w), h_(h), l_(l),
R_(R), G_(G), B_(B)
{
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
Cube::Cube(float x, float y, float z, float w, float h, float l, int rep, float R, float G, float B) : x_(x), y_(y), z_(z), w_(w), h_(h), l_(l),
R_(R), G_(G), B_(B)
{
	//(x,y,z) -> front, left, bottom point of cube
	float vertices[] = {
		//x, y, z, R, G, B
		//front
		x,			y,			z_ + l_,		R_, G_, B_,		0.0f, 0.0f,
		x + w,		y,			z_ + l_,		R_, G_, B_,		rep * 1.0f, 0.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f, rep * 1.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f, rep * 1.0f,
		x,			y + h,		z_ + l_,		R_, G_, B_,		0.0f, rep * 1.0f,
		x,			y,			z_ + l_,		R_, G_, B_,		0.0f, 0.0f,
		//rear
		x,			y,			z_,				R_, G_, B_,		0.0f, 0.0f,
		x + w,		y,			z_,				R_, G_, B_,		rep * 1.0f, 0.0f,
		x + w,		y + h,		z_,				R_, G_, B_,		rep * 1.0f, rep * 1.0f,
		x + w,		y + h,		z_,				R_, G_, B_,		rep * 1.0f, rep * 1.0f,
		x,			y + h,		z_,				R_, G_, B_,		0.0f, rep * 1.0f,
		x,			y,			z_,				R_, G_, B_,		0.0f, 0.0f,
		//right
		x + w,		y,			z_,				R_, G_, B_,		0.0f, rep * 1.0f,
		x + w,		y,			z_ + l_,		R_, G_, B_,		0.0f, 0.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f, 0.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f, 0.0f,
		x + w,		y + h,		z_ ,			R_, G_, B_,		rep * 1.0f, rep * 1.0f,
		x + w,		y,			z_,				R_, G_, B_,		0.0f, rep * 1.0f,
		//left
		x,			y,			z_,				R_, G_, B_,		0.0f, rep * 1.0f,
		x,			y,			z_ + l_,		R_, G_, B_,		0.0f, 0.0f,
		x,			y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f, 0.0f,
		x,			y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f, 0.0f,
		x,			y + h,		z_,				R_, G_, B_,		rep * 1.0f, rep * 1.0f,
		x,			y,			z_,				R_, G_, B_,		0.0f, rep * 1.0f,
		//top
		x ,			y + h,		z_,				R_, G_, B_,		0.0f, rep * 1.0f,
		x + w,		y + h,		z_,				R_, G_, B_,		rep * 1.0f, rep * 1.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f, 0.0f,
		x + w,		y + h,		z_ + l_,		R_, G_, B_,		rep * 1.0f, 0.0f,
		x ,			y + h,		z_ + l_ ,		R_, G_, B_,		0.0f, 0.0f,
		x ,			y + h,		z_,				R_, G_, B_,		0.0f, rep * 1.0f,
		//bottom
		x ,			y,			z_,				R_, G_, B_,		0.0f, rep * 1.0f,
		x + w,		y,			z_,				R_, G_, B_,		rep * 1.0f, rep * 1.0f,
		x + w,		y,			z_ + l_,		R_, G_, B_,		rep * 1.0f, 0.0f,
		x + w,		y,			z_ + l_,		R_, G_, B_,		rep * 1.0f, 0.0f,
		x ,			y,			z_ + l_ ,		R_, G_, B_,		0.0f, 0.0f,
		x ,			y,			z_,				R_, G_, B_,		0.0f, rep * 1.0f
	};

	vbo.LoadBufferData(vertices, sizeof(vertices));
	vao.BindBuffer();
	vao.SetAttribPointer(vbo, 0, 3, 8, 0); //coordinates
	vao.SetAttribPointer(vbo, 1, 3, 8, 3); //color
	vao.SetAttribPointer(vbo, 2, 2, 8, 6); //texture
	vao.UnbindBuffer();
}

Cube::~Cube()
{
	// unbind buffers
	vao.UnbindBuffer();
	vbo.UnbindBuffer();
}

void Cube::draw()
{
	//draw object -> bind buffer and draw all vertices
	vao.BindBuffer();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}