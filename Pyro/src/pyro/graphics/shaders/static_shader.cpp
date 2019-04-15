#include "pyro_pch.h"
#include "static_shader.h"
#include "pyro/entities/light.h"

// Keep in mind these paths should be relative to the sandbox project, until I fix it.

const std::string pyro::static_shader::s_vertex_file = "res/shaders/vertex.shader";
const std::string pyro::static_shader::s_fragment_file = "res/shaders/fragment.shader";

pyro::static_shader::static_shader() = default;

void pyro::static_shader::create()
{
    shader_program::create(s_vertex_file, s_fragment_file);
}

void pyro::static_shader::load_transformation(const glm::mat4 &matrix)
{
    set_uniform("transformation_mat", matrix);
}

void pyro::static_shader::load_projection(const glm::mat4 &matrix)
{
    set_uniform("projection_mat", matrix);
}

void pyro::static_shader::load_view(const glm::mat4& matrix)
{
	set_uniform("view_mat", matrix);
}

void pyro::static_shader::load_light(const light& light)
{
	set_uniform("light_pos", light.position());
	set_uniform("light_colour", light.colour());
}

void pyro::static_shader::bind_attributes()
{
    bind_attribute(0, "position");
    bind_attribute(1, "tex_coords");
    bind_attribute(2, "normal");
}
