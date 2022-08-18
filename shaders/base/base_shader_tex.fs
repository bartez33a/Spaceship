#version 330 core
out vec4 FragColor;

in vec3 ourColor; 
in vec2 TexCoord;

uniform sampler2D texture0;


void main()
{
	FragColor = texture(texture0, TexCoord);	
	//FragColor = vec4(1.0, 0.0, 0.0, 1.0);	
}
