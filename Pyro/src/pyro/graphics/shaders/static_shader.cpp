#include "pyro_pch.h"
#include "static_shader.h"

// Keep in mind these paths should be relative to the sandbox project, until I fix it.

const std::string pyro::static_shader::s_vertex_file = "res/shaders/vertex.shader";
const std::string pyro::static_shader::s_fragment_file = "res/shaders/fragment.shader";

pyro::static_shader::static_shader() = default;

void pyro::static_shader::create()
{
    shader_program::create(s_vertex_file, s_fragment_file);
}

void pyro::static_shader::bind_attributes()
{
    bind_attribute(0, "position");
}
