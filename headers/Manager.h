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

	

	Spaceship m_spaceship; //spaceship object
	Cube m_base; //base object
	Shader m_base_shader;
	Texture m_base_texture;

	//shader for meteors
	Shader m_meteor_shader;
	//textures
	Texture m_meteor_shader_tex0;
	Texture m_meteor_shader_tex1;
	Texture m_meteor_shader_tex2;
	//number od textures in meteor
	int m_meteorTexNo;
	//shader for background meteors - no texture.
	Shader m_background_meteors_shader;
	Shader m_rocket_shader;

	//fuel
	struct m_fuel_struct {
		Cube m_fuel_obj;
		int m_fuel_texNo;

		m_fuel_struct(Shader *shader, float x, float y, float z, float w, float h, float l, int rep, float R, float G, float B, int texNo) :
			m_fuel_obj{ shader, x, y, z, w, h, l , rep, R, G ,B }, m_fuel_texNo{ texNo }
		{}
		m_fuel_struct() = delete;
	};
	std::list<m_fuel_struct> m_fuel_obj_list;
	Shader m_fuel_shader;
	Texture m_fuel_shader_tex0;
	Texture m_fuel_shader_tex1;
	int m_fuelTexNo;
	double m_meteor_genereate_timer;
	int m_base_HP;

public:
	Manager();
	~Manager();
	bool checkCollisionCubes(const Cube &c1, const Cube &c2) const;
	bool checkCollisionCubeSphere(const Cube &c1, const Sphere &s1) const;
	bool checkCollisionSphere(const Sphere &s1, const Sphere& s2) const;
	bool checkCollisionCubePoint(const Cube &c1, const glm::vec3 point) const;
	void createMeteors();
	void createBackground();
	void createRocket();
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
	void generateFuel(glm::vec3 position, int percentOfChance);
};