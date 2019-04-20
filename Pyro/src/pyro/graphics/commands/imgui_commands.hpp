#pragma once

#include "pyro/logger.h"
#include "pyro/graphics/render_manager.h"
#include "render_command.hpp"
#include "pyro/application.h"

namespace pyro
{

	class RenderImGUI : render_command
	{
	public:
		RenderImGUI(application* self)
			:self(self)
		{}

		void execute() override 
		{
			// Render ImGui interface
			self->m_imguiLayer->begin();

			for (auto* layer : self->m_layers_stack)
				layer->on_imgui_render();

			self->m_imguiLayer->end();

			PYRO_RQ_TRACE("[RenderQueue] -- RenderImGUI");
		}

	private:
		application* self;

	public: //STATIC CREATOR
		static void dispatch(application* self) 
		{
			auto mem = render_manager::get()->send_command(sizeof(RenderImGUI));
			new (mem) RenderImGUI(self);
		}
	};

}
