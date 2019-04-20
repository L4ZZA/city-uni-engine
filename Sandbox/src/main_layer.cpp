﻿#include "main_layer.h"


//MainLayer::MainLayer()
//	: imgui_layer("Main Layer"), io(nullptr),
//	lightPos(8.0f, 12.0f, 8.0f), lightDiffuse(1.0f, 1.0f, 1.0f), lightAmbient(0.5f, 0.5f, 0.5f), lightSpecular(1.0f, 1.0f, 1.0f), shininess(8.0f),
//	modelShader("res/shaders/main_tex.glsl"), lampShader("res/shaders/lamp.glsl"),
//	nanosuit("res/models/nanosuit/nanosuit.obj"), lamp("res/models/lamp/lamp.obj")
//{
//	modelShader.compile_and_load();
//	lampShader.compile_and_load();
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
//	viewModel = glm::translate(viewModel, cameraPos);
//
//	glm::mat4 modelLamp = glm::mat4(1.0f);
//	modelLamp = glm::scale(modelLamp, glm::vec3(0.5f, 0.5f, 0.5f));
//	glm::mat4 viewLamp = glm::mat4(1.0f);
//	viewLamp = glm::translate(viewLamp, cameraPos);
//	viewLamp = glm::translate(viewLamp, lightPos);
//
//	//modelShader.Bind();
//
//	// Set mvp uniforms
//	modelShader.set_uniform("model", modelModel);
//	modelShader.set_uniform("mvp", (projection * viewModel * modelModel));
//	modelShader.set_uniform("viewPos", -cameraPos);
//
//	// Set light uniforms
//	modelShader.set_uniform("light.position", lightPos);
//	modelShader.set_uniform("light.ambient", lightAmbient);
//	modelShader.set_uniform("light.diffuse", lightDiffuse);
//	modelShader.set_uniform("light.specular", lightSpecular);
//
//	// material properties
//	modelShader.set_uniform("material.shininess", shininess);
//
//	// Render the model
//	nanosuit.Render(modelShader);
//
//	//lampShader.Bind();
//	lampShader.set_uniform("mvp", (projection * viewLamp * modelLamp));
//	lampShader.set_uniform("color", lightSpecular);
//	// Render the lamp
//	lamp.Render(lampShader);
//}
//
//void MainLayer::on_imgui_render()
//{
//	if (io) {
//		ImGui::Begin("Main Editor");
//
//		ImGui::Text("Light Properties");
//		ImGui::ColorEdit3("Specular Color", glm::value_ptr(lightSpecular));
//		ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(lightDiffuse));
//		ImGui::ColorEdit3("Ambient Color", glm::value_ptr(lightAmbient));
//
//		ImGui::SliderFloat3("Light Height", glm::value_ptr(lightPos), -20.0f, 20.0f);
//
//		ImGui::Text("Material Properties");
//		ImGui::SliderFloat("Shininess", &shininess, 0.0f, 128.0f);
//
//		ImGui::End();
//	}
//}

//// rect data (for future reuse)
//std::vector<float> vertices
//{
//	-0.5f,  0.5f, 0.f,//v0
//	-0.5f, -0.5f, 0.f,//v1
//	 0.5f, -0.5f, 0.f,//v2
//	 0.5f,  0.5f, 0.f,//v3
//};
//std::vector<unsigned int> indices
//{
//	0,1,3,//top left triangle (v0, v1, v3)
//	3,1,2//bottom right triangle (v3, v1, v2)
//};
//std::vector<float> tex_coords
//{
//	0,0,
//	0,1,
//	1,1,
//	1,0,
//};
//
//// cube data (for future reuse)
//std::vector<float> vertices
//{
//	-0.5f,0.5f,-0.5f,
//	-0.5f,-0.5f,-0.5f,
//	0.5f,-0.5f,-0.5f,
//	0.5f,0.5f,-0.5f,
//
//	-0.5f,0.5f,0.5f,
//	-0.5f,-0.5f,0.5f,
//	0.5f,-0.5f,0.5f,
//	0.5f,0.5f,0.5f,
//
//	0.5f,0.5f,-0.5f,
//	0.5f,-0.5f,-0.5f,
//	0.5f,-0.5f,0.5f,
//	0.5f,0.5f,0.5f,
//
//	-0.5f,0.5f,-0.5f,
//	-0.5f,-0.5f,-0.5f,
//	-0.5f,-0.5f,0.5f,
//	-0.5f,0.5f,0.5f,
//
//	-0.5f,0.5f,0.5f,
//	-0.5f,0.5f,-0.5f,
//	0.5f,0.5f,-0.5f,
//	0.5f,0.5f,0.5f,
//
//	-0.5f,-0.5f,0.5f,
//	-0.5f,-0.5f,-0.5f,
//	0.5f,-0.5f,-0.5f,
//	0.5f,-0.5f,0.5f
//};
//
//std::vector<float> tex_coords
//{
//	0,0,
//	0,1,
//	1,1,
//	1,0,
//	0,0,
//	0,1,
//	1,1,
//	1,0,
//	0,0,
//	0,1,
//	1,1,
//	1,0,
//	0,0,
//	0,1,
//	1,1,
//	1,0,
//	0,0,
//	0,1,
//	1,1,
//	1,0,
//	0,0,
//	0,1,
//	1,1,
//	1,0
//};
//
//std::vector<unsigned int> indices
//{
//	0,1,3,
//	3,1,2,
//	4,5,7,
//	7,5,6,
//	8,9,11,
//	11,9,10,
//	12,13,15,
//	15,13,14,
//	16,17,19,
//	19,17,18,
//	20,21,23,
//	23,21,22
//
//};

main_layer::main_layer(const pyro::static_shader& shader)
	:imgui_layer("main_layer")
{
	m_static_shader = shader;
}

void main_layer::on_attach()
{
	// credits: https://www.dropbox.com/sh/gscenjij7ny3xz1/AADQ3u6Ve96PP3ajyJP81ETSa?dl=0
	m_model = pyro::obj_loader::load_obj("dragon");
	m_texture = pyro::loader::load_texture("res/textures/dragon.png");
	m_static_model.create(m_model, m_texture);
	m_entity.create(m_static_model, { 0,0,-25 });
	m_light.create({ 0,0,-20 }, { 1,1,1 });
}

void main_layer::on_detach()
{
	m_static_shader.cleanup();
	pyro::loader::cleanup();
}

void main_layer::on_update(double dt)
{
	float f_dt = static_cast<float>(dt);
	m_entity.increase_rotation({ 0.f, f_dt, 0.f });
	m_camera.move(dt);
}

void main_layer::on_render(pyro::renderer& renderer)
{
	m_static_shader.start();
	m_static_shader.load_view(pyro::maths::create_view_matrix(m_camera));
	m_static_shader.load_light(m_light);
	renderer.render(m_entity, m_static_shader);
	m_static_shader.stop();
}

void main_layer::on_imgui_render()
{
}

void main_layer::on_event(pyro::event& event)
{
}
