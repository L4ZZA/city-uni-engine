#include "pyro_pch.h"
#include "core.h"
#include "logger.h"
//#include "graphics/render_manager.h"


bool pyro::core::StartupSuccess = true;

void pyro::core::startup()
{
	logger::init(spdlog::level::trace, spdlog::level::warn); // Start logging first

	PYRO_CORE_TRACE("=== Pyro Engine v" VERSION_STRING " Core startup ===============");

	platform_startup();

	// Create renderer
//    Graphics::RenderManager::Init();
}

void pyro::core::shutdown()
{
	platform_shutdown();

	PYRO_CORE_TRACE("=== Pyro Engine Core shutdown ======================");

	// Destroy renderer
//    Graphics::RenderManager::Shutdown();
}

pyro::core::core()
{}


pyro::core::~core()
{}
