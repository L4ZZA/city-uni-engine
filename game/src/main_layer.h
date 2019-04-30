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
