#include "main_layer.h"
#include "pyro/key_codes.h"
#include "pyro/events/key_event.h"
#include "pyro/input.h"
#include "pyro/graphics/loader.h"

std::vector<float> vertices
{
    -0.5f,  0.5f, 0.f,//v0
    -0.5f, -0.5f, 0.f,//v1
     0.5f, -0.5f, 0.f,//v2
     0.5f,  0.5f, 0.f,//v3
};
std::vector<unsigned int> indices
{
    0,1,3,//top left triangle (v0, v1, v3)
    3,1,2//bottom right triangle (v3, v1, v2)
};
std::vector<float> tex_coords
{
    0,0,
    0,1,
    1,1,
    1,0,
};

void main_layer::on_attach()
{
    m_model = pyro::loader::load_model(vertices, tex_coords, indices);
    m_static_shader.create();
    m_texture = pyro::loader::load_texture("res/textures/face.png");
    m_textured_model.create(m_model, m_texture);
}

void main_layer::on_detach()
{
    m_static_shader.cleanup();
    pyro::loader::cleanup();
}

void main_layer::on_update()
{
    if(pyro::input::key_pressed(pyro::key_codes::KEY_TAB))
        PYRO_INFO("Tab pressed (poll)");
    //PYRO_INFO("ExampleLayer::on_update()");
}

void main_layer::on_render(const pyro::renderer& renderer)
{
    m_static_shader.start();
    renderer.render(m_textured_model);
    m_static_shader.stop();
}

void main_layer::on_event(pyro::event& p_event)
{
    if(p_event.event_type() == pyro::event_type_e::key_pressed)
    {
        auto& e = dynamic_cast<pyro::key_pressed_event&>(p_event);
        if(e.key_code() == pyro::key_codes::KEY_TAB)
            PYRO_TRACE("Tab pressed (event)");
        PYRO_TRACE("{0}", static_cast<char>(e.key_code()));
    }
}
