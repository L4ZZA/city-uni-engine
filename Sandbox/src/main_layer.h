#pragma once
#include "pyro.h"
#include "imgui.h"

class MainLayer : public pyro::imgui_layer
{
public:
	MainLayer();
	~MainLayer();

	void on_update(double dt) override;
	void on_imgui_render() override;

private:
	ImGuiIO* io;

	pyro::graphics::Shader modelShader;
	pyro::graphics::Shader lampShader;
	pyro::graphics::Model nanosuit;
	pyro::graphics::Model lamp;

	glm::vec3 lightPos;
	glm::vec3 lightDiffuse;
	glm::vec3 lightAmbient;
	glm::vec3 lightSpecular;
	float shininess;

	glm::vec3 cameraPos = glm::vec3(0.0f, -8.0f, -20.0f);
};


///// \brief
//class main_layer : public pyro::imgui_layer
//{
//public:
//    main_layer(const pyro::static_shader &shader);
//
//    void on_attach() override;
//    void on_detach() override;
//    void on_update(double dt) override;
//    void on_render(pyro::renderer &renderer) override;
//    void on_imgui_render() override;
//    void on_event(pyro::event& event) override;
//
//private:
//	//pyro::graphics::Model m_nanosuit{};
//    //pyro::raw_model         m_model;
//    //pyro::model_texture     m_texture{ 0 };
//    //pyro::textured_model    m_static_model{};
//    //pyro::static_shader     m_static_shader;
//    //pyro::entity            m_entity{};
//	//pyro::camera			m_camera{};
//	//pyro::light				m_light{};
//};
