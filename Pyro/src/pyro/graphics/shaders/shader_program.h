#pragma once
#include "pyro_pch.h"

namespace pyro
{
    /// \brief Abstract class that represents a generic shader program,
    /// containing all the data and functions that each shader program 
    /// should have.
    class shader_program
    {
    public:
        shader_program() = default;
        shader_program(const std::string &vertex_path, const std::string &fragment_path);
        virtual ~shader_program() = default;

        /// \brief Creates a shader program with the given shaders.
        void create(const std::string &vertex_path, const std::string &fragment_path);
        /// \brief Deletes the shader program.
        void cleanup() const;
        /// \brief Binds the program in order to use it.
        void start() const;
        /// \brief Unbinds the program.
        void stop() const;

    protected:
        /// \brief 
        virtual void bind_attributes() = 0; 
        virtual void bind_attribute(int attribute, const std::string &name);

    private:
        static std::string parse_shader(const std::string &file_path);
        static unsigned int load_shader(const std::string &file_path, unsigned int type);
        static unsigned int compile_shader(const std::string &source, unsigned int type);
        static std::string get_compilation_error(unsigned int id);
        static void check_compile_errors(unsigned int shader, const std::string &type);

    protected:
        unsigned int m_program_id{ 0 };
        unsigned int m_vertex_shader_id{ 0 };
        unsigned int m_fragment_shader_id{ 0 };
    };
}
