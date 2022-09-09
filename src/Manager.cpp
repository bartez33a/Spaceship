#include "..\headers\Manager.h"

unsigned int Manager::m_meteors_ctr = 0;

// constructor for manager
Manager::Manager(GLFWwindow* window) : m_base_shader{ "shaders/base/base_shader_tex.vs", "shaders/base/base_shader_tex.fs" },
m_base_texture{ "textures/base/base.jpg", GL_TEXTURE0 },
m_base{ &m_base_shader, -10.0f, -10.0f, 20.0f, 20.0f, 20.0f, 5.0f, 1, 1.0f, 1.0f, 0.0f },
//shader for meteors
m_meteor_shader{ "shaders/meteor/shader_meteor_tex.vs", "shaders/meteor/shader_meteor_tex.fs" },
m_background_meteors_shader{ "shaders/shader_background_meteor.vs", "shaders/shader_background_meteor.fs" },
m_meteor_shader_tex0{ "textures/meteors/magma.png", GL_TEXTURE0 },
m_meteor_shader_tex1{ "textures/meteors/meteor.png", GL_TEXTURE1 },
m_meteor_shader_tex2{ "textures/meteors/meteor2.png", GL_TEXTURE2 },
m_rocket_shader("shaders/shader.vs", "shaders/shader.fs"), //shader without texture
m_fuel_shader{ "shaders/fuel/fuel_shader.vs", "shaders/fuel/fuel_shader.fs" },
m_fuel_shader_tex0{ "textures/fuel/fuel.png", GL_TEXTURE0 },
m_fuel_shader_tex1{ "textures/fuel/fuel2.jpg", GL_TEXTURE1 },
m_fuelTexNo{ 2 },
//text rendering
textGen{ 0, 128 }, // from 0 to 127 (all basic characters)
m_textGen_TextBox{0 , 128, 30},
m_shader_text{ "shaders/font/font_shader.vs", "shaders/font/font_shader.fs" },
//textBox
tb_empty{ m_textGen_TextBox, m_shader_text, m_shader_textBoxRect, "", 100, 150, 1.0f, glm::vec3(0.0f, 1.0f, 0.2f) },
m_TextBoxesNumbers(15, tb_empty),
m_TextBoxesName(10, tb_empty),
m_TextBoxesNameValue(10, tb_empty),
m_TextBoxesScore(10, tb_empty),
m_TextBoxesScoreValue(10, tb_empty),
m_shader_textBoxRect{ "shaders/textBox/rectangle/rectangleShader.vs", "shaders/textBox/rectangle/rectangleShader.fs" },
//mySQL database (schema)
m_mySQL{ "localhost", "root", "", "spaceship"}
{
	//set window size variables
	glfwGetWindowSize(window, &m_win_w, &m_win_h);
	// hide console
	//ShowWindow(GetConsoleWindow(), SW_HIDE);
	//at the beginning game is not paused
	m_pauseGame = false;
	//and menu is not turn on
	m_show_menu = false;
	// for random generator
	srand(time(NULL));

	// model and projection matrix for objects
	glm::mat4 mm = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);

	// update background meteors matrices
	m_background_meteors_shader.updateMatrices(mm, m_spaceship.get_viewMatrix(), projection);

	//update base matrices
	m_base_shader.updateMatrices(mm, m_spaceship.get_viewMatrix(), projection);
	//updateMatrices function also make shader program active
	m_base_shader.setUniformInt("texture0", 0); // texture for base

	//meteors
	//update meteors matrices
	m_meteor_shader.updateMatrices(mm, m_spaceship.get_viewMatrix(), projection);
	//and set textures' uniforms
	glUniform1i(glGetUniformLocation(m_meteor_shader.get_ID(), "texture0"), 0); // manually
	glUniform1i(glGetUniformLocation(m_meteor_shader.get_ID(), "texture1"), 1); // manually
	glUniform1i(glGetUniformLocation(m_meteor_shader.get_ID(), "texture2"), 2); // manually
	// set number of meteors textures
	setMeteorsTexNo(3);

	//update rockets matrices
	m_rocket_shader.updateMatrices(mm, m_spaceship.get_viewMatrix(), projection);

	// update fuel objects matrices
	m_fuel_shader.updateMatrices(mm, m_spaceship.get_viewMatrix(), projection);
	glUniform1i(glGetUniformLocation(m_fuel_shader.get_ID(), "texture0"), 0); // manually
	glUniform1i(glGetUniformLocation(m_fuel_shader.get_ID(), "texture1"), 1); // manually

	//text rendering shader
	glm::mat4 projection_text = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	m_shader_text.setUniformMatrix(m_shader_text.getProjectionMatrixLocation(), projection_text);
	//ortographic projection for TextBox->rectangle
	m_shader_textBoxRect.setUniformMatrix(m_shader_textBoxRect.getProjectionMatrixLocation(), projection_text);

	/// game settings
	//player
	m_score = 0;
	m_rocketsNo = 10;
	m_gameOver = false;

	//shooting 
	m_loadingAmmoTime = 1.0; //time to load ammo
	m_loadingAmmoTimer = m_loadingAmmoTime; //so we can shoot at the beginning 

	//base HP
	m_base_HP = 10;

	// create background -> darw background meteors
	createBackground();
} // constructor

