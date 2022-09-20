#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aColor;

out vec3 ourColor;

uniform mat4 projection;


void main()
{
	ourColor = aColor;
	gl_Position = projection * vec4(aPos.xy, 0.0, 1.0);
	
}