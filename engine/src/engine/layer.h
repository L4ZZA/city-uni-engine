#pragma once

#include "engine/events/event.h"

namespace engine
{
	class renderer;

	/// \brief Interface/base-class from which implement the layers in your applicaiton.
	class ENGINE_API layer
	{
	public:
		layer(const std::string &name = "Layer");
		virtual ~layer();

		/// \brief Runs when a layer is pushed onto the layer stack
		virtual void on_attach() {}
		/// \brief Runs when a layer is popped from the layer stack
		virtual void on_detach() {}
		/// \brief Runs when application calls the on_update method.
		/// Should happen every frame.
		virtual void on_update(double dt) {}
		/// \brief Render method.
		virtual void on_render(renderer &renderer) {}
		/// \brief Render method specific to imgui.
		virtual void on_imgui_render() {}
		/// \brief Runs whenever an event gets sent to the layer.
		virtual void on_event(event &event) {}

		/// \brief Gives you the name of the layer [for debug]
		const std::string& name() const { return m_debug_name; }

	protected:
		std::string m_debug_name;

		//TODO: enabling/disabling layer
	};
}
