#pragma once
#include "pyro/graphics/models/textured_model.h"
#include "glm/vec3.hpp"

namespace pyro
{
    /// \brief Instance of a textured model
    class entity
    {
    public:
        entity() = default;
        entity(const textured_model &model,
            const glm::vec3 &pos = {0,0,0},
            const glm::vec3 &rotation = {0,0,0},
            float scale = 1);

        void create(const textured_model &model,
            const glm::vec3 &pos = {0,0,0},
			const glm::vec3 &rotation = {0,0,0},
            float scale = 1);

        void increase_translation(const glm::vec3 &vec);
        void increase_rotation(const glm::vec3 &vec);

        textured_model model() const;
        glm::vec3 position() const;
        glm::vec3 rotation() const;
        float scale() const;

    private:
        textured_model  m_model;
        glm::vec3       m_position{ 0 };
        glm::vec3       m_rotation{ 0 };
        float           m_scale{ 1 };
    };
}