// destructor
Manager::~Manager()
{
}

// function which checks collision of two Cubes
// true -> collision
// false -> no collision
bool Manager::checkCollisionCubes(const Cube &c1, const Cube &c2) const
{
	//extract cube 1 
	glm::vec3 c1_pos = c1.getPosition();
	glm::vec3 c1_dim = c1.getDimensions();
	float x1 = c1_pos.x;
	float y1 = c1_pos.y;
	float z1 = c1_pos.z;
	
	float w1 = c1_dim.x;
	float h1 = c1_dim.y;
	float l1 = c1_dim.z;

	//extract cube 2 
	glm::vec3 c2_pos = c2.getPosition();
	glm::vec3 c2_dim = c2.getDimensions();
	float x2 = c2_pos.x;
	float y2 = c2_pos.y;
	float z2 = c2_pos.z;

	float w2 = c2_dim.x;
	float h2 = c2_dim.y;
	float l2 = c2_dim.z;

	if (( ((x1 + w1) >= x2) && (x1 <= (x2 + w2)))
		&&
		(((y1 + h1) >= y2) && (y1 <= (y2 + h2)))
		&&
		( ((z1 + l1) >= z2 )&& (z1 <= (z2 + l2)))
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// function which checks collision of Cube and Sphere
// true -> collision
// false -> no collision
bool Manager::checkCollisionCubeSphere(const Cube & c1, const Sphere & s1) const
{
	//extract position of cube 
	glm::vec3 c1_pos = c1.getPosition();
	glm::vec3 c1_dim = c1.getDimensions();
	float x1 = c1_pos.x;
	float y1 = c1_pos.y;
	float z1 = c1_pos.z;

	float w1 = c1_dim.x;
	float h1 = c1_dim.y;
	float l1 = c1_dim.z;

	//extract position of Sphere
	glm::vec3 s1_pos = s1.getPosition();
	float x2 = s1_pos.x;
	float y2 = s1_pos.y;
	float z2 = s1_pos.z;

	float r2 = s1.getRadius();


	if (
		(((x2 + r2) >= x1) && ((x2 - r2) <= (x1 + w1)))
		&&
		(((y2 + r2) >= y1) && ((y2 - r2) <= (y1 + h1)))
		&&
		(((z2 + r2) >= z1) && ((z2 - r2) <= (z1 + l1)))
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// function which checks collision of two Spheres
// true -> collision
// false -> no collision
bool Manager::checkCollisionSphere(const Sphere & s1, const Sphere & s2) const
{
	//extract position of Sphere
	glm::vec3 s1_pos = s1.getPosition();
	float x1 = s1_pos.x;
	float y1 = s1_pos.y;
	float z1 = s1_pos.z;
	float r1 = s1.getRadius();

	//extract position of Sphere
	glm::vec3 s2_pos = s2.getPosition();
	float x2 = s2_pos.x;
	float y2 = s2_pos.y;
	float z2 = s2_pos.z;
	float r2 = s2.getRadius();

	if (
		(((x1 + r1) >= (x2 - r2) ) && ((x1 - r1) <= (x2 + r2)))
		&&
		(((y1 + r1) >= (y2 - r2)) && ((y1 - r1) <= (y2 + r2)))
		&&
		(((z1 + r1) >= (z2 - r2)) && ((z1 - r1) <= (z2 + r2)))
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// function which checks collision of two Cube and Point (e.g. Spaceship)
// true -> collision
// false -> no collision
bool Manager::checkCollisionCubePoint(const Cube & c1, const glm::vec3 point) const
{
	//extract cube 1 
	glm::vec3 c1_pos = c1.getPosition();
	glm::vec3 c1_dim = c1.getDimensions();
	float x1 = c1_pos.x;
	float y1 = c1_pos.y;
	float z1 = c1_pos.z;

	float w1 = c1_dim.x;
	float h1 = c1_dim.y;
	float l1 = c1_dim.z;

	float x2 = point.x;
	float y2 = point.y;
	float z2 = point.z;
	
	if ((((x1 + w1) >= x2) && (x1 <= x2))
		&&
		(((y1 + h1) >= y2) && (y1 <= y2))
		&&
		(((z1 + l1) >= z2) && (z1 <= z2))
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// create meteor in valid position
// meteor cannot be generate in position in which is another meteor!
// TODO! meteor cannot be generete in spaceship's position!
void Manager::createMeteors()
{
	if (m_meteors.size() < m_meteorsMaxNo)
	{
		bool valid_pos = false;
		float pos_x;
		float pos_y;
		float pos_z;
		float radius;

		if (m_meteors.size() <= 1)
		{
			valid_pos = true;
			// random position and radius
			pos_x = float((rand() % 10000 - 5000)) / 1000.0f / 2.5f;
			pos_y = float((rand() % 10000 - 5000)) / 1000.0f / 5.0f;
			pos_z = float((rand() % 10000 - 20000)) / 1000.0f; 
			radius = float((rand() % 1000)) / 1000.0f + .2f;
		}
		else
		{
			do
			{
				pos_x = float((rand() % 10000 - 5000)) / 1000.0f / 2.5f;
				pos_y = float((rand() % 10000 - 5000)) / 1000.0f / 5.0f;
				pos_z = float((rand() % 10000 - 20000)) / 1000.0f;
				radius = float((rand() % 1000)) / 1000.0f + .2f;
				Sphere m2(&m_meteor_shader, pos_x, pos_y, pos_z, radius);

				for (auto& m : m_meteors)
				{
					//valid_pos -> if no collision == true
					valid_pos = !checkCollisionSphere(m, m2);
					if (!valid_pos)
					{
						// when there is collision -> break and draw again!
						break;
					}
				}// for each meteor

			} while (!valid_pos);
		}
		m_meteors.emplace_back(&m_meteor_shader, pos_x, pos_y, pos_z, radius, 1, ((m_meteors_ctr++) % m_meteorTexNo));
	}
}

// function which creates background -> add Point to list of background points
void Manager::createBackground()
{
	// distance from origin
	float distance = 100.0f;
	// position of each point
	float pos_x, pos_y, pos_z;

	// points around origin on Sphere
	float PI = 3.14159f;
	int sectorCount = 50;
	int stackCount = 70;
	float sectorStep = 2.0 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;
	float xy;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
		pos_z = +distance * sin(stackAngle); // axis Z
		xy = distance * cos(stackAngle);
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep; // starting from 0 to 2pi
			pos_x = xy * cos(sectorAngle);
			pos_y = xy * sin(sectorAngle);
			m_background_meteors.emplace_back(&m_background_meteors_shader, pos_x, pos_y, pos_z, 1.0f);
		} //for sectorCount
	} //for stackCount
} //createBackground()

// fucntion for generate rockets
void Manager::createRocket()
{
	glm::vec3 camPos = m_spaceship.getCamPos();
	glm::vec3 camFront = m_spaceship.getCamFront();
	
	// only if spaceship has enough ammo
	if (m_rocketsNo > 0)
	{
		// if ammo is loaded
		if (m_loadingAmmoTimer >= m_loadingAmmoTime)
		{
			// create rockets
			m_rockets.emplace_back(&m_rocket_shader, camPos.x, camPos.y, camPos.z, camFront);
			// decrease ammo
			m_rocketsNo--;
			// show information
			std::cout << "Ammo left: " << m_rocketsNo << '\n';
			// reset loading timer
			m_loadingAmmoTimer = 0.0;
		}
	}
	else
	{
		std::cout << "No more ammo!\n";
	}
} // create Rocket


// delete objects when covered distance is too long
void Manager::distanceAutoDelete()
{
	//delete rockets
	for (auto it = m_rockets.begin(); it != m_rockets.end();)
	{
		if ((*it).checkDistance() > 20.0f)
		{
			it = m_rockets.erase(it);
		}
		else
		{
			it++;
		}
	}

	//delete meteors
	for (auto it = m_meteors.begin(); it != m_meteors.end();)
	{
		if ((*it).checkDistance() > 50.0f)
		{
			it = m_meteors.erase(it);
		}
		else
		{
			//increase only when you dont erase element, if you erase element, function erase 
			//moves itr to the next element while itr != meteors.end()
			it++;
		}
	}
}

// delete meteor from list of meteors
std::list<Meteor>::iterator Manager::deleteMeteor(std::list<Meteor>::iterator it)
{
	return m_meteors.erase(it);
}

// delete rocket from list of rockets
std::list<Rocket>::iterator Manager::deleteRocket(std::list<Rocket>::iterator it)
{
	return m_rockets.erase(it);
}

// function for drawing all objects. this function also moves all objects
void Manager::m_drawAndMoveAllObjects(double deltaTime)
{
	//draw background meteors
	//rotate background meteors
	m_background_meteors_shader.rotateObjects(0.2 * deltaTime);
	for (auto &b : m_background_meteors)
	{
		b.draw();
	}

	//draw base
	//different shader with different textures? you have to bind those textures
	m_base_texture.bindTexture();
	m_base.draw();

	// draw meteors
	//you have to bind all textures otherwise shape will be black
	m_meteor_shader_tex0.bindTexture();
	m_meteor_shader_tex1.bindTexture();
	m_meteor_shader_tex2.bindTexture();
	for (auto &m : m_meteors)
	{
		m.move(deltaTime);
		m.draw_tex();
	}

	//draw rockets
	for (auto & r : m_rockets)
	{
		r.move(deltaTime);
		r.draw();
	}

	//draw fuel
	m_fuel_shader_tex0.bindTexture();
	m_fuel_shader_tex1.bindTexture();
	for (auto& f : m_fuel_obj_list)
	{
		f.m_fuel_obj.draw();
		m_fuel_shader.setUniformInt("tex", f.m_fuel_texNo);
		//std::cout << "fuel_tex_no = " << f.m_fuel_texNo << '\n';
	}

	/// text rendering
	//position of text and value in screen coordinates
	int text_x_pos = 630;
	int value_x_pos = 750;
	//show score:
	std::string s_score = "Score: ";
	//s_score += std::to_string(m_score);
	textGen.render(m_shader_text, s_score, text_x_pos, 550, .5f, glm::vec3(1.0f, 0.0f, 0.0f));
	std::string s_score2 = std::to_string(m_score);
	textGen.render(m_shader_text, s_score2, value_x_pos, 550, .5f, glm::vec3(1.0f, 1.0f, 0.0f));

	//show fuel
	std::string s_fuel = "Fuel: ";
	//s_fuel += std::to_string(int(m_spaceship.getFuel()));
	textGen.render(m_shader_text, s_fuel, text_x_pos, 520, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
	std::string s_fuel2 = std::to_string(int(m_spaceship.getFuel()));
	textGen.render(m_shader_text, s_fuel2, value_x_pos, 520, .5f, glm::vec3(1.0f, 1.0f, 0.0f));

	//show ammo
	std::string s_ammo = "Ammo: ";
	//s_ammo += std::to_string(m_rocketsNo);
	textGen.render(m_shader_text, s_ammo, text_x_pos, 490, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

	std::string s_ammo2 = std::to_string(m_rocketsNo);
	textGen.render(m_shader_text, s_ammo2, value_x_pos, 490, .5f, glm::vec3(1.0f, 1.0f, 0.0f));

	//show base HP
	std::string s_baseHP = "Base HP: ";
	//s_baseHP += std::to_string(m_base_HP);
	textGen.render(m_shader_text, s_baseHP, text_x_pos, 460, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

	std::string s_baseHP2 = std::to_string(m_base_HP);
	textGen.render(m_shader_text, s_baseHP2, value_x_pos, 460, .5f, glm::vec3(1.0f, 1.0f, 0.0f));
}

// function for drawing all objects.
void Manager::m_drawAllObjects()
{
	//draw background meteors
	//rotate background meteors
	for (auto &b : m_background_meteors)
	{
		b.draw();
	}

	//draw base
	//different shader with different textures? you have to bind those textures
	m_base_texture.bindTexture();
	m_base.draw();

	// draw meteors
	//you have to bind all textures otherwise shape will be black
	m_meteor_shader_tex0.bindTexture();
	m_meteor_shader_tex1.bindTexture();
	m_meteor_shader_tex2.bindTexture();
	for (auto &m : m_meteors)
	{
		m.draw_tex();
	}

	//draw rockets
	for (auto & r : m_rockets)
	{
		r.draw();
	}

	//draw fuel
	m_fuel_shader_tex0.bindTexture();
	m_fuel_shader_tex1.bindTexture();
	for (auto& f : m_fuel_obj_list)
	{
		f.m_fuel_obj.draw();
		m_fuel_shader.setUniformInt("tex", f.m_fuel_texNo);
		//std::cout << "fuel_tex_no = " << f.m_fuel_texNo << '\n';
	}

	/// text rendering
	//position of text and value in screen coordinates
	int text_x_pos = 630;
	int value_x_pos = 750;
	//show score:
	std::string s_score = "Score: ";
	//s_score += std::to_string(m_score);
	textGen.render(m_shader_text, s_score, text_x_pos, 550, .5f, glm::vec3(1.0f, 0.0f, 0.0f));
	std::string s_score2 = std::to_string(m_score);
	textGen.render(m_shader_text, s_score2, value_x_pos, 550, .5f, glm::vec3(1.0f, 1.0f, 0.0f));

	//show fuel
	std::string s_fuel = "Fuel: ";
	//s_fuel += std::to_string(int(m_spaceship.getFuel()));
	textGen.render(m_shader_text, s_fuel, text_x_pos, 520, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
	std::string s_fuel2 = std::to_string(int(m_spaceship.getFuel()));
	textGen.render(m_shader_text, s_fuel2, value_x_pos, 520, .5f, glm::vec3(1.0f, 1.0f, 0.0f));

	//show ammo
	std::string s_ammo = "Ammo: ";
	//s_ammo += std::to_string(m_rocketsNo);
	textGen.render(m_shader_text, s_ammo, text_x_pos, 490, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

	std::string s_ammo2 = std::to_string(m_rocketsNo);
	textGen.render(m_shader_text, s_ammo2, value_x_pos, 490, .5f, glm::vec3(1.0f, 1.0f, 0.0f));

	//show base HP
	std::string s_baseHP = "Base HP: ";
	//s_baseHP += std::to_string(m_base_HP);
	textGen.render(m_shader_text, s_baseHP, text_x_pos, 460, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));

	std::string s_baseHP2 = std::to_string(m_base_HP);
	textGen.render(m_shader_text, s_baseHP2, value_x_pos, 460, .5f, glm::vec3(1.0f, 1.0f, 0.0f));
}

// function for checking collisions
void Manager::m_checkAllCollisions()
{
	//check collision meteor <--> rocket
	//for each rocket
	int i = 0; int j = 0;
	for (std::list<Rocket>::iterator it = m_rockets.begin(); it != m_rockets.end();)
	{
		j = 0;
		i++;
		bool coll = false; //check collision
						   //for each meteor
		for (std::list<Meteor>::iterator it2 = m_meteors.begin(); it2 != m_meteors.end();)
		{
			j++;
			// if rocket and meteor have collision
			if (checkCollisionCubeSphere(*it, *it2))
			{
				glm::vec3 pos = (*it).getPosition();
				generateFuel(pos, 30);
				it = deleteRocket(it); //returns iterator to next element
				it2 = deleteMeteor(it2);

				std::cout << "Score: " << ++m_score << '\n';
				//if you shoot meteor -> you get more ammo
				m_rocketsNo += 2;
				coll = true;
				break;
			}
			else
			{
				it2++;
			}
		} //for loop -> meteors
		if (!coll)
		{
			it++;
		}
	} //for loop -> rockets

	  //check collision of meteor and base
	for (std::list<Meteor>::iterator it = m_meteors.begin(); it != m_meteors.end(); it++)
	{
		// if rocket and meteor have collision
		if (checkCollisionCubeSphere(m_base, *it))
		{
			it = deleteMeteor(it); //returns iterator to next element
			std::cout << "meteor hits base!\n";
			m_base_HP--;

			if (m_base_HP <= 0)
			{
				m_gameOver = true;
			}
		}
	} //for loop -> meteors


	  //check if we tank fuel
	for (auto it = m_fuel_obj_list.begin(); it != m_fuel_obj_list.end();)
	{
		if (checkCollisionCubePoint((*it).m_fuel_obj, m_spaceship.getCamPos()))
		{
			//collision
			it = m_fuel_obj_list.erase(it);
			m_spaceship.addFuel(5.0f);
			std::cout << "FUEL TANKED!\n";
		}
		else
		{
			it++;
		}
	} // tank fuel
}

// function for moving spaceship.
void Manager::m_moveSpaceship(GLFWwindow * window, double deltaTime)
{
	static glm::vec3 spaceship_pos = m_spaceship.getCamPos();
	//if we have enough fuel, get user input
	if (m_spaceship.getFuel() > 0)
	{
		m_spaceship.Input(window, deltaTime);
		//if spaceship moved
		if (m_spaceship.getCamPos() != spaceship_pos)
		{
			spaceship_pos = m_spaceship.getCamPos();
			//decrease fuel
			m_spaceship.useFuel(0.1);
			std::cout << "Spaceship fuel: " << m_spaceship.getFuel() << '\n';
		}
	}
}

// function for update shaders matrices.
void Manager::m_updateShadersMatrices()
{
	glm::mat4 mm = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);
	//update view matrix of shaders 
	m_base_shader.setUniformMatrix(m_base_shader.getViewMatrixLocation(), m_spaceship.get_viewMatrix());
	m_background_meteors_shader.setUniformMatrix(m_background_meteors_shader.getViewMatrixLocation(), m_spaceship.get_viewMatrix());
	m_meteor_shader.setUniformMatrix(m_meteor_shader.getViewMatrixLocation(), m_spaceship.get_viewMatrix());
	m_rocket_shader.setUniformMatrix(m_rocket_shader.getViewMatrixLocation(), m_spaceship.get_viewMatrix());
	m_fuel_shader.setUniformMatrix(m_fuel_shader.getViewMatrixLocation(), m_spaceship.get_viewMatrix());
}

// function for checking if our score is in top 10
// and if it is, we can put our name and score to database.
void Manager::checkAndWriteScore()
{
	//clear console
	//system("cls");
	// show console
	ShowWindow(GetConsoleWindow(), SW_SHOW);

	std::vector<int> best_ten_scores = m_mySQL.getBestScores("spaceship", "best_score");
	bool is_new_score_best = false;
	const int top_ten_no = 10;
	if (top_ten_no > best_ten_scores.size())
	{
		// we have empty places, so add this score!
		if (m_score >= 1)
		{
			is_new_score_best = true;
		}
	}
	else
	{
		for (int i = 0; i < top_ten_no; i++)
		{
			if ((m_score > best_ten_scores[i]) && (m_score >= 1))
			{
				is_new_score_best = true;
				break;
			}
		}
	}
	
	if (is_new_score_best)
	{
		bool validName = false;
		std::string name;
		do {
			system("cls");
			std::cout << "name: " << name << "<-\n";
			std::cout << "Congrats! Your score is in top 10!\n";
			std::cout << "Write your name: ";
			std::getline(std::cin, name);
			std::cout << "\n";
			validName = (name.size() < 20) && (name.size() > 0);
			
			if (!validName)
			{
				if (name.size() != 0)
				{
					std::cout << "Name is too long!\nMax length of Name is 20 characters!\nTry Again ;)";
				}
				else
				{
					std::cout << "Name cannot be empty! Write something ;)";
				}
				getchar();
			}
		} while (!validName);
		m_mySQL.writeScore(name, m_score);
	}
}


// main function of manager. logic of game, input processing, creating and deleting objects
bool Manager::play(GLFWwindow * window, double deltaTime)
{
	///process keyboard input
	processInput(window);
	// if game is not paused and menu is hidden
	if (!m_show_menu)
	{
		if (!m_pauseGame)
		{
			//FreeConsole();
			/// create meteors 
			// new meteor after 1 sec if there is not too many meteors
			if ((glfwGetTime() - m_meteor_genereate_timer) > 1.0)
			{
				m_meteor_genereate_timer = glfwGetTime();
				createMeteors();
			}

			/// move of spaceship
			m_moveSpaceship(window, deltaTime);

			// increase loading ammo time
			m_loadingAmmoTimer += deltaTime;

			/// set view matrix for each shader -> camera!
			m_updateShadersMatrices();

			/// drawing objects
			m_drawAndMoveAllObjects(deltaTime);

			/// delete objects and check collisions
			//delete rockets and meteors when covered distance is too far
			distanceAutoDelete();
			m_checkAllCollisions();

		}
		else // game is paused and menu is hidden
		{
			/// drawing objects
			m_drawAllObjects();
			glm::vec4 aaa = textGen.render(m_shader_text, "Game paused!", (m_win_w / 2.0f - 180.0f), (m_win_h / 2.0f - 10.0f), 1.1f, glm::vec3(0.9f, 0.2f, 0.0f));
		}
	} 
	else //show menu independently on game state (paused or not)
	{
		//draw textBoxes with text Name
		for (auto&n : m_TextBoxesName)
		{
			n.draw();
		}
		//draw TextBoxes with text Score
		for (auto&s : m_TextBoxesScore)
		{
			s.draw();
		}
		// draw name and score values
		std::for_each(m_TextBoxesNameValue.begin(), m_TextBoxesNameValue.end(), [](TextBox &box) {box.draw(); });
		std::for_each(m_TextBoxesScoreValue.begin(), m_TextBoxesScoreValue.end(), [](TextBox &box) {box.draw(); });
		
		std::for_each(m_TextBoxesNumbers.begin(), m_TextBoxesNumbers.end(), [](TextBox &box) {box.draw(); });
	}

	
	/// check if game is over
	if (!m_gameOver)
	{
		return true;
	}
	else // gameOver!
	{
		//best score?
		checkAndWriteScore();
		return false;
	}
}

//set number of meteors available textures
void Manager::setMeteorsTexNo(int meteorTexNo)
{
	m_meteorTexNo = meteorTexNo;
}

//get number of meteors available textures
int Manager::getMeteorsTexNo() const
{
	return m_meteorTexNo;
}

// get view matrix from m_spaceship object
glm::mat4 Manager::getViewMatrix() const
{
	return m_spaceship.get_viewMatrix();
}

// mouse input callback
void Manager::mouseInput(double xoffset, double yoffset)
{
	if (!m_pauseGame)
	{
		//if spaceship has enough fuel
		if (m_spaceship.getFuel() > 0)
		{
			// process mouse input -> so we can rotate spaceship
			m_spaceship.mouseInput(xoffset, yoffset);
			// and decrease amount of fuel
			m_spaceship.useFuel(0.1);
		}
	}
}

// keyboard callback funtion
void Manager::processInput(GLFWwindow * window)
{
	static bool space_pushed = false;
	static bool key_p_pushed = false;
	static bool key_m_pushed = false;

	//
	//	** KEY ESC	**
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		m_gameOver = true;
	}

	if (!m_pauseGame)
	{
		//
		//	** SPACE	**
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			if (!space_pushed) //positive edge
			{
				createRocket();
			}
			space_pushed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
		{
			space_pushed = false;
		}
	}
	//
	//	** KEY P	**
	
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		{
			if (!key_p_pushed)
			{
				//make sure we are not in menu, because if we are, we can be in menu and unpause game!
				if (!m_show_menu)
				{
					m_pauseGame = !m_pauseGame;
				}
				std::cout << "P pushed, m_show_menu: " << m_show_menu << ", m_pause_game = " << m_pauseGame << '\n';

			}
			key_p_pushed = true;
		}
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE)
		{
			key_p_pushed = false;
		}
	//
	//	** KEY M	**
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		//if raise edge on key M
		if (!key_m_pushed)
		{
			//show menu
			m_show_menu = !m_show_menu;
			// update top ten scores vector
			topTen = m_mySQL.getTopTen("spaceship", "best_score");
			const float y_top = 550;
			float y = y_top;

			float x_number = 50.0f;
			float x_name = 0.0f;
			float x_nameValue = 0.0f;
			float x_score = 0.0f;
			float x_scoreValue = 0.0f;

			// render numbers 1...10
			for (int i = 0; i < topTen.size(); i++)
			{
				m_TextBoxesNumbers[i].updateTextBox(std::to_string(i+1), x_number, y, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
				glm::vec4 act_pos = m_TextBoxesNumbers[i].getPosition();
				if (x_name < act_pos.x + act_pos.z)
				{
					x_name = act_pos.x + act_pos.z;
				}

				y -= 50;
			}
			// render "name"
			y = y_top;
			x_name += 30; //spacing
			for (int i = 0; i < topTen.size(); i++)
			{
				m_TextBoxesName[i].updateTextBox("Name", x_name, y,1.0f, glm::vec3(0.0f, 0.8f, 0.0f));
				glm::vec4 act_pos = m_TextBoxesName[i].getPosition();
				if (x_nameValue < act_pos.x + act_pos.z)
				{
					x_nameValue = act_pos.x + act_pos.z;
				}
				y -= 50;
			}
			// render name value
			y = y_top;
			x_nameValue += 20; // spacing
			for (int i = 0; i < topTen.size(); i++)
			{
				m_TextBoxesNameValue[i].updateTextBox(topTen[i].name, x_nameValue, y, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
				glm::vec4 act_pos = m_TextBoxesNameValue[i].getPosition();
				if (x_score < act_pos.x + act_pos.z)
				{
					x_score = act_pos.x + act_pos.z;
				}
				y -= 50;
			}
			
			// render "score"
			y = y_top;
			x_score += 50; // spacing
			for (int i = 0; i < topTen.size(); i++)
			{
				m_TextBoxesScore[i].updateTextBox("Score", x_score, y, 1.0f, glm::vec3(0.0f, 0.8f, 0.0f));
				glm::vec4 act_pos = m_TextBoxesScore[i].getPosition();
				if (x_scoreValue < act_pos.x + act_pos.z)
				{
					x_scoreValue = act_pos.x + act_pos.z;
				}
				y -= 50;
			}			
			//score value
			y = y_top;
			y = y_top;
			x_scoreValue += 20; // spacing
			for (int i = 0; i < topTen.size(); i++)
			{
				m_TextBoxesScoreValue[i].updateTextBox(std::to_string(topTen[i].score), x_scoreValue, y, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
				y -= 50;
			}

			//and if game is not pasued, pause game OR if menu is hidden and game is paused, unpause game
			if (m_pauseGame ^ m_show_menu)
			{
				m_pauseGame = !m_pauseGame;
			}

			std::cout << "M pushed, m_show_menu: " << m_show_menu << ", m_pause_game = " << m_pauseGame << '\n';
		}
		key_m_pushed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE)
	{
		key_m_pushed = false;
	}
}

// function which creates fuel objects (from destroyed meteor)
// first argument -> position of object
// second argument -> percent of chance to generate object
void Manager::generateFuel(glm::vec3 position, int percentOfChance)
{
	int chance = rand() % 100;
	float x = position.x;
	float y = position.y;
	float z = position.z;
	if (chance < percentOfChance)
	{
		int texNo = rand() % m_fuelTexNo;
		m_fuel_obj_list.emplace_back(&m_fuel_shader, x, y, z, 0.3f, 0.3f, 0.3f, 1, 0.0f, 0.0f, 0.0f, texNo);
	}
}