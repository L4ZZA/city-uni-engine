#include <pyro.h>
#include "pyro/entry_point.h"
#include "imgui.h"
#include "main_layer.h"


class game : public pyro::application
{
public:
	game()
		:application("The engine", { 1280, 720, false, false })
	{
		m_static_shader.create();
		m_renderer.init(m_static_shader);
		const auto layer = new main_layer();
		//const auto layer = new main_layer(m_static_shader);
		push_layer(layer);
	}
};

pyro::application * pyro::create_application()
{
	return new game();
}
