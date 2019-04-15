#include "pyro_pch.h"
#include "renderer.h"
#include "glad/glad.h"
#include "pyro/entities/entity.h"
#include "models/raw_model.h"
#include "shaders/static_shader.h"
#include "pyro/utils/maths.h"
#include "pyro/application.h"

void pyro::renderer::init(static_shader& shader)
{
	shader.start();
	auto width = static_cast<float>(application::instance().get_window().width());
	auto height = static_cast<float>(application::instance().get_window().height());
	shader.load_projection(maths::create_projection_matrix(s_fov, width, height, s_near, s_far));
	shader.stop();
}

void pyro::renderer::prepare() const
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0f);
	glClearColor(0, 0.0f, 0, 1);
}

void pyro::renderer::render(const entity &entity, static_shader &shader) const
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
