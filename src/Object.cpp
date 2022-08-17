#include "../headers/Object.h"



Object::Object()
{
}


Object::~Object()
{
}

#include <iostream>

// pos is position of camera
bool Object::checkCollision(glm::vec3 pos)
{
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;

//	std::cout << "camPos: " << x << ", " << y << " , " << z << '\n';
	std::cout << "objPos: " << x_ << ", " << y_ << " , " << z_ << '\n';

	if (x >= x_ && x <= (x_ + w_) &&
		y >= y_ && y <= (y_ + h_) &&
		z >= z_ && z <= (z_ + l_))
	{
		return true;
	}
	else
	{
		return false;
	}	
}
