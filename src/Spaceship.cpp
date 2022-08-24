#include "..\headers\Spaceship.h"

// constructor
// Spaceship is basically a FPS camera object - a point in 3d space
// arguments: positon_of_camera (x, y, z), front_of_camera (x, y, z), up_vector(x, y, z)
Spaceship::Spaceship() : Camera{glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f) }
{
	// set fuel
	m_fuel = 100.0;
}

// destructor
Spaceship::~Spaceship()
{
}

// this funtion returns amount of spaceship's fuel
double Spaceship::getFuel() const
{
	return m_fuel;
}

// this function decrease amount of spaceship's fuel
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

// this function increase amount of spaceship's fuel
void Spaceship::addFuel(double quantity)
{
	m_fuel += quantity;
}
