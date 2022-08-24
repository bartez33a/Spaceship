#pragma once
#include "Camera.h"

//spaceship class
// spaceship is a point in 3d space
// spaceship can move if it has enough fuel
// we can move our spaceship by keyboard input and rotate it by mouse input
class Spaceship :
	public Camera
{
public:
	// constructor
	Spaceship();
	//destructor
	~Spaceship();
	// get amount of fuel
	double getFuel() const;
	// use fuel for moving
	void useFuel(double quantity);
	// tank fuel
	void addFuel(double quantity);
private:
	// amount of fuel
	double m_fuel;
};

