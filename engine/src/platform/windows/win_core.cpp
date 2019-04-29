#include "pch.h"
#include "engine/core.h"
#include "GLFW/glfw3.h"
#include "win_input.h"

static bool gGLFWInitialized = false;

void engine::core::platform_startup()
{
	ENGINE_CORE_TRACE("Platform-specific startup: Windows");

	if(!gGLFWInitialized)
	{
		const int32 success = glfwInit();
		if(!success)
		{
			ENGINE_CORE_FATAL("Could not initialize GLFW!");
			startup_success = false;
		}
		else
			ENGINE_CORE_TRACE("GLFW initialized");

		gGLFWInitialized = true;
	}

	ENGINE_CORE_TRACE("Starting input manager");
	win_input::init();
}

void engine::core::platform_shutdown()
{
	ENGINE_CORE_TRACE("Platform-specific shutdown: Windows");

	ENGINE_CORE_TRACE("Stopping input manager");
	win_input::shutdown();

	if(gGLFWInitialized)
	{
		glfwTerminate();
		ENGINE_CORE_TRACE("GLFW terminated");
	}
}

