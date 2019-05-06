#include "pch.h"
#include "win_window.h"

#include "engine/core.h"
#include "engine/events/application_event.h"
#include "engine/events/key_event.h"
#include "engine/events/mouse_event.h"

#include "platform/opengl/context.h"

//=============================================================================

namespace engine
{
	static bool s_glfw_initialized = false;

	static void glfw_error_callback(int error_code, const char * description)
	{
		LOG_CORE_ERROR("[win_window] GLFW Error ({0}): {1}", error_code, description);
	}
}

engine::window* engine::window::create(const std::string& name, window_props const& props)
{
	return new win_window(name, props);
}

engine::win_window::win_window(const std::string& name, window_props const& props)
{
	init(name, props);
}

engine::win_window::~win_window()
{
	shut_down();
}

void engine::win_window::on_update()
{
	glfwPollEvents();
	m_context->swap_buffers();
}

void engine::win_window::vsync(bool enabled)
{
	if(enabled)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);

	m_data.vsync = enabled;
}

bool engine::win_window::vsync()
{
	return m_data.vsync;
}

void engine::win_window::init(const std::string& name, window_props const& props)
{
	if(!core::startup_success)
		CORE_ASSERT(false, "[win_window] GLFW was not initialized!");

	m_data.title = name;
	m_data.x = props.x;
	m_data.y = props.y;
	m_data.width = props.width;
	m_data.height = props.height;
	m_data.vsync = props.vsync;

	LOG_CORE_INFO("[win_window] Creating window {0} [{1},{2},{3},{4}]", 
		name, props.y, props.y, props.width, props.height);


	m_window = glfwCreateWindow(
		static_cast<int>(props.width),
		static_cast<int>(props.height),
		name.c_str(),
		nullptr, nullptr);

	// set window origin
	LOG_CORE_INFO("[win_window] Setting window origin.");
	glfwSetWindowPos(m_window, props.x, props.y);

	LOG_CORE_INFO("[win_window] Creating graphics context.");
	m_context = new opengl_context(m_window);
	m_context->init();

	// we're telling glfw to pass the window_data struct to all the defined callbacks
	// so that we ca work with our defined data.
	LOG_CORE_INFO("[win_window] Setting WindowUserPointer.");
	glfwSetWindowUserPointer(m_window, &m_data);

	LOG_CORE_INFO("[win_window] Setting vsync to {}.", m_data.vsync ? "true" : "false");
	vsync(true);


	// GLFW callbacks
	LOG_CORE_INFO("[win_window] Setting WindowSizeCallback.");
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
		window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
		data.width = width;
		data.height = height;

		window_resize_event event(width, height);
		data.event_callback(event);
	});

	LOG_CORE_INFO("[win_window] Setting WindowCloseCallback.");
	glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){
		window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
		window_closed_event event;
		data.event_callback(event);
	});

	LOG_CORE_INFO("[win_window] Setting KeyCallback.");
	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
		window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

		switch(action)
		{
			case GLFW_PRESS:
			{
				key_pressed_event event(key, 0);
				data.event_callback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				key_released_event event(key);
				data.event_callback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				key_pressed_event event(key, 1);
				data.event_callback(event);
				break;
			}
		}
	});

	LOG_CORE_INFO("[win_window] Setting CharCallback.");
	glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int key){
		window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

		key_typed_event event(key);
		data.event_callback(event);
	});

	LOG_CORE_INFO("[win_window] Setting MouseButtonCallback.");
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods){
		window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

		switch(action)
		{
			case GLFW_PRESS:
			{
				mouse_button_pressed_event event(button);
				data.event_callback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				mouse_button_released_event event(button);
				data.event_callback(event);
				break;
			}
		}
	});

	LOG_CORE_INFO("[win_window] Setting ScrollCallback.");
	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset){
		window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
		mouse_scrolled_event event(static_cast<float>(xOffset), static_cast<float>(yOffset));
		data.event_callback(event);
	});

	LOG_CORE_INFO("[win_window] Setting CursorPosCallback.");
	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos){
		window_data &data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));
		mouse_moved_event event(static_cast<float>(xPos), static_cast<float>(yPos));
		data.event_callback(event);
	});
}

void engine::win_window::shut_down()
{
	LOG_CORE_INFO("[win_window] Destroing window.");
	glfwDestroyWindow(m_window);
}
