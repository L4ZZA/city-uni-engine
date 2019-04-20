#pragma once


namespace pyro::graphics 
{

	class Shader
	{
	public:
		Shader(const char* filename);
		~Shader();

		void CompileAndLoad();
		void Bind();

		void SetUniform(const std::string& name, float val);
		void SetUniform(const std::string& name, const glm::vec3& vec);
		void SetUniform(const std::string& name, const glm::vec4& vec);
		void SetUniform(const std::string& name, int32 val);
		void SetUniform(const std::string& name, const glm::mat4& mat);

		inline bool IsLoaded() { return this->programLoaded; }

		inline std::string ToString() const { return this->filename; }

	private: // methods
		uint32 getShaderTypeFromString(const std::string& shaderType);

	private: // fields
		std::unordered_map<uint32, std::string>* shaderSources;
		std::string filename;

		uint32 shaderProgramId;

		bool programLoaded;
	};

	inline std::ostream& operator<<(std::ostream& os, const Shader& s)
	{
		return os << s.ToString();
	}

}
