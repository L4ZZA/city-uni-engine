#include <pyro.h>
#include "pyro/entry_point.h"
#include "imgui.h"
#include "main_layer.h"

//
//class MainLayer : public pyro::imgui_layer
//{
//public:
//	MainLayer();
//	~MainLayer();
//
//	void on_update(double dt) override;
//	void on_imgui_render() override;
//
//private:
//	ImGuiIO* io;
//
//	pyro::graphics::Shader modelShader;
//	pyro::graphics::Shader lampShader;
//	pyro::graphics::Model nanosuit;
//	pyro::graphics::Model lamp;
//
//	glm::vec3 lightPos;
//	glm::vec3 lightDiffuse;
//	glm::vec3 lightAmbient;
//	glm::vec3 lightSpecular;
//	float shininess;
//
//	glm::vec3 cameraPos = glm::vec3(0.0f, -8.0f, -20.0f);
//};
//
//MainLayer::MainLayer()
//	: imgui_layer("Main Layer"), io(nullptr),
//	lightPos(8.0f, 12.0f, 8.0f), lightDiffuse(1.0f, 1.0f, 1.0f), lightAmbient(0.5f, 0.5f, 0.5f), lightSpecular(1.0f, 1.0f, 1.0f), shininess(8.0f),
//	modelShader("res/shaders/main_tex.glsl"), lampShader("res/shaders/lamp.glsl"),
//	nanosuit("res/models/nanosuit/nanosuit.obj"), lamp("res/models/lamp/lamp.obj")
//{
//	this->modelShader.CompileAndLoad();
//	this->lampShader.CompileAndLoad();
//}
//
//MainLayer::~MainLayer()
//{ }
//
//void MainLayer::on_update(double dt)
//{
//	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)pyro::application::instance().get_window().width() / (float)pyro::application::instance().get_window().height(), 0.1f, 1000.0f);
//
//	glm::mat4 modelModel = glm::mat4(1.0f);
//	modelModel = glm::rotate(modelModel, glm::radians(45.0f * (float)pyro::application::instance().get_timer()->Total()), glm::vec3(0.0f, 1.0f, 0.0f));
//	glm::mat4 viewModel = glm::mat4(1.0f);
//	viewModel = glm::translate(viewModel, this->cameraPos);
//
//	glm::mat4 modelLamp = glm::mat4(1.0f);
//	modelLamp = glm::scale(modelLamp, glm::vec3(0.5f, 0.5f, 0.5f));
//	glm::mat4 viewLamp = glm::mat4(1.0f);
//	viewLamp = glm::translate(viewLamp, cameraPos);
//	viewLamp = glm::translate(viewLamp, lightPos);
//
//	//this->modelShader.Bind();
//
//	// Set mvp uniforms
//	this->modelShader.SetUniform("model", modelModel);
//	this->modelShader.SetUniform("mvp", (projection * viewModel * modelModel));
//	this->modelShader.SetUniform("viewPos", -this->cameraPos);
//
//	// Set light uniforms
//	this->modelShader.SetUniform("light.position", this->lightPos);
//	this->modelShader.SetUniform("light.ambient", this->lightAmbient);
//	this->modelShader.SetUniform("light.diffuse", this->lightDiffuse);
//	this->modelShader.SetUniform("light.specular", this->lightSpecular);
//
//	// material properties
//	this->modelShader.SetUniform("material.shininess", this->shininess);
//
//	// Render the model
//	this->nanosuit.Render(this->modelShader);
//
//	//this->lampShader.Bind();
//	this->lampShader.SetUniform("mvp", (projection * viewLamp * modelLamp));
//	this->lampShader.SetUniform("color", lightSpecular);
//	// Render the lamp
//	this->lamp.Render(lampShader);
//}
//
//void MainLayer::on_imgui_render()
//{
//	if (this->io) {
//		ImGui::Begin("Main Editor");
//
//		ImGui::Text("Light Properties");
//		ImGui::ColorEdit3("Specular Color", glm::value_ptr(this->lightSpecular));
//		ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(this->lightDiffuse));
//		ImGui::ColorEdit3("Ambient Color", glm::value_ptr(this->lightAmbient));
//
//		ImGui::SliderFloat3("Light Height", glm::value_ptr(this->lightPos), -20.0f, 20.0f);
//
//		ImGui::Text("Material Properties");
//		ImGui::SliderFloat("Shininess", &this->shininess, 0.0f, 128.0f);
//
//		ImGui::End();
//	}
//}


class game : public pyro::application
{
public:
	game()
		:application("the game", { 1280, 720, false, false })
	{
		m_static_shader.create();
		m_renderer.init(m_static_shader);
		const auto layer = new main_layer();
		//const auto layer = new main_layer(m_static_shader);
		push_layer(layer);
	}
};

pyro::application * pyro::create_application()
{
	return new game();
}
