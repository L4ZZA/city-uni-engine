#include "pch.h"

#include "shader.h"

#include <regex>
#include <fstream>
#include "glad/glad.h"

namespace engine
{

	shader::shader(const char* filename)
	{
		std::string file;
		std::regex re("(?:#shader\\s+(vertex|fragment|pixel))\\s+([^]+?)(?=#endshader)");

		m_shader_sources = new std::unordered_map<unsigned int, std::string>();

		std::ifstream is(filename, std::ifstream::binary);
		if(is)
		{
			// Save filename
			m_filename = filename;

			is.seekg(0, std::ios::end);
			file.resize(is.tellg());
			is.seekg(0, std::ios::beg);

			is.read(&file[0], file.size());

			is.close();

			std::sregex_iterator next(file.begin(), file.end(), re, std::regex_constants::match_any);
			std::sregex_iterator ender;
			while(next != ender)
			{
				std::smatch match = *next;

				uint32 shaderType = type_from_string(match.str(1));
				if(shaderType)
					m_shader_sources->insert({ shaderType, match.str(2) });

				++next;
			}
		}
	}

	shader::~shader()
	{
		LOG_CORE_INFO("[shader] Destructing {}.", m_filename);
		if(m_program_loaded)
			glDeleteProgram(m_program_id);

		delete m_shader_sources;
	}

	void shader::compile_and_load()
	{
		GLint success;
		char infoLog[1024];

		std::vector<GLuint> shaderIds;

		// Set to default status, and false on error
		m_program_loaded = true;

		m_program_id = glCreateProgram();

		for(auto& kv : *(m_shader_sources))
		{
			GLint shaderId = glCreateShader(kv.first);
			const GLchar* source = kv.second.c_str();

			glShaderSource(shaderId, 1, &source, nullptr);
			glCompileShader(shaderId);

			// Error check
			glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);
				LOG_CORE_ERROR("[shader] Unable to compile shader {0}: {1}", m_filename, infoLog);
				glDeleteShader(shaderId);
			}
			else
			{
				glAttachShader(m_program_id, shaderId);
				shaderIds.push_back(shaderId);
			}
		}

		glLinkProgram(m_program_id);

		// Check if successful
		glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);
		if(!success)
		{
			glGetProgramInfoLog(m_program_id, 1024, nullptr, infoLog);
			LOG_CORE_WARN("[shader] Unable to link shader program {0}: {1}", m_filename, infoLog);
			glDeleteProgram(m_program_id);

			m_program_loaded = false;
			m_program_id = 0;
		}

		// Finally, clean up shader programs
		for(auto shaderId : shaderIds)
		{
			glDetachShader(m_program_id, shaderId);
			glDeleteShader(shaderId);
		}
	}

	void shader::bind() const
	{
		glUseProgram(m_program_id);
		LOG_RQ_TRACE("[shader] bound shader (prog {0})", m_program_id);
	}

	void shader::unbind() const
	{
		glUseProgram(0);
		LOG_RQ_TRACE("[shader] unbound shader (prog {0})", m_program_id);
	}

	void shader::set_uniform(const std::string& name, float val)
	{
		const int32 uniformLocation = glGetUniformLocation(m_program_id, name.c_str());
		glUniform1f(uniformLocation, val);

		LOG_RQ_TRACE("[shader] set_uniform (float) (prog {0}): uniform: '{1}' = {2}(vec3)", m_program_id, name, vec);
	}

	void shader::set_uniform(const std::string& name, const glm::vec3& vec)
	{
		const int32 uniformLocation = glGetUniformLocation(m_program_id, name.c_str());
		glUniform3f(uniformLocation, vec[0], vec[1], vec[2]);

		LOG_RQ_TRACE("[shader] set_uniform (glm::vec3) (prog {0}): uniform: '{1}' = {2}(vec3)", m_program_id, name, vec);
	}

	void shader::set_uniform(const std::string& name, const glm::vec4& vec)
	{
		const int32 uniformLocation = glGetUniformLocation(m_program_id, name.c_str());
		glUniform4f(uniformLocation, vec[0], vec[1], vec[2], vec[3]);

		LOG_RQ_TRACE("[shader] set_uniform (glm::vec4) (prog {0}): uniform: '{1}' = {2}(vec4)", m_program_id, name, vec);
	}

	void shader::set_uniform(const std::string& name, int32 val)
	{
		const int32 uniformLocation = glGetUniformLocation(m_program_id, name.c_str());
		glUniform1i(uniformLocation, val);

		LOG_RQ_TRACE("[shader] set_uniform (int32)  (prog {0}): uniform: '{1}' = {2}(int32)", m_program_id, name, val);
	}

	void shader::set_uniform(const std::string& name, const glm::mat4& mat)
	{
		const int32 uniformLocation = glGetUniformLocation(m_program_id, name.c_str());
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(mat));

		LOG_RQ_TRACE("[shader] set_uniform (glm::mat4) (prog {0}): uniform: '{1}' = {2}(mat4)", programId, name, mat);
	}

	uint32 shader::type_from_string(const std::string& shaderType)
	{
		if(shaderType == "vertex")
			return GL_VERTEX_SHADER;
		if(shaderType == "fragment" || shaderType == "pixel")
			return GL_FRAGMENT_SHADER;

		return 0;
	}

}
