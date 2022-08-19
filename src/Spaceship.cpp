#include "..\headers\Spaceship.h"


// Spaceship is basically  a  FPS camera object
// arguments: positon_of_camera, front_of_camera, up_vector
Spaceship::Spaceship() : Camera{glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }
{
	m_fuel = 100.0;
}


Spaceship::~Spaceship()
{
}

double Spaceship::getFuel() const
{
	return m_fuel;
}

void Spaceship::useFuel(double quantity)
{
	if (m_fuel - quantity >= 0)
	{
		m_fuel -= quantity;
	}
	else
	{
		m_fuel = 0.0;
	}
}

void Spaceship::addFuel(double quantity)
{
	m_fuel += quantity;
}
