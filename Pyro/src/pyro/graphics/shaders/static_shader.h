#pragma once
#include "shader_program.h"

namespace pyro
{
    /// \brief Used to create all the static (don't move) models in the game.
    class static_shader : public pyro::shader_program
    {
    public:
        static_shader();
        void create();
        void load_transformation(const glm::mat4 &matrix);
        void load_projection(const glm::mat4 &matrix);
        void load_view(const glm::mat4 &matrix);

    protected:
        void bind_attributes() override;
        
    private:
        static const std::string s_vertex_file;
        static const std::string s_fragment_file;
    };
}
