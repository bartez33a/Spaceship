#include "..\headers\Manager.h"

// TODO -> change to lambda
void print_position_cubes(const Cube& c, std::string name) {
	glm::vec3 pos = c.getPosition();
	glm::vec3 dim = c.getDimensions();
	float x1 = pos.x;
	float y1 = pos.y;
	float z1 = pos.z;

	float w1 = dim.x;
	float h1 = dim.y;
	float l1 = dim.z;

	std::cout << name << " Position: (" << x1 << ",\t" << y1 << ",\t" << z1 << ") dim: (" << w1 << ",\t" << h1 << ",\t" << l1 << ")\n";
}

// TODO -> change to lambda
void print_position_sphere(const Sphere& s, std::string name) {
	glm::vec3 pos = s.getPosition();
	float x1 = pos.x;
	float y1 = pos.y;
	float z1 = pos.z;

	float r1 = s.getRadius();

	std::cout << name << " Position: (" << x1 << ",\t" << y1 << ",\t" << z1 << ") radius: (" << r1 << ")\n";
}

Manager::Manager() : m_base_shader{ "shaders/base/base_shader_tex.vs", "shaders/base/base_shader_tex.fs" },
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
m_fuel_shader_tex1 {"textures/fuel/fuel2.jpg", GL_TEXTURE1},
m_fuelTexNo{2}
{
	srand(time(NULL));

	glm::mat4 mm = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);

	//background meteors
	m_background_meteors_shader.updateMatrices(mm, m_spaceship.get_viewMatrix(), projection);

	//draw base
	m_base_shader.updateMatrices(mm, m_spaceship.get_viewMatrix(), projection);
	//updateMatrices function also make shader program active
	m_base_shader.setUniformInt("texture0", 0);
	// texture for base

	//meteors
	m_meteor_shader.updateMatrices(mm, m_spaceship.get_viewMatrix(), projection);
	//0 ->  glActiveTexture(GL_TEXTURE0); // activate texture unit 0
	glUniform1i(glGetUniformLocation(m_meteor_shader.get_ID(), "texture0"), 0); // manually
	glUniform1i(glGetUniformLocation(m_meteor_shader.get_ID(), "texture1"), 1); // manually
	glUniform1i(glGetUniformLocation(m_meteor_shader.get_ID(), "texture2"), 2); // manually
	setMeteorsTexNo(3);

	//rockets
	m_rocket_shader.use();
	m_rocket_shader.updateMatrices(mm, m_spaceship.get_viewMatrix(), projection);


	m_fuel_shader.use();
	m_fuel_shader.updateMatrices(mm, m_spaceship.get_viewMatrix(), projection);
	glUniform1i(glGetUniformLocation(m_fuel_shader.get_ID(), "texture0"), 0); // manually
	glUniform1i(glGetUniformLocation(m_fuel_shader.get_ID(), "texture1"), 1); // manually

	//player
	m_score = 0;
	m_rocketsNo = 10;
	m_gameOver = false;

	//shooting 
	m_loadingAmmoTime = 1.0; //time to load ammo
	m_loadingAmmoTimer = m_loadingAmmoTime; //so we can shoot at the beginning 

	//base HP
	m_base_HP = 10;

}


Manager::~Manager()
{
}

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


unsigned int Manager::m_meteors_ctr = 0;

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
			pos_x = float((rand() % 10000 - 5000)) / 1000.0f / 2.5f;
			pos_y = float((rand() % 10000 - 5000)) / 1000.0f / 5.0f;
			pos_z = float((rand() % 10000 - 20000)) / 1000.0f; //
			radius = float((rand() % 1000)) / 1000.0f + .1f;
		}
		else
		{
			do
			{
				pos_x = float((rand() % 10000 - 5000)) / 1000.0f / 2.5f;
				pos_y = float((rand() % 10000 - 5000)) / 1000.0f / 5.0f;
				pos_z = float((rand() % 10000 - 20000)) / 1000.0f;
				radius = float((rand() % 1000)) / 1000.0f + .1f;
				Sphere m2(&m_meteor_shader, pos_x, pos_y, pos_z, radius);

				for (auto& m : m_meteors)
				{
					//valid_pos -> gdy brak kolizji == true
					valid_pos = !checkCollisionSphere(m, m2);
					if (!valid_pos)
					{
						// gdy jest kolizja -> przerwij i losuj ponownie!
						break;
					}
				}// for each meteor

			} while (!valid_pos);
		}
		m_meteors.emplace_back(&m_meteor_shader, pos_x, pos_y, pos_z, radius, 1, ((m_meteors_ctr++) % m_meteorTexNo));
	}
}

