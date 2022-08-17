#include "../headers/Shape.h"



Shape::Shape(Shader *shader, float x, float y, float z, float R, float G, float B): x_{x}, y_{y}, z_{z}, R_{R}, G_{G}, B_{B}, shader_(shader)
{

}


Shape::~Shape()
{
	//std::cout << "SHAPE destructor! id-> " << id_no << '\n';
}

void Shape::set_ID(int id)
{
	id_no = id;
}

 int Shape::get_ID() const
{
	return id_no;
}
