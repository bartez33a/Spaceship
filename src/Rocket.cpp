#include "..\headers\Rocket.h"

// constructor
// x,y,z - initial position
// dir - vector which points direction of movement
Rocket::Rocket(Shader *s, float x, float y, float z, glm::vec3 dir) : Cube{ s, x,y,z, 0.1f, 0.1f, 0.5f, 1.0f, 0.0f, 0.0f }, direction{ dir }
{
	speed =  25.0;
	init_pos  = glm::vec3(x, y, z) ;
}

Rocket::Rocket(Shader * s, float x, float y, float z, int rep, int texNo, glm::vec3 dir): Cube{ s, x,y,z, 0.1f, 0.1f, 0.2f, 1, 1.0f, 0.0f, 0.0f },
direction{ dir }
{
	m_tex_no = texNo;
	speed = 25.0;
	init_pos = glm::vec3(x, y, z);
}

// copy constructor
Rocket::Rocket(const Rocket & rocket) : Cube(rocket.shader_, rocket.x_, rocket.y_, rocket.z_, 0.1, 0.1, 0.1, 1.0, 0.0, 0.0)
{
	direction = rocket.direction;
	speed = rocket.speed;
	distance = rocket.distance;
	init_pos = rocket.init_pos;
}

// move conststctor for rocket
Rocket::Rocket(Rocket && rocket): Cube(std::move(rocket.shader_), std::move(rocket.x_), 
	std::move(rocket.y_), std::move(rocket.z_), 0.1, 0.1, 0.1, 1.0, 0.0, 0.0)
{
	direction = std::move(rocket.direction);
	speed = std::move(rocket.speed);
	distance = std::move(rocket.distance);
	init_pos = std::move(rocket.init_pos);
}

// destructor
Rocket::~Rocket()
{
}

// this function returns distance covered from init position
float Rocket::checkDistance()
{
	return sqrt(pow((position.x - init_pos.x), 2.0f) + pow((position.y - init_pos.y), 2.0f) +
		pow((position.z - init_pos.z), 2.0f));
}

// fucntion for moving object
void Rocket::move(double deltaTime)
{
	float speed_factor = speed * deltaTime;
	//visualise position.
	position += direction*speed_factor;
	//move object in vertex shader
	modelMatrix = glm::translate(modelMatrix, direction*speed_factor);
}

// assignment operator
Rocket & Rocket::operator=(const Rocket & rocket) 
{
	this->speed = rocket.speed;
	this->distance = rocket.distance;
	this->init_pos = rocket.init_pos;
	return *this;
}