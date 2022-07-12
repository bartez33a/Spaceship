#include "../headers/Shader.h"


Shader::Shader()
{

}

//read shaders code from files vs_file and fs_file and try to create shader program.
Shader::Shader(const char* vs_file, const char* fs_file)
{
	std::fstream VS_filestream(vs_file, std::ios::in); //vertex shader file
	std::fstream FS_filestream(fs_file, std::ios::in); //fragment shader file
	std::string vs_code; //vertex shader file content
	std::string fs_code; //fragment shader file content


	// Read Vertex shader code
	if (VS_filestream.is_open())
	{
		//read vs from file
		while (!VS_filestream.eof())
		{
			std::string tmp;
			std::getline(VS_filestream, tmp);
			vs_code += (tmp + '\n');
		}
		//std::cout << "VS: \n" << vs_code << '\n';
		VS_filestream.close();
	}
	else
	{
		std::cout << "Couldn't open vertex shader file!\n";
		getchar();
		exit (EXIT_FAILURE);
	}

	// read Fragment shader code
	if (FS_filestream.is_open())
	{
		//read vs from file
		while (!FS_filestream.eof())
		{
			std::string tmp;
			std::getline(FS_filestream, tmp);
			fs_code += (tmp + '\n');
		}
		//std::cout << "VS: \n" << fs_code << '\n';
		FS_filestream.close();
	}
	else
	{
		std::cout << "Couldn't open fragment shader file!\n";
		getchar();
		exit(EXIT_FAILURE);
	}

/*  NOW WE CAN COMPILE OUR SHADERS */

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	const char * vs_code_cStr = vs_code.c_str();
	glShaderSource(vertexShader, 1, &vs_code_cStr, NULL);
	glCompileShader(vertexShader);

	//check if compile was successfull
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// compile fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char * fs_code_cStr = fs_code.c_str();

	glShaderSource(fragmentShader, 1, &fs_code_cStr, NULL);
	glCompileShader(fragmentShader);

	//check if compile was successfull
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// now we can create shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	//checking if linking was successfull
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR glShaderProgram couldn't be linked!\n" <<
			infoLog << std::endl;

		std::cout << "vs:" << vs_file << " fs: " << fs_file << '\n';
	}


	//and we can delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//look for matrix location in shader
	//if location == -1, then uniform is not present in shader
	modelLoc = glGetUniformLocation(ID, "model");
	viewLoc = glGetUniformLocation(ID, "view");
	projectionLoc = glGetUniformLocation(ID, "projection");

	//initialize uniform matrix
	model = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	projection = glm::mat4(1.0f);
}


Shader::~Shader()
{
	//we can delete shader program
	glDeleteProgram(ID);
}

//use shader program
void Shader::use()
{
	glUseProgram(ID);
}

//get ID of shader program
unsigned int Shader::get_ID() const
{
	return ID;
}

void Shader::createShader(const char* vs_file, const char* fs_file)
{
	std::fstream VS_filestream(vs_file, std::ios::in); //vertex shader file
	std::fstream FS_filestream(fs_file, std::ios::in); //fragment shader file
	std::string vs_code; //vertex shader file content
	std::string fs_code; //fragment shader file content

						 // Read Vertex shader code
	if (VS_filestream.is_open())
	{
		//read vs from file
		while (!VS_filestream.eof())
		{
			std::string tmp;
			std::getline(VS_filestream, tmp);
			vs_code += (tmp + '\n');
		}
		//std::cout << "VS: \n" << vs_code << '\n';
		VS_filestream.close();
	}
	else
	{
		std::cout << "Couldn't open vertex shader file!\n";
		getchar();
		exit(EXIT_FAILURE);
	}

	// read Fragment shader code
	if (FS_filestream.is_open())
	{
		//read vs from file
		while (!FS_filestream.eof())
		{
			std::string tmp;
			std::getline(FS_filestream, tmp);
			fs_code += (tmp + '\n');
		}
		//std::cout << "VS: \n" << fs_code << '\n';
		FS_filestream.close();
	}
	else
	{
		std::cout << "Couldn't open fragment shader file!\n";
		getchar();
		exit(EXIT_FAILURE);
	}

	/*  NOW WE CAN COMPILE OUR SHADERS */

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	const char * vs_code_cStr = vs_code.c_str();
	glShaderSource(vertexShader, 1, &vs_code_cStr, NULL);
	glCompileShader(vertexShader);

	//check if compile was successfull
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// compile fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char * fs_code_cStr = fs_code.c_str();

	glShaderSource(fragmentShader, 1, &fs_code_cStr, NULL);
	glCompileShader(fragmentShader);

	//check if compile was successfull
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	// now we can create shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	//checking if linking was successfull
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR glShaderProgram couldn't be linked!\n" <<
			infoLog << std::endl;

		std::cout << "vs:" << vs_file << " fs: " << fs_file << '\n';
	}


	//and we can delete shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::setUniformMatrix(int location, glm::mat4 matrix)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniformfloat(std::string name, float value)
{

	glUniform1f(glGetUniformLocation(ID, name.c_str()), value); // manually

}

int Shader::getModelMatrixLocation() const
{
	return modelLoc;
}

int Shader::getViewMatrixLocation() const
{
	return viewLoc;
}

int Shader::getProjectionMatrixLocation() const
{
	return projectionLoc;
}
