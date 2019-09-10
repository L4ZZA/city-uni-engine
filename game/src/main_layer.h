#pragma once
#include <engine.h>

class example_layer : public engine::layer
{
public:
    example_layer();

    void on_update(const engine::timestep& timestep) override;
    void on_render() override; 
    void on_event(engine::event& event) override; 

private: 
    engine::ref<engine::shader>         m_color_shader{}; 
    engine::ref<engine::vertex_array>   m_triangle_va{}; 

    engine::ref<engine::shader>         m_flat_color_shader{}; 
    engine::ref<engine::shader>         m_textured_shader{}; 
    engine::ref<engine::vertex_array>   m_rect_va{}; 
    engine::ref<engine::vertex_array>   m_cube_va{}; 
    engine::ref<engine::texture_2d>     m_texture{}; 
    engine::ref<engine::texture_2d>     m_face_texture{}; 

    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera; 

    glm::vec3                       m_rect_color{.2f, .3f, .6f}; 
    glm::vec3                       m_rect_pos{0}; 
    const float                     m_rect_speed{1.f}; 
};
