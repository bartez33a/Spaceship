#version 330 core
out vec4 FragColor;

in vec3 ourColor; 
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int tex;

void main()
{
	if (tex == 0)
	{
		FragColor = texture(texture0, TexCoord);	
		//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else if (tex == 1)
	{
		FragColor = texture(texture1, TexCoord);
		//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else if (tex == 2)
	{
		FragColor = texture(texture2, TexCoord);
		//FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	//FragColor = mix(texture(texture1, vec2(TexCoord.x, TexCoord.y)), texture(texture0, TexCoord), 0.5);
}
