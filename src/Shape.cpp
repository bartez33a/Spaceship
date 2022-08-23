#include "../headers/Shape.h"

// constructor
Shape::Shape(Shader *shader, float x, float y, float z, float R, float G, float B): x_{x}, y_{y}, z_{z}, R_{R}, G_{G}, B_{B}, shader_(shader)
{
}

// destructor
Shape::~Shape()
{
}