#pragma once
#include <engine.h>

struct BaseLight
{
    glm::vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;

    BaseLight()
    {
        Color = glm::vec3(0.0f, 0.0f, 0.0f);
        AmbientIntensity = 0.0f;
        DiffuseIntensity = 0.0f;
    }
};

struct DirectionalLight : public BaseLight
{        
    glm::vec3 Direction;

    DirectionalLight()
    {
        Direction = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};

struct PointLight : public BaseLight
{
    glm::vec3 Position;

    struct
    {
        float Constant;
        float Linear;
        float Exp;
    } Attenuation;

    PointLight()
    {
        Position = glm::vec3(0.0f, 0.0f, 0.0f);
        Attenuation.Constant = 1.0f;
        Attenuation.Linear = 0.0f;
        Attenuation.Exp = 0.0f;
    }
};

struct SpotLight : public PointLight
{
    glm::vec3 Direction;
    float Cutoff;

    SpotLight()
    {
        Direction = glm::vec3(0.0f, 0.0f, 0.0f);
        Cutoff = 0.0f;
    }
};

class example_layer : public engine::layer
{
public:
    example_layer();
	~example_layer();

    void on_update(const engine::timestep& time_step) override;
    void on_render() override; 
    void on_event(engine::event& event) override;

private:
    engine::ref<engine::shader>         m_color_shader{}; 
    engine::ref<engine::vertex_array>   m_triangle_va{}; 

    engine::ref<engine::shader>         m_flat_color_shader{}; 
    //engine::ref<engine::shader>         m_textured_shader{};
    engine::ref<engine::vertex_array>   m_rect_va{}; 
    engine::ref<engine::vertex_array>   m_cube_va{}; 
    engine::ref<engine::texture_2d>     m_texture{}; 
    engine::ref<engine::texture_2d>     m_face_texture{};
	engine::ref<engine::texture_2d>     m_terrain_texture{};
	engine::ref<engine::texture_2d>     m_skybox_texture{};
	engine::ref<engine::skybox>			m_skybox{};
    engine::ref<engine::skinned_mesh>   m_skinned_mesh;
    DirectionalLight                    m_directionalLight;
    float                               m_running_time = 0.f;

	float								m_anim_timer = 0.0f;
	float								m_anim_displacement = 0.f;

	std::vector<engine::ref<engine::game_object>>     m_game_objects{};

	engine::ref<engine::game_object> m_cube{};

	engine::ref<engine::bullet_manager> m_manager;

    engine::orthographic_camera       m_2d_camera; 
    engine::perspective_camera        m_3d_camera; 

    glm::vec3                       m_rect_color{.2f, .3f, .6f}; 
    glm::vec3                       m_rect_pos{0}; 
    const float                     m_rect_speed{1.f}; 
};
