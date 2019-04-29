#include "pch.h"
#include "camera.h"
#include "platform/windows/win_input.h"
#include "engine/key_codes.h"

void engine::camera::move(double dt)
{
	float f_dt = static_cast<float>(dt);
	if(win_input::key_pressed(key_codes::KEY_W))
	{
		m_position.z += s_speed * f_dt;
	}
	if(win_input::key_pressed(key_codes::KEY_S))
	{
		m_position.z -= s_speed * f_dt;
	}
	if(win_input::key_pressed(key_codes::KEY_A))
	{
		m_position.x += s_speed * f_dt;
	}
	if(win_input::key_pressed(key_codes::KEY_D))
	{
		m_position.x -= s_speed * f_dt;
	}
}
