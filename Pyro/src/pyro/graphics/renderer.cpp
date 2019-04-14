#include "pyro_pch.h"
#include "renderer.h"
#include "glad/glad.h"
#include "pyro/entities/entity.h"
#include "models/raw_model.h"
#include "shaders/static_shader.h"
#include "pyro/utils/maths.h"

void pyro::renderer::prepare() const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 0, 0, 1);
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
