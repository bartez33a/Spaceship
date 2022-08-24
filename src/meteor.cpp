#include "../headers/meteor.h"

Meteor::Meteor(Shader *s, float x, float y, float z, float r, float R, float G, float B) : Sphere(s, x, y, z, r, R, G, B)
{
	// random position
	srand(time(NULL));
	float rand_x = float(rand() % 2001 - 1000) / 1000.0f / 2.0; // -1 , 1.0
	float rand_y = float(rand() % 2001 - 1000) / 1000.0f / 2.0; // -1.0 , 1.0
	// random speed
	speed = float(rand() % 5000) / 1000.0f;
	// random direction of movement
	direction.x = rand_x;
	direction.y = rand_y;
	direction.y = 0.0f;
	direction.z = 1.0f; //to us
	glm::normalize(direction);
}

Meteor::Meteor(Shader *s, float x, float y, float z, float r, int rep, int tex_no, float R, float G, float B): Sphere(s, x, y, z, r, rep, tex_no, R, G, B)
{	
	// random position
	srand(time(NULL));
	float rand_x = float(rand() % 2001 - 1000) / 1000.0f / 2.0f; // -1 , 1.0
	float rand_y = float(rand() % 2001 - 1000) / 1000.0f / 2.0f; // -1.0 , 1.0
	// random speed
	speed = (float(rand() % 2000) + 1000.0 ) / 1000.0f;
	// random direction of movement
	direction.x = rand_x;
	direction.y = rand_y;
	direction.z = 1.0f; //to us
	glm::normalize(direction);
}

//copy constructor for meteor without texture
Meteor::Meteor(const Meteor & m): Sphere(m.shader_, m.x_, m.y_, m.z_, m.radius, m.R_, m.G_, m.B_)
{
	speed = m.speed;
	direction = m.direction;
}
//copy constructor for meteor with texture
Meteor::Meteor(const Meteor & m, int rep, int tex_no) : Sphere(m.shader_, m.x_, m.y_, m.z_, m.radius, rep, tex_no, m.R_, m.G_, m.B_)
{
	speed = m.speed;
	direction = m.direction;
}

Meteor::~Meteor()
{
}

// move meteor
void Meteor::move(double deltaTime)
{
	float speed_factor = speed * deltaTime;
	//change position of meteor.
	position += direction*speed_factor;
	// update modelMatrix matrix from sphere class. function draw() updates model matrix in shader
	modelMatrix = glm::translate(modelMatrix, direction*speed_factor);
}

// get position of meteor
glm::vec3 Meteor::getPosition() const
{
	return position;
}

// function returns distance from initial position
float Meteor::checkDistance()
{
	return sqrt(pow((position.x - init_pos.x), 2.0f) + pow((position.y - init_pos.y), 2.0f) + 
		pow((position.z - init_pos.z), 2.0f));
}



