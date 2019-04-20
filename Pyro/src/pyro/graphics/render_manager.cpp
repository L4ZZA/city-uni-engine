#include "pyro_pch.h"
#include "render_manager.h"

#include "glad/glad.h" // TODO: Move out to platform-specific renderer

#include "commands/general_commands.hpp"

namespace pyro::graphics 
{

	std::shared_ptr<RenderManager> RenderManager::instance;

	RenderManager::RenderManager()
		: commandQueue(1 * 1024 * 1024 /* 1 MB */)
	{ }

	RenderManager::~RenderManager()
	{
		std::cout << "RenderManager Destructor" << std::endl;
	}

	std::shared_ptr<RenderManager> RenderManager::Create()
	{
		PYRO_CORE_ASSERT(!instance, "Renderer already created!");
		instance = std::make_shared<RenderManager>();
		return instance;
	}

	void RenderManager::Init()
	{
		// TODO: Responsible for picking correct RenderApi
		glEnable(GL_MULTISAMPLE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void RenderManager::Shutdown()
	{
		std::cout << "RenderManager::Shutdown" << std::endl;
	}

	void* RenderManager::SubmitToQueue(uint32 size)
	{
		return this->commandQueue.Allocate(size);
	}

	void RenderManager::Render()
	{
		RenderManager::instance->commandQueue.Execute();
	}

	void RenderManager::SetClearColor(float r, float g, float b)
	{
		Command::SetClearColor::Dispatch(r, g, b);
	}

	void RenderManager::ClearBuffer()
	{
		Command::ClearBuffer::Dispatch();
	}

}
