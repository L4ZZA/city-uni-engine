#include "pyro_pch.h"
#include "renderer.h"
#include "glad/glad.h"
#include "models/textured_model.h"

void pyro::renderer::prepare() const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 0, 0, 1);
}

void pyro::renderer::render(const textured_model &textured_model) const
{
    raw_model model = textured_model.model();
    glBindVertexArray(model.vao_id());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textured_model.texture().id());
    glDrawElements(GL_TRIANGLES, model.vertex_count(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
