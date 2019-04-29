#pragma once
#include "core.h"
#include "application.h"

//=============================================================================
// Specifies the main entry point of the application regardless of the 
// platform in which the project will be built onto.
// The engine is supposed to handle the pain of creating it because of this.
//=============================================================================

#ifdef ENGINE_PLATFORM_WIN

/// \brief Externally declared in the executable project. 
extern engine::application* engine::create_application();

int main(int argc, char** argv)
{
#ifdef ENGINE_DEBUG
	auto hwnd = GetConsoleWindow();
	SetWindowPos(hwnd, HWND_TOP, 0, static_cast<int>(1080 * 0.72), 1920, static_cast<int>(1080 * 0.25), SWP_NOZORDER);
	ShowWindow(hwnd, SW_SHOW);
#else
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	engine::core::startup();

	if(!engine::core::startup_success)
		return -1;

	auto app = engine::create_application();
	app->run();
	delete app;

	engine::core::shutdown();
}

#else
#error Pyro only supports Windows!
#endif