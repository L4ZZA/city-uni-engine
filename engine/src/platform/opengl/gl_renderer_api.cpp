#include "pch.h"
#include "gl_renderer_api.h"
#include "glad/glad.h"

enum class engine::e_primitive_type 
{ 
    points = GL_POINTS, 
    lines = GL_LINES, 
    line_loop = GL_LINE_LOOP, 
    line_strip = GL_LINE_STRIP, 
    triangles = GL_TRIANGLES, 
    triangle_strip = GL_TRIANGLE_STRIP, 
    triangle_fan = GL_TRIANGLE_FAN, 
};

engine::e_primitive_type engine::renderer_api::s_primitive_type = engine::e_primitive_type::triangles; 

void engine::gl_renderer_api::init()
{
    glEnable(GL_DEPTH_TEST);
    enable_alpha();
    enable_culling();
}

void engine::gl_renderer_api::enable_wireframe()
{
    glPolygonMode(GL_FRONT, GL_LINE); 
    glPolygonMode(GL_BACK, GL_LINE); 
}

void engine::gl_renderer_api::disable_wireframe()
{
    glPolygonMode(GL_FRONT, GL_FILL); 
    glPolygonMode(GL_BACK, GL_FILL); 
}

void engine::gl_renderer_api::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void engine::gl_renderer_api::clear_color(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void engine::gl_renderer_api::draw_indexed(const ref<vertex_array>& vertex_array)
{
    glDrawElements(GL_TRIANGLES, vertex_array->index_buffer()->count(), GL_UNSIGNED_INT, nullptr);
}

void engine::gl_renderer_api::draw_indexed(const ref<mesh>& mesh)
{
}

void engine::gl_renderer_api::primitive_type(const e_primitive_type& type)
{
    s_primitive_type = type; 
}

//------------------------------------------------------------------------------------------------- 

void engine::gl_renderer_api::enable_alpha()
{
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

void engine::gl_renderer_api::enable_culling()
{
    glEnable(GL_CULL_FACE); 
    glCullFace(GL_BACK); 
}
