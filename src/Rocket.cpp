#include "..\headers\Rocket.h"

int Rocket::counter = 0;

Rocket::Rocket(Shader *s, float x, float y, float z, glm::vec3 dir) : Cube{ s, x,y,z, 0.1f, 0.1f, 0.1f, 1.0f, 0.0f, 0.0f }, direction{ dir }
{
	counter++;
	id_no = counter;
	speed =  25.0;
	init_pos  = glm::vec3(x, y, z) ;
	//std::cout << "Rocket constructor. Position: (" << x << ", " << y << ", " << z << ")\n";
}

Rocket::Rocket(const Rocket & rocket) : Cube(rocket.shader_, rocket.x_, rocket.y_, rocket.z_, 0.1, 0.1, 0.1, 1.0, 0.0, 0.0)
{
	direction = rocket.direction;
	speed = rocket.speed;
	distance = rocket.distance;
	init_pos = rocket.init_pos;
	id_no = rocket.id_no;
}

// move conststctor for rocket
Rocket::Rocket(const Rocket && rocket): Cube(rocket.shader_, rocket.x_, rocket.y_, rocket.z_, 0.1, 0.1, 0.1, 1.0, 0.0, 0.0)
{
	direction = rocket.direction;
	speed = rocket.speed;
	distance = rocket.distance;
	init_pos = rocket.init_pos;
	id_no = rocket.id_no;
}

Rocket::~Rocket()
{
	//std::cout << "Rocket no "<< id_no <<" destructor!\n";
}

float Rocket::checkDistance()
{
	return sqrt(pow((position.x - init_pos.x), 2.0f) + pow((position.y - init_pos.y), 2.0f) +
		pow((position.z - init_pos.z), 2.0f));
}


void Rocket::move(double deltaTime)
{
	float speed_factor = speed * deltaTime;
	//visualise position.
	position += direction*speed_factor;

	modelMatrix = glm::translate(modelMatrix, direction*speed_factor);
}

Rocket & Rocket::operator=(const Rocket & rocket) 
{
	Rocket r(shader_, x_, y_, z_, direction);
	r.direction = rocket.direction;
	r.speed = rocket.speed;
	r.distance = rocket.distance;
	r.init_pos = rocket.init_pos;
	r.id_no = rocket.id_no;
	return r;
}