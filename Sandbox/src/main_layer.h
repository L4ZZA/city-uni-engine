#pragma once
#include "pyro/layer.h"
#include "pyro/graphics/renderer.h"
#include "pyro/graphics/shaders/static_shader.h"
#include "pyro/graphics/textures/model_texture.h"
#include "pyro/graphics/models/textured_model.h"
#include "pyro/entities/entity.h"
#include "pyro/events/key_event.h"

//glm::mat4 camera(float Translate, glm::vec2 const & Rotate)
//{
//    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
//    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
//    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
//    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
//    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
//    return Projection * View * Model;
//}

/// \brief
class main_layer : public pyro::layer
{
public:
    main_layer() = default;

    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render(pyro::renderer &renderer) override;
    void on_event(pyro::event& event) override;

private:
    pyro::raw_model         m_model;
    pyro::model_texture     m_texture{ 0 };
    pyro::textured_model    m_static_model{};
    pyro::static_shader     m_static_shader;
    pyro::entity            m_entity{};
};
