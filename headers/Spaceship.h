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
private:
	double m_fuel;


};

