#include "main_layer.h"


main_layer::main_layer()
	: layer("Main Layer"),
	m_light_pos(8.0f, 12.0f, 8.0f), m_light_diffuse(1.0f, 1.0f, 1.0f), m_light_ambient(0.5f, 0.5f, 0.5f), m_light_specular(1.0f, 1.0f, 1.0f), m_shininess(8.0f),
	m_model_shader("res/shaders/main_tex.glsl"), m_lamp_shader("res/shaders/lamp.glsl"),
	m_nanosuit("res/models/nanosuit/nanosuit.obj"), m_lamp("res/models/bulb/bulb.fbx"), 
	m_triangle(game_object(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), m_triangle_mesh, 1.0f, true, 1.0f, 0, glm::vec3(1.0f, 1.0f, 1.0f)))
{
	m_model_shader.compile_and_load();
	m_lamp_shader.compile_and_load();
	//bullet_manager * manager = new bullet_manager({triangle});
}

main_layer::~main_layer()
{}

void main_layer::on_update(double dt)
{}

void main_layer::on_render(engine::renderer& renderer)
{
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float) engine::application::instance().get_window().width() / (float) engine::application::instance().get_window().height(), 0.1f, 1000.0f);

	glm::mat4 modelModel = glm::mat4(1.0f);
	modelModel = glm::rotate(modelModel, glm::radians(45.0f * (float) engine::application::instance().get_timer()->total()), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 trianlgeModel = glm::mat4(1.0f);
	trianlgeModel = glm::rotate(trianlgeModel, glm::radians(-45.0f * (float)engine::application::instance().get_timer()->total()), glm::vec3(0.0f, 1.0f, 0.0f));
	trianlgeModel = glm::scale(trianlgeModel, { 10.0f, 10.0f, 10.0f });

	glm::mat4 viewModel = glm::mat4(1.0f);
	viewModel = glm::translate(viewModel, m_camera_pos);

	glm::mat4 modelLamp = glm::mat4(1.0f);
	modelLamp = glm::scale(modelLamp, {0.005f, 0.005f, 0.005f });
	glm::mat4 viewLamp = glm::mat4(1.0f);
	viewLamp = glm::translate(viewLamp, m_camera_pos);
	viewLamp = glm::translate(viewLamp, m_light_pos);

	m_model_shader.bind();
	// Set mvp uniforms
	m_model_shader.set_uniform("model", modelModel);
	m_model_shader.set_uniform("mvp", (projection * viewModel * modelModel));
	m_model_shader.set_uniform("viewPos", -m_camera_pos);

	// Set light uniforms
	m_model_shader.set_uniform("light.position", m_light_pos);
	m_model_shader.set_uniform("light.ambient", m_light_ambient);
	m_model_shader.set_uniform("light.diffuse", m_light_diffuse);
	m_model_shader.set_uniform("light.specular", m_light_specular);

	// material properties
	m_model_shader.set_uniform("material.shininess", m_shininess);

	// Render the model
	renderer.render_3d(m_nanosuit, m_model_shader);

	m_model_shader.bind();

	m_model_shader.set_uniform("model", trianlgeModel);
	m_model_shader.set_uniform("mvp", (projection * viewModel * trianlgeModel));

	renderer.render_3d(m_triangle.get_mesh(), m_model_shader);

	//lampShader.Bind();
	m_lamp_shader.bind();
	m_lamp_shader.set_uniform("mvp", (projection * viewLamp * modelLamp));
	m_lamp_shader.set_uniform("color", m_light_specular);
	// Render the lamp
	renderer.render_3d(m_lamp, m_lamp_shader);
}
