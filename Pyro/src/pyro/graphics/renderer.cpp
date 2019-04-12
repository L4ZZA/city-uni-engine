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
    glDrawArrays(GL_TRIANGLES, 0, model.vertex_count());
    glBindVertexArray(0);
}
