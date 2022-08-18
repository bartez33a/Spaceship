#pragma once

#include "Cube.h"
#include "Camera.h"
#include "Rocket.h"
#include "meteor.h"
#include "Point.h"
#include "Spaceship.h"
#include "Texture.h"
#include <vector>
#include <list>
#include <ctime>
#include <cstdlib>

class Manager
{
	bool m_gameOver;
	static unsigned int m_meteors_ctr;
	int m_score;
	const int m_meteorsMaxNo = 20; //max number of meteors
	int m_rocketsNo; // ammo of spaceship
	double m_loadingAmmoTimer;
	double m_loadingAmmoTime;


	std::list<Meteor> m_meteors;
	std::list<Rocket> m_rockets;
	std::list<Point> m_background_meteors;

	//number od textures in meteor
	int m_meteorTexNo;

	Spaceship m_spaceship; //spaceship object
	Cube m_base; //base object
	Shader m_base_shader;
	Texture m_base_texture;

	//shader for meteors
	Shader m_meteor_shader;
	//textures
	Texture m_meteor_shader_tex1;
	Texture m_meteor_shader_tex2;
	Texture m_meteor_shader_tex3;

	//shader for background meteors - no texture.
	Shader m_background_meteors_shader;

public:
	Manager();
	~Manager();
	bool checkCollisionCubes(const Cube &c1, const Cube &c2) const;
	bool checkCollisionCubeSphere(const Cube &c1, const Sphere &s1) const;
	bool checkCollisionSphere(const Sphere &s1, const Sphere& s2);
	void createMeteors();
	void createBackground();
	void createRocket(Shader *shader);
	std::list<Meteor> &getMeteors();
	void distanceAutoDelete(); //delete objects because of distance covered
	std::list<Rocket> &getRockets();
	std::list<Meteor>::iterator deleteMeteor(std::list<Meteor>::iterator it);
	std::list<Rocket>::iterator deleteRocket(std::list<Rocket>::iterator it);

	bool play(GLFWwindow * window, double deltaTime);
	void setMeteorsTexNo(int meteorTexNo);
	int getMeteorsTexNo() const;
	glm::mat4 getViewMatrix() const;
	void mouseInput(double, double);
};