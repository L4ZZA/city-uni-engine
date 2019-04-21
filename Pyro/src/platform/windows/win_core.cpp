#include "pyro_pch.h"
#include "pyro/core.h"
#include "GLFW/glfw3.h"
#include "win_input.h"

static bool gGLFWInitialized = false;

void pyro::core::platform_startup()
{
	PYRO_CORE_TRACE("Platform-specific startup: Windows");

	if (!gGLFWInitialized)
	{
		const int32 success = glfwInit();
		if (!success) 
		{
			PYRO_CORE_FATAL("Could not initialize GLFW!");
			StartupSuccess = false;
		}
		else
			PYRO_CORE_TRACE("GLFW initialized");

		gGLFWInitialized = true;
	}

	PYRO_CORE_TRACE("Starting input manager");
	win_input::init();
}

void pyro::core::platform_shutdown()
{
	PYRO_CORE_TRACE("Platform-specific shutdown: Windows");

	PYRO_CORE_TRACE("Stopping input manager");
	win_input::shutdown();

	if (gGLFWInitialized) 
	{
		glfwTerminate();
		PYRO_CORE_TRACE("GLFW terminated");
	}
}

