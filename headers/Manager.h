#pragma once

#include "Cube.h"
#include "Camera.h"
#include "Rocket.h"
#include "meteor.h"
#include "Point.h"
#include "Spaceship.h"
#include "Texture.h"
#include "TextGenerator.h"
#include "TextBox.h"
#include "MySQL.h"
#include "Slider.h"
#include <vector>
#include <list>
#include <ctime>
#include <cstdlib>
// hide and show console
#include <Windows.h>
#include <algorithm>

// game manager class
class Manager
{
public:
	Manager(GLFWwindow* window);
	~Manager();
	/// main function
	bool play(GLFWwindow * window, double deltaTime);
	// get number of meteors textures
	int getMeteorsTexNo() const;
	// get camera view matrix
	glm::mat4 getViewMatrix() const;
	// mouse input callback function
	void mouseInput(double, double);
	// process window input
	void processInput(GLFWwindow *window);

private:
	/// check collision functions
	bool checkCollisionCubes(const Cube &c1, const Cube &c2) const;
	bool checkCollisionCubeSphere(const Cube &c1, const Sphere &s1) const;
	bool checkCollisionSphere(const Sphere &s1, const Sphere& s2) const;
	bool checkCollisionCubePoint(const Cube &c1, const glm::vec3 point) const;
	/// create object functions
	void createMeteors();
	// set number of meteors textures
	void setMeteorsTexNo(int meteorTexNo);
	void createRocket();
	void createBackground();
	// generate fuel from destroyed meteors
	void generateFuel(glm::vec3 position, int percentOfChance);

	/// delete object funtions
	// autodelete objects when distance is too long
	void distanceAutoDelete();
	//delete meteor function
	std::list<Meteor>::iterator deleteMeteor(std::list<Meteor>::iterator it);
	std::list<Rocket>::iterator deleteRocket(std::list<Rocket>::iterator it);
	//draw and moveall objects
	void drawAndMoveAllObjects(double deltaTime);
	//only draw all objects
	void drawAllObjects();
	//check all collisions
	void checkAllCollisions();
	// move spaceship
	void moveSpaceship(GLFWwindow * window, double deltaTime);
	// update matrices for shaders
	void updateShadersMatrices();

	
	/// private variables
	// is gameover?
	bool m_gameOver;
	// meteors counter for choose texture
	static unsigned int m_meteors_ctr; 
	// score counter
	int m_score;
	// max number of meteors
	const int m_meteorsMaxNo = 20;
	// spaceship's ammo
	int m_rocketsNo; 
	// for loading spaceship's ammo
	double m_loadingAmmoTimer;
	double m_loadingAmmoTime; // loading time
	
	// meteor generator period
	double m_meteor_genereate_timer;
	// base hp
	int m_base_HP;
	
	/// objects
	std::list<Meteor> m_meteors;
	std::list<Rocket> m_rockets;
	std::list<Point> m_background_meteors;

	Spaceship m_spaceship; //spaceship object
	Cube m_base; //base object
	Shader m_base_shader;
	Texture m_base_texture;

	// shader for meteors
	Shader m_meteor_shader;
	// textures for meteors
	Texture m_meteor_shader_tex0;
	Texture m_meteor_shader_tex1;
	Texture m_meteor_shader_tex2;
	// number od textures in meteor
	int m_meteorTexNo;
	
	// shader for background meteors - without texture.
	Shader m_background_meteors_shader;

	//Shader for rockets with texture
	Shader m_rocket_shader_tex;
	//Texture for rockets
	Texture m_rocket_tex0;

	//fuel object
	struct m_fuel_struct {
		Cube m_fuel_obj;
		int m_fuel_texNo; // texture number

		//constructor
		m_fuel_struct(Shader *shader, float x, float y, float z, float w, float h, float l, int rep, float R, float G, float B, int texNo) :
			m_fuel_obj{ shader, x, y, z, w, h, l , rep, R, G ,B }, m_fuel_texNo{ texNo } 
		{}
		// default constructor
		m_fuel_struct() = delete;
	};
	std::list<m_fuel_struct> m_fuel_obj_list;
	// shader for fuel object
	Shader m_fuel_shader;
	// texture for fuel object
	Texture m_fuel_shader_tex0;
	Texture m_fuel_shader_tex1;
	// number of fuel object textures
	int m_fuelTexNo;

	//slider for laoding ammo visualization
	Shader m_slider_shader;
	Slider m_ammo_slider;

	//text rendering
	TextGenerator textGen;
	Shader m_shader_text;

	//TextBox
	//text rendering
	TextGenerator m_textGen_TextBox;
	Shader m_shader_textBoxRect; //shader before TextBox objects!
	TextBox tb_empty;
	std::vector<TextBox> m_TextBoxesNumbers;
	std::vector<TextBox> m_TextBoxesName, m_TextBoxesNameValue, m_TextBoxesScore, m_TextBoxesScoreValue;

	//sql database
	MySQL m_mySQL;
	// this function checks if m_score is in top 10 scores in database and if it is,
	// then it insert this score into database
	void checkAndWriteScore();

	//pauseGame
	bool m_pauseGame;

	//window size
	int m_win_w;
	int m_win_h;

	//menu
	bool m_show_menu = false;
	//for storing top ten scores
	std::vector<MySQL::m_Row> topTen;
	void drawMenu();
	void generateMenu();
	
};