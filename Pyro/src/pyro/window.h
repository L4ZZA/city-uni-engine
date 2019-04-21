#pragma once

#include "pyro/events/event.h"

namespace pyro
{
	struct window_props
	{
		unsigned int    width;
		unsigned int    height;
		bool			full_screen;
		bool			vsync;
	};

	/// \brief Interface representing a desktop system based window.
	class PYRO_API window
	{
	public:
		static window* create(const std::string& name, window_props const &props = window_props());

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