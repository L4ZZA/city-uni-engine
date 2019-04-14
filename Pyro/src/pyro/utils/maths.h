#pragma once
#include "pyro_pch.h"

namespace pyro
{
    namespace maths
    {
        static glm::mat4 create_transformation_matrix(
            const glm::vec3 &translation,
            const glm::vec3 &rotation,
            float scale)
        {
            glm::mat4 mat = glm::identity<glm::mat4>();
            mat = glm::translate(mat, translation);
            mat = glm::rotate(mat, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); 
            mat = glm::rotate(mat, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); 
            mat = glm::rotate(mat, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
            mat = glm::scale(mat, glm::vec3(scale));
            return mat;
        }
    }
}
