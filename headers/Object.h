#pragma once

#include <glm\glm.hpp>
class Object
{
protected:
	float x_, y_, z_;
	float w_, h_, l_;
public:

	 Object();
	~Object();

	virtual bool checkCollision(glm::vec3);
};

