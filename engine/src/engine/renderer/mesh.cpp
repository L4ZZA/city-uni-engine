#include "pch.h"
#include "mesh.h"
#include "renderer.h"
#include "platform/opengl/gl_mesh.h"

engine::mesh* engine::mesh::create(
    const std::vector<vertex>& vertices, 
    const std::vector<uint32_t>& indices)
{
    switch (renderer::api())
    {
        case renderer_api::e_api::none: CORE_ASSERT(false, "[vertex_buffer] e_renderer_api::none currently not supported!"); return nullptr;
        case renderer_api::e_api::open_gl: return new gl_mesh(vertices, indices);
    }

    CORE_ASSERT(false, "[vertex_buffer] Unknown renderer api!");
    return nullptr;
}
