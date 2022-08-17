#include "..\headers\Manager.h"

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

void print_position_sphere(const Sphere& s, std::string name) {
	glm::vec3 pos = s.getPosition();
	float x1 = pos.x;
	float y1 = pos.y;
	float z1 = pos.z;

	float r1 = s.getRadius();

	std::cout << name << " Position: (" << x1 << ",\t" << y1 << ",\t" << z1 << ") radius: (" << r1 << ")\n";
}

Manager::Manager()
{
	srand(time(NULL));

	//player
	m_score = 0;
	m_rocketsNo = 10;
	m_gameOver = false;

	//shooting 
	m_loadingAmmoTime = 1.0; //time to load ammo
	m_loadingAmmoTimer = m_loadingAmmoTime; //so we can shoot at the beginning 

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

bool Manager::checkCollisionSphere(const Sphere & s1, const Sphere & s2)
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

void Manager::createMeteors(Shader *shader)
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
			pos_y = float((rand() % 10000 - 5000)) / 1000.0f / 2.5f;
			pos_z = float((rand() % 10000 - 5000)) / 1000.0f / 5.0f;
			radius = float((rand() % 1000)) / 1000.0f + .1f;
		}
		else
		{
			do
			{
				pos_x = float((rand() % 10000 - 5000)) / 1000.0f / 2.5f;
				pos_y = float((rand() % 10000 - 5000)) / 1000.0f / 2.5f;
				pos_z = float((rand() % 10000 - 5000)) / 1000.0f / 5.0f;
				radius = float((rand() % 1000)) / 1000.0f + .1f;
				Sphere m2(shader, pos_x, pos_y, pos_z, radius);

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
		m_meteors.emplace_back(shader, pos_x, pos_y, pos_z, radius, 1, ((m_meteors_ctr++) % m_meteorTexNo));
	}
}

//create background -> add Point to list of background points
void Manager::createBackground(Shader *shader)
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
			m_background_meteors.emplace_back(shader, pos_x, pos_y, pos_z, 1.0f);
		} //for sectorCount
	} //for stackCount
} //createBackground()

void Manager::createRocket(Shader *shader)
{
	glm::vec3 camPos = m_spaceship.getCamPos();
	glm::vec3 camFront = m_spaceship.getCamFront();

	if (m_rocketsNo > 0)
	{
		if (m_loadingAmmoTimer >= m_loadingAmmoTime)
		{
		
		m_rockets.emplace_back(shader, camPos.x, camPos.y, camPos.z, camFront);
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
	static glm::vec3 spaceship_pos = m_spaceship.getCamPos();
	m_loadingAmmoTimer += deltaTime;
	m_spaceship.Input(window, deltaTime);

	if (m_spaceship.getCamPos() != spaceship_pos)
	{
		spaceship_pos = m_spaceship.getCamPos();
		m_spaceship.useFuel(0.1);
	}

	std::cout << "Spaceship fuel: " << m_spaceship.getFuel() << '\n';

	//draw background meteors
	for (auto &b : m_background_meteors)
	{
		b.draw();
	}

	// draw meteors
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
