#pragma once

#include "Rectangle.h"
#include "Shader.h"

class Slider
{
public:
	Slider(Shader* shader, float x, float y, float z, float w, float h, float time, float R, float G, float B);
	~Slider();
	// this function draws slider
	void animate(float actualTime);
	// this function is for reset slider - draw empty rectangle
	void reset();
	// set preset value of timer
	void setTime(float time);
private:
	//shader program for drawing rectangle
	Shader m_shader;
	//position of bottom left corner
	float m_x, m_y, m_z;
	// width and height
	float m_w, m_h;
	// RGB colors of rectangle
	float m_R, m_G, m_B;
	//time of animation
	float m_time_of_animation;
	//actual time
	float m_actual_time;
	// inner rectangle with fill
	shapes::Rectangle m_inner_rect;
	// outer rectangle without fill - box
	shapes::Rectangle m_outer_rect;


};

