#pragma once
#include "engine.h"

class main_layer : public engine::layer
{
public:
	main_layer();
	~main_layer();

	void on_update(double dt) override;
	void on_render(engine::renderer &renderer) override;

private:
	engine::shader m_model_shader;
	engine::shader m_lamp_shader;
	engine::model m_nanosuit;
	// https://free3d.com/3d-model/b1-battledroid-full-rig-star-wars-485835.html
	//engine::model m_battledroid;
	// https://free3d.com/3d-model/simple-flashlight-70024.html
	engine::model m_lamp;

	glm::vec3 m_light_pos;
	glm::vec3 m_light_diffuse;
	glm::vec3 m_light_ambient;
	glm::vec3 m_light_specular;
	float m_shininess;

	glm::vec3 m_camera_pos = glm::vec3(0.0f, -8.0f, -20.0f);

	engine::mesh::vertex v1 = engine::mesh::vertex({ glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f), glm::vec2(0.0f, 0.0f) });
	engine::mesh::vertex v2 = engine::mesh::vertex({ glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f), glm::vec2(5.0f, 10.0f) });
	engine::mesh::vertex v3 = engine::mesh::vertex({ glm::vec3(5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f), glm::vec2(10.0f, 0.0f) });

	engine::mesh m_triangle_mesh = engine::mesh({ v1, v2, v3 }, { 0,1,2 }, { engine::texture("res/textures/dragon.png") });

	game_object * m_triangle;

	bullet_manager * manager;

	std::vector<game_object *> m_game_objects;


};
