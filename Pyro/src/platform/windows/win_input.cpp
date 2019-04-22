#include "pyro_pch.h"
#include "win_input.h"
#include "pyro/application.h"
#include "GLFW/glfw3.h"

pyro::input* pyro::input::s_instance;

void pyro::win_input::init()
{
	s_instance = new win_input();
}

void pyro::win_input::shutdown()
{
	SAFE_RELEASE(s_instance);
}

bool pyro::win_input::key_pressed_impl(int key_code) const
{
	auto window = application::instance().get_window().native_window();
	auto state = glfwGetKey(static_cast<GLFWwindow*>(window), key_code);
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool pyro::win_input::mouse_button_pressed_impl(int button) const
{
	auto window = application::instance().get_window().native_window();
	auto state = glfwGetMouseButton(static_cast<GLFWwindow*>(window), button);
	return state == GLFW_PRESS;
}

std::pair<float, float> pyro::win_input::mouse_position_impl() const
{
	auto window = application::instance().get_window().native_window();
	double xpos, ypos;
	glfwGetCursorPos(static_cast<GLFWwindow*>(window), &xpos, &ypos);

	return { static_cast<float>(xpos), static_cast<float>(ypos) };
}

float pyro::win_input::mouse_x_impl() const
{
	// c++ 17 way to assign std::pair(s)
	auto[x, y] = mouse_position_impl();
	return x;
}

float pyro::win_input::mouse_y_impl() const
{
	auto[x, y] = mouse_position_impl();
	return y;
}
