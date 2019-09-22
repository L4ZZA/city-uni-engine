#pragma once
/*

	Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "engine/core.h"
#include "engine/renderer/texture.h"

#include <map>
#include <vector>
#include <assimp/scene.h>       // Output data structure

namespace engine{
    class SkinnedMesh
    {
    public:
        SkinnedMesh();

        ~SkinnedMesh();

        bool LoadMesh(const std::string& Filename);

        void Render();
	    
        uint32_t NumBones() const
        {
            return m_NumBones;
        }
        
        void BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms);
        
    private:
        #define NUM_BONES_PER_VEREX 4

        struct BoneInfo
        {
            glm::mat4 BoneOffset;
            glm::mat4 FinalTransformation;        

            BoneInfo()
            {
                BoneOffset = glm::mat4(0);
                FinalTransformation = glm::mat4(0);
            }
        };
        
        struct VertexBoneData
        {        
            uint32_t IDs[NUM_BONES_PER_VEREX];
            float Weights[NUM_BONES_PER_VEREX];

            VertexBoneData()
            {
                Reset();
            };
            
            void Reset()
            {
                memset(IDs, 0, sizeof(uint32_t) * NUM_BONES_PER_VEREX);
                memset(Weights, 0, sizeof(float) * NUM_BONES_PER_VEREX);        
            }
            
            void AddBoneData(uint32_t BoneID, float Weight);
        };

        void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
        uint32_t FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
        uint32_t FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
        uint32_t FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
        const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName);
        void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
        bool InitFromScene(const aiScene* pScene, const std::string& Filename);
        void InitMesh(uint32_t MeshIndex,
                      const aiMesh* paiMesh,
                      std::vector<glm::vec3>& Positions,
                      std::vector<glm::vec3>& Normals,
                      std::vector<glm::vec2>& TexCoords,
                      std::vector<VertexBoneData>& Bones,
                      std::vector<unsigned int>& Indices);
        void LoadBones(uint32_t MeshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData>& Bones);
        bool InitMaterials(const aiScene* pScene, const std::string& Filename);
        void Clear();

    #define INVALID_MATERIAL 0xFFFFFFFF
      
    enum VB_TYPES {
        INDEX_BUFFER,
        POS_VB,
        NORMAL_VB,
        TEXCOORD_VB,
        BONE_VB,
        NUM_VBs            
    };

        uint32_t m_VAO;
        std::array<uint32_t, NUM_VBs> m_Buffers;

        struct MeshEntry {
            MeshEntry()
            {
                NumIndices    = 0;
                BaseVertex    = 0;
                BaseIndex     = 0;
                MaterialIndex = INVALID_MATERIAL;
            }
            
            unsigned int NumIndices;
            unsigned int BaseVertex;
            unsigned int BaseIndex;
            unsigned int MaterialIndex;
        };
        
        std::vector<MeshEntry> m_Entries;
        std::vector<ref<texture_2d>> m_Textures;
         
        std::map<std::string,uint32_t> m_BoneMapping; // maps a bone name to its index
        uint32_t m_NumBones;
        std::vector<BoneInfo> m_BoneInfo;
        glm::mat4 m_GlobalInverseTransform;
        
        const aiScene* m_pScene;
    };
}
