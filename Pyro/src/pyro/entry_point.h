#pragma once
#include "core.h"

//=============================================================================
// Specifies the main entry point of the application regardless of the 
// platform in which the project will be built onto.
// The engine is supposed to handle the pain of creating it because of this.
//=============================================================================

#ifdef PYRO_PLATFORM_WIN

/// \brief Externally declared in the executable project. 
extern pyro::application* pyro::create_application();

int main(int argc, char** argv)
{
#ifdef PYRO_DEBUG
	auto hwnd = GetConsoleWindow();
	SetWindowPos(hwnd, HWND_TOP, 0, static_cast<int>(1080 * 0.72), 1920, static_cast<int>(1080 * 0.25), SWP_NOZORDER);
	ShowWindow(hwnd, SW_SHOW);
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	pyro::core::startup();

	if (!pyro::core::StartupSuccess)
		return -1;

	auto app = pyro::create_application();
	app->run();
	delete app;

	pyro::core::shutdown();
}

#else
	#error Pyro only supports Windows!
#endif