#include "pyro_pch.h"
#include "shader_program.h"
#include "glad/glad.h"
#include <fstream>

pyro::shader_program::shader_program(const std::string & vertex_path, const std::string & fragment_path)
{
    create(vertex_path, fragment_path);
}

void pyro::shader_program::create(const std::string& vertex_path, const std::string& fragment_path)
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

void pyro::shader_program::bind_attribute(int attribute, const std::string& name)
{
    glBindAttribLocation(m_program_id, attribute, name.c_str());
}

unsigned int pyro::shader_program::load_shader(const std::string& file_path, unsigned int type)
{
    std::string shader = parse_shader(file_path);
    if(shader.empty())
        PYRO_ERROR("Shader not found: {}", file_path);
    return compile_shader(shader, type);
}

std::string pyro::shader_program::parse_shader(const std::string& file_path)
{
    std::ifstream stream(file_path);

    std::string line;
    std::stringstream ss;
    while(getline(stream, line))
    {
        ss << line << "\n";
    }

    return ss.str();
}

unsigned int pyro::shader_program::compile_shader(const std::string& source, unsigned int type)
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
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);
    std::string error(message);

    return error;
}

void pyro::shader_program::check_compile_errors(unsigned int shader, const std::string& type)
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
