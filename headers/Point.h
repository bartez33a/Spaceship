#pragma once
#include "Shape.h"

class Point :
	public Shape
{
public:
	static int noOfPoints;
	Point(Shader *shader, float x, float y, float z, float size);
	~Point();
	void draw();
private: 
	float m_size;
};

