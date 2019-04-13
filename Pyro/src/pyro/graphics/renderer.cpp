#include "pyro_pch.h"
#include "renderer.h"
#include "glad/glad.h"

void pyro::renderer::prepare() const
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(1, 0, 0, 1);
}

void pyro::renderer::render(raw_model model) const
{
    glBindVertexArray(model.vao_id());
    glDrawElements(GL_TRIANGLES, model.vertex_count(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
