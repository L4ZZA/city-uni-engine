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
	engine::shader modelShader;
	engine::shader lampShader;
	engine::model nanosuit;
	engine::model lamp;

	glm::vec3 lightPos;
	glm::vec3 lightDiffuse;
	glm::vec3 lightAmbient;
	glm::vec3 lightSpecular;
	float shininess;

	glm::vec3 cameraPos = glm::vec3(0.0f, -8.0f, -20.0f);
};


/// \brief
//class main_layer : public engine::imgui_layer
//{
//public:
//    main_layer(const engine::static_shader &shader);
//
//    void on_attach() override;
//    void on_detach() override;
//    void on_update(double dt) override;
//    void on_render(engine::renderer &renderer) override;
//    void on_imgui_render() override;
//    void on_event(engine::event& event) override;
//
//private:
//	//engine::Model m_nanosuit{};
//    engine::raw_model         m_model;
//    engine::model_texture     m_texture{ 0 };
//    engine::textured_model    m_static_model{};
//    engine::static_shader     m_static_shader;
//    engine::entity            m_entity{};
//	engine::camera			m_camera{};
//	engine::light				m_light{};
//};
