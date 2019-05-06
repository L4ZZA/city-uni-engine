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
};
