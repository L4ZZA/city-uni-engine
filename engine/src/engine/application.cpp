#include "pch.h"
#include "application.h"
#include "graphics/renderer.h"
#include "utils/timer.h"

engine::application* engine::application::s_instance{ nullptr };

engine::application::application(const std::string &name, const window_props &properties)
	: m_title(name), m_properties(properties)
{
	LOG_ASSERT(!s_instance, "Application already exists!");
	s_instance = this;

	m_window = std::unique_ptr<window>(window::create(name, properties));
	m_window->event_callback(BIND_EVENT_FN(application::on_event));
}

engine::application::~application()
{

}

void engine::application::run()
{
	m_timer = new timer;
	m_timer->start();

	while(m_running)
	{
		const double dt = m_timer->elapsed();
		double millisecondsPerFrame = s_secondsPerFrame * 0.0001;

		if(dt > millisecondsPerFrame)
		{

			m_renderer.prepare();
			for(auto* layer : m_layers_stack)
			{
				layer->on_update(dt);
				layer->on_render(m_renderer);
			}
		}

		m_window->on_update();
	}
}

void engine::application::on_event(event& event)
{
	event_dispatcher dispatcher(event);
	dispatcher.dispatch<window_closed_event>(BIND_EVENT_FN(application::on_window_close));

	//LOG_CORE_TRACE("{0}", event);

	// events are executed from top of the stack to bottom (aka end to start of the list)
	for(auto it = m_layers_stack.end(); it != m_layers_stack.begin(); )
	{
		(*--it)->on_event(event);
		// stop event propagation to next layer if flagged as handled
		if(event.handled)
			break;
	}
}

void engine::application::push_layer(layer* layer)
{
	m_layers_stack.push_layer(layer);
	layer->on_attach();
}

void engine::application::push_overlay(layer* overlay)
{
	m_layers_stack.push_overlay(overlay);
	overlay->on_attach();
}

bool engine::application::on_window_close(window_closed_event& event)
{
	m_running = false;

	return true;
}
