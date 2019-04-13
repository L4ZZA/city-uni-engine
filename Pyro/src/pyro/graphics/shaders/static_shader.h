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
    protected:
        void bind_attributes() override;

    private:
        static const std::string s_vertex_file;
        static const std::string s_fragment_file;
    };
}
