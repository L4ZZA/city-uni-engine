#pragma once
#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>

namespace Assimp
{
    /// \brief
    static glm::mat4 ToGlm(aiMatrix4x4 const &from)
    {
        glm::mat4 to;

        //the a,b,c,d in assimp is the row - the 1,2,3,4 is the column
        to[0][0] = static_cast<float>(from.a1); 
        to[0][1] = static_cast<float>(from.b1); 
        to[0][2] = static_cast<float>(from.c1); 
        to[0][3] = static_cast<float>(from.d1);

        to[1][0] = static_cast<float>(from.a2); 
        to[1][1] = static_cast<float>(from.b2); 
        to[1][2] = static_cast<float>(from.c2); 
        to[1][3] = static_cast<float>(from.d2);

        to[2][0] = static_cast<float>(from.a3); 
        to[2][1] = static_cast<float>(from.b3); 
        to[2][2] = static_cast<float>(from.c3); 
        to[2][3] = static_cast<float>(from.d3);

        to[3][0] = static_cast<float>(from.a4);
        to[3][1] = static_cast<float>(from.b4);
        to[3][2] = static_cast<float>(from.c4);
        to[3][3] = static_cast<float>(from.d4);

        return std::move(to);
    }
    
    /// \brief
    static glm::mat3 ToGlm(aiMatrix3x3 const &from)
    {
        glm::mat3 to;

        //the a,b,c,d in assimp is the row - the 1,2,3,4 is the column
        to[0][0] = static_cast<float>(from.a1); 
        to[0][1] = static_cast<float>(from.b1); 
        to[0][2] = static_cast<float>(from.c1);

        to[1][0] = static_cast<float>(from.a2); 
        to[1][1] = static_cast<float>(from.b2); 
        to[1][2] = static_cast<float>(from.c2);

        to[2][0] = static_cast<float>(from.a3);
        to[2][1] = static_cast<float>(from.b3);
        to[2][2] = static_cast<float>(from.c3);

        return std::move(to);
    }
}
