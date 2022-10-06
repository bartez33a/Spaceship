#version 330 core
out vec4 FragColor;

in vec3 ourColor; 
in vec2 TexCoord;

// diffuse textures
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_diffuse3;
uniform sampler2D texture_diffuse4;
uniform sampler2D texture_diffuse5;
uniform sampler2D texture_diffuse6;
uniform sampler2D texture_diffuse7;
uniform sampler2D texture_diffuse8;
uniform sampler2D texture_diffuse9;

// specular textures
uniform sampler2D texture_specular1;
uniform sampler2D texture_specular2;
uniform sampler2D texture_specular3;
uniform sampler2D texture_specular4;
uniform sampler2D texture_specular5;
uniform sampler2D texture_specular6;
uniform sampler2D texture_specular7;
uniform sampler2D texture_specular8;
uniform sampler2D texture_specular9;

uniform int tex;

void main()
{
	if (tex == 0)
	{
		FragColor = vec4(0.2f, 0.2f, 0.2f, 1.0f);
	}
	else if (tex == 1)
	{
		FragColor = texture(texture_diffuse1, TexCoord);	
	}
	else if (tex == 2)
	{
		FragColor = texture(texture_diffuse2, TexCoord);	
	}
	else if (tex == 3)
	{
		FragColor = texture(texture_diffuse3, TexCoord);	
	}
	else if (tex == 4)
	{
		FragColor = texture(texture_diffuse4, TexCoord);	
	}
	else if (tex == 5)
	{
		FragColor = texture(texture_diffuse5, TexCoord);	
	}
	else if (tex == 6)
	{
		FragColor = texture(texture_diffuse6, TexCoord);	
	}
	else if (tex == 7)
	{
		FragColor = texture(texture_diffuse7, TexCoord);	
	}
	else if (tex == 8)
	{
		FragColor = texture(texture_diffuse8, TexCoord);	
	}
	else if (tex == 9)
	{
		FragColor = texture(texture_diffuse9, TexCoord);	
	}
	else if (tex == 10)
	{
		FragColor = texture(texture_specular1, TexCoord);	
	}
	else if (tex == 11)
	{
		FragColor = texture(texture_specular2, TexCoord);	
	}
	else if (tex == 12)
	{
		FragColor = texture(texture_specular3, TexCoord);	
	}
	else if (tex == 13)
	{
		FragColor = texture(texture_specular4, TexCoord);	
	}
	else if (tex == 14)
	{
		FragColor = texture(texture_specular5, TexCoord);	
	}
	else if (tex == 15)
	{
		FragColor = texture(texture_specular6, TexCoord);	
	}
	else if (tex == 16)
	{
		FragColor = texture(texture_specular7, TexCoord);	
	}
	else if (tex == 17)
	{
		FragColor = texture(texture_specular8, TexCoord);	
	}
	else if (tex == 18)
	{
		FragColor = texture(texture_specular9, TexCoord);	
	}
	
}
