#pragma once

#include "render_command.hpp"
#include "pyro/graphics/render_manager.h"

#include "pyro/defs.h"
#include "pyro/logger.h"

#include "glad/glad.h"

namespace pyro
{
	// --------------------------------------------------------------------------
	// -- SetClearColor                                                        --
	// --------------------------------------------------------------------------
	class set_clear_color : render_command
	{
	public:
		set_clear_color(float r, float g, float b)
			:m_r(r), m_g(g), m_b(b)
		{}

		void execute() override 
		{
			glClearColor(m_r, m_g, m_b, 1.0f);
			PYRO_RQ_TRACE("[RenderQueue] -- SetClearColor: <{0}, {1}, {2}>", r, g, b);
		}

	private:
		float m_r;
		float m_g;
		float m_b;

	public: //STATIC CREATOR
		static void dispatch(float r, float g, float b) 
		{
			auto mem = render_manager::get()->send_command(sizeof(set_clear_color));
			new (mem) set_clear_color(r, g, b);
		}
	};

	// --------------------------------------------------------------------------
	// -- ClearBuffer                                                          --
	// --------------------------------------------------------------------------
	class clear_buffer : render_command
	{
	public:
		clear_buffer()
		{}

		void execute() override 
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			PYRO_RQ_TRACE("[RenderQueue] -- ClearBuffer");
		}

	public: //STATIC CREATOR
		static void dispatch() 
		{
			auto mem = render_manager::get()->send_command(sizeof(clear_buffer));
			new (mem) clear_buffer();
		}
	};

}
