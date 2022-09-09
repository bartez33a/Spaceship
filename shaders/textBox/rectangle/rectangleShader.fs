#version 330 core

in vec3 ourColor;
out vec4 color;


void main()
{
	color = vec4(ourColor, 1.0) ;
	//color = vec4(0.1, 0.5, 0.0, 1.0) ;
}