#pragma once


namespace pyro 
{

	class shader
	{
	public:
		shader(const char* filename);
		~shader();

		void compile_and_load();
		void bind();

		void set_uniform(const std::string& name, float val);
		void set_uniform(const std::string& name, const glm::vec3& vec);
		void set_uniform(const std::string& name, const glm::vec4& vec);
		void set_uniform(const std::string& name, int32 val);
		void set_uniform(const std::string& name, const glm::mat4& mat);

		bool is_loaded() const { return m_program_loaded; }

		std::string to_string() const { return m_filename; }

	private: // methods
		uint32 type_from_string(const std::string& shaderType);

	private: // fields
		std::unordered_map<uint32, std::string>* m_shaderSources;
		std::string m_filename;

		uint32 m_program_id;

		bool m_program_loaded = false;
	};

	inline std::ostream& operator<<(std::ostream& os, const shader& s)
	{
		return os << s.to_string();
	}

}
