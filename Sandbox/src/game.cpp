#include <pyro.h>
#include "imgui.h"
#include "game.h"
#include "main_layer.h"


pyro::application * pyro::create_application()
{
    return new sandbox_app();
}

sandbox_app::sandbox_app()
{
    m_static_shader.create();
	m_renderer.init(m_static_shader);
	auto layer = new main_layer(m_static_shader);
    push_layer(layer);
}
