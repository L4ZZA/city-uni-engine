#include "pch.h"
#include "core.h"
#include "logger.h"
//#include "graphics/render_manager.h"


bool engine::core::startup_success = true;

void engine::core::startup()
{
	logger::init(spdlog::level::trace, spdlog::level::warn); // Start logging first

	LOG_CORE_TRACE("=== Pyro Engine v" VERSION_STRING " Core startup ===============");

	platform_startup();

	// Create renderer
//    Graphics::RenderManager::Init();
}

void engine::core::shutdown()
{
	platform_shutdown();

	LOG_CORE_TRACE("=== Pyro Engine Core shutdown ======================");

	// Destroy renderer
//    Graphics::RenderManager::Shutdown();
}

engine::core::core()
{}


engine::core::~core()
{}
