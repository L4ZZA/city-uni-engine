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


#include "pch.h"
#include "engine/core.h"
#include "skinned_mesh.h"
#include "glad/glad.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include "engine/utils/assimp_extensions.h"

#define GLCheckError() (glGetError() == GL_NO_ERROR)

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

static auto s_assimp_flags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;

void engine::SkinnedMesh::VertexBoneData::AddBoneData(uint32_t BoneID, float Weight)
{
    for(uint32_t i = 0; i < NUM_BONES_PER_VEREX; i++)
    {
        if(Weights[i] == 0.0)
        {
            IDs[i] = BoneID;
            Weights[i] = Weight;
            return;
        }
    }

    // should never get here - more bones than we have space for
    CORE_ASSERT(false, "[SkinnedMesh] Too many bones added.");
}

engine::SkinnedMesh::SkinnedMesh()
{
    LOG_CORE_INFO("[SkinnedMesh] Obect created.");
    m_VAO = 0;
    memset(m_Buffers.data(), 0, sizeof(uint32_t) * NUM_VBs);
    m_NumBones = 0;
    m_pScene = nullptr;
}


engine::SkinnedMesh::~SkinnedMesh()
{
    Clear();
}


void engine::SkinnedMesh::Clear()
{
    for(uint32_t i = 0; i < m_Textures.size(); i++)
    {
        m_Textures[i].reset();
    }

    if(m_Buffers[0] != 0)
    {
        glDeleteBuffers(m_Buffers.size(), m_Buffers.data());
    }

    if(m_VAO != 0)
    {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}


bool engine::SkinnedMesh::LoadMesh(const std::string& filename)
{
    // Release the previously loaded mesh (if it exists)
    Clear();

    LOG_CORE_INFO("[SkinnedMesh] Loading mesh: {}", filename);

    // Create the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create the buffers for the vertices attributes
    glGenBuffers(m_Buffers.size(), m_Buffers.data());

    bool Ret = false;

    auto importer = new Assimp::Importer();
    m_pScene = importer->ReadFile(filename.c_str(), s_assimp_flags);

    if(m_pScene)
    {
        m_GlobalInverseTransform = Assimp::ToGlm(m_pScene->mRootNode->mTransformation);
        m_GlobalInverseTransform = glm::inverse(m_GlobalInverseTransform);
        Ret = InitFromScene(m_pScene, filename);
    }
    else
    {
        LOG_CORE_ERROR("Error parsing '{}': '{}'\n", filename.c_str(), importer->GetErrorString());
    }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);
    delete importer;
    return Ret;
}


bool engine::SkinnedMesh::InitFromScene(const aiScene* pScene, const std::string& filename)
{
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    std::vector<glm::vec3> Positions;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec2> TexCoords;
    std::vector<VertexBoneData> Bones;
    std::vector<uint32_t> Indices;

    uint32_t NumVertices = 0;
    uint32_t NumIndices = 0;

    // Count the number of vertices and indices
    for(uint32_t i = 0; i < m_Entries.size(); i++)
    {
        m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
        m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex = NumVertices;
        m_Entries[i].BaseIndex = NumIndices;

        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices += m_Entries[i].NumIndices;
    }

    // Reserve space in the vectors for the vertex attributes and indices
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Bones.resize(NumVertices);
    Indices.reserve(NumIndices);

    // Initialize the meshes in the scene one by one
    for(uint32_t i = 0; i < m_Entries.size(); i++)
    {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
    }

    if(!InitMaterials(pScene, filename))
    {
        return false;
    }

    // Generate and populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

    return GLCheckError();
}


void engine::SkinnedMesh::InitMesh(uint32_t MeshIndex,
                                   const aiMesh* paiMesh,
                                   std::vector<glm::vec3>& Positions,
                                   std::vector<glm::vec3>& Normals,
                                   std::vector<glm::vec2>& TexCoords,
                                   std::vector<VertexBoneData>& Bones,
                                   std::vector<uint32_t>& Indices)
{
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for(uint32_t i = 0; i < paiMesh->mNumVertices; i++)
    {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
        Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
        TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
    }

    LoadBones(MeshIndex, paiMesh, Bones);

    // Populate the index buffer
    for(uint32_t i = 0; i < paiMesh->mNumFaces; i++)
    {
        const aiFace& Face = paiMesh->mFaces[i];
        CORE_ASSERT(Face.mNumIndices == 3, "[skinned_mesh] Each face should have 3 indices");
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
    }
}


