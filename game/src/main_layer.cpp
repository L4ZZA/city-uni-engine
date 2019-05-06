#include "main_layer.h"


main_layer::main_layer()
	: layer("Main Layer"),
	lightPos(8.0f, 12.0f, 8.0f), lightDiffuse(1.0f, 1.0f, 1.0f), lightAmbient(0.5f, 0.5f, 0.5f), lightSpecular(1.0f, 1.0f, 1.0f), shininess(8.0f),
	modelShader("res/shaders/main_tex.glsl"), lampShader("res/shaders/lamp.glsl"),
	nanosuit("res/models/nanosuit/nanosuit.obj"), lamp("res/models/nanosuit/nanosuit.obj")
{
	modelShader.compile_and_load();
	lampShader.compile_and_load();
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
	glm::mat4 viewModel = glm::mat4(1.0f);
	viewModel = glm::translate(viewModel, cameraPos);

	glm::mat4 modelLamp = glm::mat4(1.0f);
	modelLamp = glm::scale(modelLamp, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 viewLamp = glm::mat4(1.0f);
	viewLamp = glm::translate(viewLamp, cameraPos);
	viewLamp = glm::translate(viewLamp, lightPos);

	//modelShader.Bind();

	// Set mvp uniforms
	modelShader.set_uniform("model", modelModel);
	modelShader.set_uniform("mvp", (projection * viewModel * modelModel));
	modelShader.set_uniform("viewPos", -cameraPos);

	// Set light uniforms
	modelShader.set_uniform("light.position", lightPos);
	modelShader.set_uniform("light.ambient", lightAmbient);
	modelShader.set_uniform("light.diffuse", lightDiffuse);
	modelShader.set_uniform("light.specular", lightSpecular);

	// material properties
	modelShader.set_uniform("material.shininess", shininess);

	// Render the model
	renderer.render_3d(nanosuit, modelShader);

	//lampShader.Bind();
	lampShader.set_uniform("mvp", (projection * viewLamp * modelLamp));
	lampShader.set_uniform("color", lightSpecular);
	// Render the lamp
	renderer.render_3d(lamp, lampShader);
}
