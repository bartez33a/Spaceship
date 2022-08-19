#include "../headers/meteor.h"

int Meteor::counter = 0;

Meteor::Meteor(Shader *s, float x, float y, float z, float r, float R, float G, float B) : Sphere(s, x, y, z, r, R, G, B)
{
	counter++;
	id_no = counter;
	srand(time(NULL));
	float rand_x = float(rand() % 2001 - 1000) / 1000.0f / 2.0; // -1 , 1.0
	float rand_y = float(rand() % 2001 - 1000) / 1000.0f / 2.0; // -1.0 , 1.0

	speed = float(rand() % 5000) / 1000.0f;
	//speed = 30.0f;

	direction.x = rand_x;
	direction.y = rand_y;
	direction.y = 0.0f;
	direction.z = 1.0f; //to us
	glm::normalize(direction);
}

Meteor::Meteor(Shader *s, float x, float y, float z, float r, int rep, int tex_no, float R, float G, float B): Sphere(s, x, y, z, r, rep, tex_no, R, G, B)
{	
	counter++;
	id_no = counter;
	srand(time(NULL));
	float rand_x = float(rand() % 2001 - 1000) / 1000.0f / 2.0f; // -1 , 1.0
	float rand_y = float(rand() % 2001 - 1000) / 1000.0f / 2.0f; // -1.0 , 1.0

	speed = (float(rand() % 2000) + 1000.0 ) / 1000.0f;
	//speed = 2.0f;

	direction.x = rand_x;
	direction.y = rand_y;
	//direction.y = 0.0f;
	direction.z = 1.0f;
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
	//std::cout << "Removing meteor!\n";
}



//mamy pozycje poczatkowa zapisana jako position, losujemy kierunek.
void Meteor::move(double deltaTime)
{
	float speed_factor = speed * deltaTime;
	//visualise position.
	position += direction*speed_factor;
	//std::cout << "position of meteor (" << position.x << ", " << position.y << ", " << position.z << ")\n";
	modelMatrix = glm::translate(modelMatrix, direction*speed_factor);
	//std::cout << "meteor no. " << id_no << " position (" << position.x << ", " << position.y << ", " << position.z << ")\n";
}

glm::vec3 Meteor::getPosition() const
{
	return position;
}

float Meteor::checkDistance()
{
	return sqrt(pow((position.x - init_pos.x), 2.0f) + pow((position.y - init_pos.y), 2.0f) + 
		pow((position.z - init_pos.z), 2.0f));
}