void engine::SkinnedMesh::LoadBones(uint32_t MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
{
    for(uint32_t i = 0; i < pMesh->mNumBones; i++)
    {
        uint32_t BoneIndex = 0;
        std::string BoneName(pMesh->mBones[i]->mName.data);

        if(m_BoneMapping.find(BoneName) == m_BoneMapping.end())
        {
            // Allocate an index for a new bone
            BoneIndex = m_NumBones;
            m_NumBones++;
            BoneInfo bi;
            m_BoneInfo.push_back(bi);
            m_BoneInfo[BoneIndex].BoneOffset = Assimp::ToGlm(pMesh->mBones[i]->mOffsetMatrix);
            m_BoneMapping[BoneName] = BoneIndex;
        }
        else
        {
            BoneIndex = m_BoneMapping[BoneName];
        }

        for(uint32_t j = 0; j < pMesh->mBones[i]->mNumWeights; j++)
        {
            uint32_t VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
            Bones[VertexID].AddBoneData(BoneIndex, Weight);
        }
    }
}


bool engine::SkinnedMesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
    LOG_CORE_INFO("[SkinnedMesh] Loading materials.");
    
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;

    if(SlashIndex == std::string::npos)
    {
        Dir = ".";
    }
    else if(SlashIndex == 0)
    {
        Dir = "/";
    }
    else
    {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool ret = true;

    // Initialize the materials
    for(uint32_t i = 0; i < pScene->mNumMaterials; i++)
    {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = nullptr;

        if(pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
        {
            aiString Path;

            if(pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, nullptr, nullptr, nullptr, nullptr, nullptr) == AI_SUCCESS)
            {
                std::string p(Path.data);

                if(p.substr(0, 2) == ".\\")
                {
                    p = p.substr(2, p.size() - 2);
                }

               std:: string FullPath = Dir + "/" + p;

                m_Textures[i] = texture_2d::create(FullPath);

                //if(!m_Textures[i]->Load())
                //{
                //    printf("Error loading texture '%s'\n", FullPath.c_str());
                //    delete m_Textures[i];
                //    m_Textures[i] = nullptr;
                //    Ret = false;
                //}
                //else
                //{
                //    printf("%d - loaded texture '%s'\n", i, FullPath.c_str());
                //}
            }
        }
    }

    return ret;
}


void engine::SkinnedMesh::Render()
{
    glBindVertexArray(m_VAO);

    for(uint32_t i = 0; i < m_Entries.size(); i++)
    {
        const uint32_t MaterialIndex = m_Entries[i].MaterialIndex;

        CORE_ASSERT(MaterialIndex < m_Textures.size());

        if(m_Textures[MaterialIndex])
        {
            m_Textures[MaterialIndex]->bind(0);
        }

        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 m_Entries[i].NumIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(uint32_t) * m_Entries[i].BaseIndex),
                                 m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
}


uint32_t engine::SkinnedMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    for(uint32_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
    {
        if(AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
        {
            return i;
        }
    }

    CORE_ASSERT(false,"");

    return 0;
}


uint32_t engine::SkinnedMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    CORE_ASSERT(pNodeAnim->mNumRotationKeys > 0);

    for(uint32_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
    {
        if(AnimationTime < static_cast<float>(pNodeAnim->mRotationKeys[i + 1].mTime))
        {
            return i;
        }
    }

    CORE_ASSERT(false,"");

    return 0;
}


uint32_t engine::SkinnedMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    CORE_ASSERT(pNodeAnim->mNumScalingKeys > 0);

    for(uint32_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
    {
        if(AnimationTime < static_cast<float>(pNodeAnim->mScalingKeys[i + 1].mTime))
        {
            return i;
        }
    }

    CORE_ASSERT(false,"");

    return 0;
}


void engine::SkinnedMesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if(pNodeAnim->mNumPositionKeys == 1)
    {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    uint32_t PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    uint32_t NextPositionIndex = (PositionIndex + 1);
    CORE_ASSERT(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    const auto DeltaTime = static_cast<float>(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    double Factor = (AnimationTime - static_cast<double>(pNodeAnim->mPositionKeys[PositionIndex].mTime)) / DeltaTime;
    CORE_ASSERT(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}


void engine::SkinnedMesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    // we need at least two values to interpolate...
    if(pNodeAnim->mNumRotationKeys == 1)
    {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    uint32_t RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    uint32_t NextRotationIndex = (RotationIndex + 1);
    CORE_ASSERT(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    CORE_ASSERT(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
}


void engine::SkinnedMesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    if(pNodeAnim->mNumScalingKeys == 1)
    {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint32_t ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    uint32_t NextScalingIndex = (ScalingIndex + 1);
    CORE_ASSERT(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    double Factor = (AnimationTime - static_cast<double>(pNodeAnim->mScalingKeys[ScalingIndex].mTime)) / DeltaTime;
    CORE_ASSERT(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
}


void engine::SkinnedMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
{
    std::string NodeName(pNode->mName.data);

    const aiAnimation* pAnimation = m_pScene->mAnimations[0];

    glm::mat4 NodeTransformation = Assimp::ToGlm(pNode->mTransformation);

    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

    if(pNodeAnim)
    {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
        glm::mat4 ScalingM;
        ScalingM = glm::scale(ScalingM, {Scaling.x, Scaling.y, Scaling.z});

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
        glm::mat4 RotationM = Assimp::ToGlm(RotationQ.GetMatrix());

        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
        glm::mat4 TranslationM;
        TranslationM = glm::translate(TranslationM, {Translation.x, Translation.y, Translation.z});

        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
    }

    glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

    if(m_BoneMapping.find(NodeName) != m_BoneMapping.end())
    {
        uint32_t BoneIndex = m_BoneMapping[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
    }

    for(uint32_t i = 0; i < pNode->mNumChildren; i++)
    {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
    }
}


void engine::SkinnedMesh::BoneTransform(float TimeInSeconds, std::vector<glm::mat4>& Transforms)
{
    const glm::mat4 Identity(1);

    float TicksPerSecond = (float)(m_pScene->mAnimations[0]->mTicksPerSecond != 0 ? m_pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
    float TimeInTicks = TimeInSeconds * TicksPerSecond;
    float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[0]->mDuration);

    ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);

    Transforms.resize(m_NumBones);

    for(uint32_t i = 0; i < m_NumBones; i++)
    {
        Transforms[i] = m_BoneInfo[i].FinalTransformation;
    }
}


const aiNodeAnim* engine::SkinnedMesh::FindNodeAnim(const aiAnimation* pAnimation, const std::string& NodeName)
{
    for(uint32_t i = 0; i < pAnimation->mNumChannels; i++)
    {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if(std::string(pNodeAnim->mNodeName.data) == NodeName)
        {
            return pNodeAnim;
        }
    }

    return nullptr;
}
