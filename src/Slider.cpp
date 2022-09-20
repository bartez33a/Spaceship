#include "..\headers\Slider.h"

// constructor for Slider object
// shader - shader program
// x,y,z - coordinates of bottom left corner of rectangle
// w, h - dimensions of slider
// time - initial value of timer preset
// RGB - colors of slider
Slider::Slider(Shader* shader, float x, float y, float z, float w, float h, float time, float R, float G, float B):
	m_x{x},
	m_y{y},
	m_z{z},
	m_w{w},
	m_h{h},
	m_R{R},
	m_G{G},
	m_B{B},
	m_time_of_animation{time},
	m_actual_time{0.f},
	m_shader{*shader},
	m_inner_rect{ *shader, x, y, z, 0.0f, h, R, G, B },
	m_outer_rect{ *shader, x, y, z, w, h, R, G, B } //at the beggining width of rectangle is equal to 0.0f
{

}

//destructor
Slider::~Slider()
{
}

// function for drawing slider
// acutalTime is value of time for animation
// if actualTime >= m_time_of_animation (that is set via setTime(double) member function)
// then Silder is fully filled
void Slider::animate(float actualTime)
{
	//set m_actual_time
	m_actual_time = actualTime;
	//calculate new dimensions of inner rectangle
	float w = m_w * m_actual_time / m_time_of_animation;

	//draw rectangles
	m_outer_rect.drawLines();
	m_inner_rect.updateDimensions(m_x, m_y, w, m_h);
	m_inner_rect.draw();

	// limit value of m_actual_time
	if (m_actual_time >= m_time_of_animation)
	{
		m_actual_time = m_time_of_animation;
	}
}

// function for reset current value of timer
void Slider::reset()
{
	m_actual_time = 0.0f;
	// draw empty slider
	animate(0.0f);
}

// function for changing timer preset value
void Slider::setTime(float time)
{
	m_time_of_animation = time;
}
