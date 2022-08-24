#pragma once
#include "Shape.h"

class Point :
	public Shape
{
public:
	//constructor
	Point(Shader *shader, float x, float y, float z, float size);
	// default constructor
	Point() = delete;
	//destructor
	~Point();
	// function for drawing objects
	void draw();
private: 
	// size of point
	float m_size;
};

