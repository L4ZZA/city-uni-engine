#include "pyro_pch.h"
#include "shader_program.h"
#include "glad/glad.h"
#include <fstream>
#include "pyro/utils/file_utils.h"

pyro::shader_program::shader_program(const std::string  &vertex_path, const std::string  &fragment_path)
{
    create(vertex_path, fragment_path);
}

void pyro::shader_program::create(const std::string &vertex_path, const std::string &fragment_path)
{
    m_vertex_shader_id = load_shader(vertex_path, GL_VERTEX_SHADER);
    check_compile_errors(m_vertex_shader_id, "VERTEX");
    m_fragment_shader_id = load_shader(fragment_path, GL_FRAGMENT_SHADER);
    check_compile_errors(m_fragment_shader_id, "FRAGMENT");

    m_program_id = glCreateProgram();
    glAttachShader(m_program_id, m_vertex_shader_id);
    glAttachShader(m_program_id, m_fragment_shader_id);

    bind_attributes();

    glLinkProgram(m_program_id);
    glValidateProgram(m_program_id);
    check_compile_errors(m_program_id, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDetachShader(m_program_id, m_vertex_shader_id);
    glDetachShader(m_program_id, m_fragment_shader_id);
    glDeleteShader(m_vertex_shader_id);
    glDeleteShader(m_fragment_shader_id);
}

void pyro::shader_program::cleanup() const
{
    stop();
    glDeleteProgram(m_program_id);
}

void pyro::shader_program::start() const
{
    glUseProgram(m_program_id);
}

void pyro::shader_program::stop() const
{
    glUseProgram(0);
}

void pyro::shader_program::bind_attribute(int attribute, const std::string &name)
{
    glBindAttribLocation(m_program_id, attribute, name.c_str());
}

void pyro::shader_program::set_uniforms(const std::string &name, const int *values, int count /*= 1*/)
{
    int location = get_uniform_location(name);
    glUniform1iv(location, count, values);
}

void pyro::shader_program::set_uniform(const std::string &name, int value)
{
    set_uniforms(name, &value, 1);
}

void pyro::shader_program::set_uniforms(const std::string &name, const float *values, int count /*= 1*/)
{
    int location = get_uniform_location(name);
    glUniform1fv(location, count, values);
}

void pyro::shader_program::set_uniform(const std::string &name, float value)
{
    set_uniforms(name, &value, 1);
}

void pyro::shader_program::set_uniforms(const std::string &name, const glm::vec2 *vectors, int count /*= 1*/)
{
    int location = get_uniform_location(name);
    glUniform2fv(location, count, (float*)vectors);
}

void pyro::shader_program::set_uniform(const std::string &name, const glm::vec2 &vector)
{
    set_uniforms(name, &vector, 1);
}

void pyro::shader_program::set_uniforms(const std::string &name, const glm::vec3 *vectors, int count /*= 1*/)
{
    int location = get_uniform_location(name);
    glUniform3fv(location, count, (float*)vectors);
}

void pyro::shader_program::set_uniform(const std::string &name, const glm::vec3 &vector)
{
    set_uniforms(name, &vector, 1);
}

void pyro::shader_program::set_uniforms(const std::string &name, const glm::vec4 *vectors, int count /*= 1*/)
{
    int location = get_uniform_location(name);
    glUniform4fv(location, count, (float*)vectors);
}

void pyro::shader_program::set_uniform(const std::string &name, const glm::vec4 &vector)
{
    set_uniforms(name, &vector, 1);
}

void pyro::shader_program::set_uniforms(const std::string &name, const glm::mat3 *matrices, int count /*= 1*/)
{
    int location = get_uniform_location(name);
    glUniformMatrix3fv(location, count, false, (float*)matrices);
}

void pyro::shader_program::set_uniform(const std::string &name, const glm::mat3 &matrix)
{
    set_uniforms(name, &matrix, 1);
}

void pyro::shader_program::set_uniforms(const std::string &name, const glm::mat4 *matrices, int count /*= 1*/)
{
    int location = get_uniform_location(name);
    glUniformMatrix4fv(location, count, false, (float*)matrices);
}

void pyro::shader_program::set_uniform(const std::string &name, const glm::mat4 &matrix)
{
    set_uniforms(name, &matrix, 1);
}

unsigned int pyro::shader_program::load_shader(const std::string &file_path, unsigned int type)
{
    std::string shader = read_file(file_path);
    if(shader.empty())
        PYRO_ERROR("Shader not found: {}", file_path);
    return compile_shader(shader, type);
}

unsigned int pyro::shader_program::compile_shader(const std::string &source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        std::string error = get_compilation_error(id);
        PYRO_CORE_ERROR("Failed to compile {0} shader:\n {1}.\n {2}", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), source, error);
        glDeleteShader(id);
        id = 0;
    }

    return id;
}

std::string pyro::shader_program::get_compilation_error(unsigned int id)
{
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char *message = (char*)alloca(length  *sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::string error(message);

    return error;
}

void pyro::shader_program::check_compile_errors(unsigned int shader, const std::string &type)
{
    int success;
    char infoLog[1024];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            PYRO_CORE_ERROR("ERROR::SHADER_COMPILATION_ERROR of type: {0}\n{1}\n -- --------------------------------------------------- -- ", type, infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            PYRO_CORE_ERROR("ERROR::PROGRAM_LINKING_ERROR of type: {0}\n{1}\n -- --------------------------------------------------- -- ", type, infoLog);
        }
    }
}

int pyro::shader_program::get_uniform_location(const std::string &name)
{
    // if uniform already exists
    if(m_uniforms_cache.find(name) != m_uniforms_cache.end())
        return m_uniforms_cache[name];

    // if doesn't exists, yet find it and cache it
    int location = glGetUniformLocation(m_program_id, name.c_str());
    if(location == -1)
        PYRO_CORE_WARN("Shader n_{} Uniform {} doesn't exist!", m_program_id, name);

    m_uniforms_cache[name] = location;
    return location;
}
