#pragma once
#include <engine.h>
#include "engine/utils/timer.h"

class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;
	void render_object(engine::ref<engine::game_object>, engine::ref<engine::shader> shader);

private: 
    engine::ref<engine::shader>         m_color_shader{}; 
    engine::ref<engine::vertex_array>   m_triangle_va{}; 

    engine::ref<engine::shader>         m_flat_color_shader{}; 
    engine::ref<engine::shader>         m_textured_shader{};
    engine::ref<engine::vertex_array>   m_rect_va{}; 
    engine::ref<engine::vertex_array>   m_cube_va{}; 
    engine::ref<engine::texture_2d>     m_texture{}; 
    engine::ref<engine::texture_2d>     m_face_texture{};
	engine::ref<engine::texture_2d>     m_terrain_texture{};
	engine::ref<engine::texture_2d>     m_skybox_texture{};
	engine::ref<engine::skybox>			m_skybox{};
	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::bullet_manager* m_manager;

    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera; 

    glm::vec3                       m_rect_color{.2f, .3f, .6f}; 
    glm::vec3                       m_rect_pos{0}; 
    const float                     m_rect_speed{1.f}; 
};
