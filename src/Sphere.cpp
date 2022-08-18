#include "../headers/Sphere.h"


static int id = 0;
Sphere::Sphere(Shader *shader, float x, float y, float z, float r, float R, float G, float B) : Shape(shader, x, y, z, R, G, B), radius{ r }
{
	position.x = x;
	position.y = y;
	position.z = z;

	init_pos = position;

	modelMatrix = glm::mat4(1.0f);
	id_no = id++;

	struct vertex 
	{
		float x, y, z;
		float R, G, B;
	};

	vertex v;
	//colors
	v.R = R;
	v.G = G;
	v.B = B;

	std::vector<vertex> vertices;

	float PI = 3.14159f;
	float sectorCount = 36.0;
	float stackCount = 18.0;
	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;
	float xy;

	//sector -> longitude
	//stack -> latitude
	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
		v.z = z_ + radius * sin(stackAngle); // axis Z
		xy = radius * cos(stackAngle); 
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi
			v.x = x_ + xy * cos(sectorAngle);
			v.y = y_ + xy * sin(sectorAngle);
			vertices.push_back(v);
		}
	}

	//use indices for drawing
	std::vector<unsigned int> indices;
	std::vector<unsigned int> lineIndices;
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * int(sectorCount + 1);     // beginning of current stack
		k2 = k1 + (int)sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}

			// store indices for lines
			// vertical lines for all stacks, k1 => k2
			lineIndices.push_back(k1);
			lineIndices.push_back(k2);
			if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
			{
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}
		}
	}

	vertices_number = (int)vertices.size();
	indices_number = (int)indices.size();

	//OpenGL
	vbo.LoadBufferData(reinterpret_cast<float*>(vertices.data()), (int)vertices.size() * sizeof(vertex));
	vao.BindBuffer();
	ebo.LoadBufferData(reinterpret_cast<GLuint*>(indices.data()), sizeof(GLuint) * (int)indices.size());
	vao.SetAttribPointer(vbo, 0, 3, 6, 0); //vertex
	vao.SetAttribPointer(vbo, 1, 3, 6, 3); //color
	vao.UnbindBuffer();
	ebo.UnbindBuffer();
}

//constructor of Sphere object with texture
Sphere::Sphere(Shader *shader, float x, float y, float z, float r, int rep, int tex_no, float R, float G, float B): Shape(shader, x, y, z, R, G, B), radius{ r }
{
	//texture  number
	m_tex_no = tex_no;
	position.x = x;
	position.y = y;
	position.z = z;

	auto print_sphere = [=]() {
		std::cout << "Creating sphere id: " << id_no << 
			", tex_no: " << m_tex_no <<
			" x = " << x << " y =  " << y << " z = " << z << 
			"radius = " << r << '\n';
	};

	//print_sphere();
	
	init_pos = position;

	modelMatrix = glm::mat4(1.0f);
	id_no = id++;

	struct vertex
	{
		float x, y, z;
		float R, G, B;
		float t1, t2;
	};

	vertex v;
	//colors
	v.R = R;
	v.G = G;
	v.B = B;

	std::vector<vertex> vertices;

	float PI = 3.14159f;
	float sectorCount = 36.0;
	float stackCount = 18.0;
	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;
	float xy;

	//sector -> longitude
	//stack -> latitude
	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep; // starting from pi/2 to -pi/2
		v.z = z_ + radius * sin(stackAngle); // axis Z
		xy = radius * cos(stackAngle);
		v.t2 = float(i) / stackCount * rep;
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi
			v.x = x_ + xy * cos(sectorAngle);
			v.y = y_ + xy * sin(sectorAngle);
			v.t1 = float(j) / sectorCount * rep;
			vertices.push_back(v);
		}
	}

	//use indices for drawing
	std::vector<unsigned int> indices;
	std::vector<unsigned int> lineIndices;
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (int)(sectorCount + 1);     // beginning of current stack
		k2 = k1 + (int)sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}

			// store indices for lines
			// vertical lines for all stacks, k1 => k2
			lineIndices.push_back(k1);
			lineIndices.push_back(k2);
			if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
			{
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}
		}
	}

	vertices_number = (int)vertices.size();
	indices_number = (int)indices.size();

	//OpenGL
	vbo.LoadBufferData(reinterpret_cast<float*>(vertices.data()), (int)vertices.size() * sizeof(vertex));
	vao.BindBuffer();
	ebo.LoadBufferData(reinterpret_cast<GLuint*>(indices.data()), sizeof(GLuint) * (int)indices.size());
	vao.SetAttribPointer(vbo, 0, 3, 8, 0); // vertex
	vao.SetAttribPointer(vbo, 1, 3, 8, 3); // color
	vao.SetAttribPointer(vbo, 2, 2, 8, 6); // texture
	vao.UnbindBuffer();
	ebo.UnbindBuffer();
}




Sphere::~Sphere()
{
	//std::cout << "Sphere destructor no. " << id_no << "\n";
}

void Sphere::draw()
{
	shader_->use();	
	shader_->setUniformMatrix(shader_->getModelMatrixLocation(), modelMatrix);
	vao.BindBuffer();

	glDrawElements(GL_TRIANGLES, indices_number, GL_UNSIGNED_INT, 0);
	vao.UnbindBuffer();
}

void Sphere::draw_tex()
{
	shader_->use();
	//select texture
	shader_->setUniformInt("tex", m_tex_no);
	shader_->setUniformMatrix(shader_->getModelMatrixLocation(), modelMatrix); //translate object
	vao.BindBuffer();
	//std::cout << "Draw meteor " << id_no << " m_tex_no = " << m_tex_no << '\n';
	glDrawElements(GL_TRIANGLES, indices_number, GL_UNSIGNED_INT, 0);
	vao.UnbindBuffer();
}


float Sphere::getRadius() const
{
	return radius;
}

glm::vec3 Sphere::getPosition() const
{
	return position;
}
