#include "pyro_pch.h"
#include "camera.h"
#include "platform/windows/win_input.h"
#include "pyro/key_codes.h"

void pyro::camera::move(double dt)
{
	float f_dt = static_cast<float>(dt);
	if(win_input::key_pressed(key_codes::KEY_W))
	{
		m_position.z += f_dt;
	}
	if(win_input::key_pressed(key_codes::KEY_S))
	{
		m_position.z -= f_dt;
	}
	if(win_input::key_pressed(key_codes::KEY_A))
	{
		m_position.x += f_dt;
	}
	if(win_input::key_pressed(key_codes::KEY_D))
	{
		m_position.x -= f_dt;
	}
}
