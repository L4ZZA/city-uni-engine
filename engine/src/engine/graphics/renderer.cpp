﻿#include "pch.h"
#include "renderer.h"
#include "glad/glad.h"
#include "engine/entities/entity.h"
#include "models/raw_model.h"
#include "shaders/static_shader.h"
#include "engine/utils/maths.h"
#include "engine/application.h"

void engine::renderer::init(static_shader& shader)
{
	shader.start();
	auto width = static_cast<float>(application::instance().get_window().width());
	auto height = static_cast<float>(application::instance().get_window().height());
	shader.load_projection(maths::create_projection_matrix(s_fov, width, height, s_near, s_far));
	shader.stop();
}

void engine::renderer::prepare() const
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0f);
	glClearColor(0.7f, 0.7f, 0.9f, 1);
}

void engine::renderer::render(const entity &entity, static_shader &shader) const
{
	textured_model model = entity.model();
	auto raw_model = model.raw();
	auto mat = maths::create_transformation_matrix(entity.position(), entity.rotation(), entity.scale());
	shader.load_transformation(mat);

	glBindVertexArray(raw_model.vao_id());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.texture().id());
	glDrawElements(GL_TRIANGLES, raw_model.vertex_count(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}