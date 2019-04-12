#include <pyro.h>
#include "imgui.h"
#include "pyro/events/key_event.h"
#include "game.h"
#include "main_layer.h"


pyro::application * pyro::create_application()
{
    return new sandbox_app();
}

sandbox_app::sandbox_app()
{
    push_layer(new main_layer());
}
