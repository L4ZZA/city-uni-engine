#pragma once

#include "pyro/events/event.h"

namespace pyro
{
	struct window_props
	{
		std::string     m_title;
		unsigned int    m_width;
		unsigned int    m_height;
		bool			m_vsync;

		window_props(
			std::string const &title = "pyro",
			unsigned int width = 1280,
			unsigned int height = 720,
			bool vsync = true)
			: m_title(title), m_width(width), m_height(height), m_vsync(vsync)
		{
		}
	};

	/// \brief Interface representing a desktop system based window.
	class PYRO_API window
	{
	public:
		static window* create(window_props const &props = window_props());

		//---------------------------------------------------------------------

		using event_callback_fn = std::function<void(event&)>;

		virtual ~window() = default;

		//---------------------------------------------------------------------

		virtual void on_update() = 0;

		//---------------------------------------------------------------------

		virtual void event_callback(event_callback_fn const &callback) = 0;
		virtual void vsync(bool enabled) = 0;
		virtual bool vsync() = 0;

		virtual void* native_window() const = 0;

		//---------------------------------------------------------------------

		virtual unsigned int width()  const = 0;
		virtual unsigned int height() const = 0;

	};
}