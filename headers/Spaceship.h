#pragma once
#include "Camera.h"
class Spaceship :
	public Camera
{
public:
	Spaceship();
	~Spaceship();
	double getFuel() const;
	void useFuel(double quantity);
	void addFuel(double quantity);
private:
	double m_fuel;


};