//create background -> add Point to list of background points
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

void Manager::createRocket()
{
	glm::vec3 camPos = m_spaceship.getCamPos();
	glm::vec3 camFront = m_spaceship.getCamFront();

	if (m_rocketsNo > 0)
	{
		if (m_loadingAmmoTimer >= m_loadingAmmoTime)
		{
		
		m_rockets.emplace_back(&m_rocket_shader, camPos.x, camPos.y, camPos.z, camFront);
		m_rocketsNo--;
		std::cout << "Ammo left: " << m_rocketsNo << '\n';
		m_loadingAmmoTimer = 0.0;
		}
	}
	else
	{
		std::cout << "No more ammo!\n";
		m_gameOver = true;
	}
}

// get list of Meteors
std::list<Meteor>& Manager::getMeteors()
{
	return m_meteors;
}

// delete objects when covered distance is too far
void Manager::distanceAutoDelete()
{
	//delete rockets
	for (std::list<Rocket>::iterator it = m_rockets.begin(); it != m_rockets.end();)
	{
		if ((*it).checkDistance() > 15.0f)
		{
			it = m_rockets.erase(it);
		}
		else
		{
			it++;
		}
	}

	//delete meteors
	for (std::list<Meteor>::iterator it = m_meteors.begin(); it != m_meteors.end();)
	{
		if ((*it).checkDistance() > 30.0f)
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

// get list of rockets
std::list<Rocket>& Manager::getRockets()
{
	return m_rockets;
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

// main function of manager. logic of game
bool Manager::play(GLFWwindow * window, double deltaTime)
{
	/// create meteors 
	// new meteor after 1 sec if there is not too many meteors
	if ((glfwGetTime() - m_meteor_genereate_timer) > 1.0)
	{
		m_meteor_genereate_timer = glfwGetTime();
		createMeteors();
	}
	
	/// move of spaceship
	static glm::vec3 spaceship_pos = m_spaceship.getCamPos();
	//if we have enough fuel, get user input
	if(m_spaceship.getFuel() > 0)
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

	// loading ammo time
	m_loadingAmmoTimer += deltaTime;
	
	/// set view matrix for each shader -> camera!
	glm::mat4 mm = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);
	//update view matrix of shaders 
	m_base_shader.setUniformMatrix(m_base_shader.getViewMatrixLocation(), m_spaceship.get_viewMatrix());
	m_background_meteors_shader.setUniformMatrix(m_background_meteors_shader.getViewMatrixLocation(), m_spaceship.get_viewMatrix());
	m_meteor_shader.setUniformMatrix(m_meteor_shader.getViewMatrixLocation(), m_spaceship.get_viewMatrix());
	m_rocket_shader.setUniformMatrix(m_rocket_shader.getViewMatrixLocation(), m_spaceship.get_viewMatrix());
	m_fuel_shader.setUniformMatrix(m_fuel_shader.getViewMatrixLocation(), m_spaceship.get_viewMatrix());

	/// drawing objects
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


	/// delete objects and check collisions
	//delete rockets and meteors when covered distance is too far
	distanceAutoDelete();

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
	for (std::list<Meteor>::iterator it = m_meteors.begin(); it != m_meteors.end();it++)
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

	
	//check if we get fuel
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

	}

	/// check if game is over
	if (!m_gameOver)
	{
		return true;
	}
	else
	{
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

glm::mat4 Manager::getViewMatrix() const
{
	return m_spaceship.get_viewMatrix();
}

void Manager::mouseInput(double xoffset, double yoffset)
{
	if (m_spaceship.getFuel() > 0)
	{
		m_spaceship.mouseInput(xoffset, yoffset);
		m_spaceship.useFuel(0.1);
	}

}

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
