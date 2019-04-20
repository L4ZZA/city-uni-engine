#pragma once

#include "pyro/core/manager.h"

#include "render_command_queue.h"

namespace pyro
{

	class render_manager : public pyro::Manager
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		render_manager();
		virtual ~render_manager();

		static std::shared_ptr<render_manager> create();

		void init() override;
		void shutdown() override;

		void* send_command(uint32 size);
		void render() const;

		void set_clear_color(float r, float g, float b) const;
		void clear_buffer() const;

		static std::shared_ptr<render_manager> get() { return m_instance; }

	private:
		static std::shared_ptr <render_manager> m_instance;

		RenderCommandQueue m_command_queue;
	};

}
