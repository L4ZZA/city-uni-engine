#include "pyro_pch.h"
#include "render_manager.h"

#include "glad/glad.h" // TODO: Move out to platform-specific renderer

#include "commands/general_commands.hpp"

namespace pyro 
{

	std::shared_ptr<render_manager> render_manager::m_instance;

	render_manager::render_manager()
		: m_command_queue(1 * 1024 * 1024 /* 1 MB */)
	{ }

	render_manager::~render_manager()
	{
		std::cout << "RenderManager Destructor" << std::endl;
	}

	std::shared_ptr<render_manager> render_manager::create()
	{
		PYRO_CORE_ASSERT(!m_instance, "Renderer already created!");
		m_instance = std::make_shared<render_manager>();
		return m_instance;
	}

	void render_manager::init()
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

	void render_manager::shutdown()
	{
		std::cout << "RenderManager::Shutdown" << std::endl;
	}

	void* render_manager::send_command(uint32 size)
	{
		return m_command_queue.Allocate(size);
	}

	void render_manager::render() const
	{
		m_instance->m_command_queue.Execute();
	}

	void render_manager::set_clear_color(float r, float g, float b) const
	{
		set_clear_color::dispatch(r, g, b);
	}

	void render_manager::clear_buffer() const
	{
		clear_buffer::dispatch();
	}

}
