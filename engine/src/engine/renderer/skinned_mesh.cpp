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

#include <glad/glad.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "engine/utils/assimp_extensions.h"
#include "renderer.h"

static const uint32_t s_MeshImportFlags =
aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
aiProcess_Triangulate |             // Make sure we're triangles
aiProcess_SortByPType |             // Split meshes by primitive type
aiProcess_GenNormals |              // Make sure we have legit normals
aiProcess_GenUVCoords |             // Convert UVs if required 
aiProcess_OptimizeMeshes |          // Batch draws where possible
aiProcess_ValidateDataStructure;    // Validation

struct LogStream : public Assimp::LogStream
{
    static void Initialize()
    {
        if(Assimp::DefaultLogger::isNullLogger())
        {
            Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
            Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
        }
    }

    virtual void write(const char* message) override
    {
        LOG_CORE_ERROR("[LogStream] Assimp error: {0}", message);
    }
};

static Assimp::Importer* importer;

engine::skinned_mesh::skinned_mesh(const std::string& filename)
    : m_FilePath(filename)
{
    LogStream::Initialize();
    LOG_CORE_INFO("Loading mesh: {0}", filename);
    m_Importer = std::make_unique<Assimp::Importer>();

    const aiScene* scene = m_Importer->ReadFile(filename, s_MeshImportFlags);
    if(!scene || !scene->HasMeshes())
        LOG_CORE_ERROR("Failed to load mesh file: {0}", filename);

    m_directory = m_FilePath.substr(0, m_FilePath.find_last_of('/') + 1);

    m_IsAnimated = scene->mAnimations != nullptr;
    m_MeshShader = m_IsAnimated ? renderer::shaders_library()->get("animated_mesh") : renderer::shaders_library()->get("static_mesh");
    
    m_InverseTransform = glm::inverse(Assimp::ToGlm(scene->mRootNode->mTransformation));



    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;

    m_Submeshes.reserve(scene->mNumMeshes);
    for(size_t m = 0; m < scene->mNumMeshes; m++)
    {
        aiMesh* mesh = scene->mMeshes[m];

        Submesh submesh;
        submesh.BaseVertex = vertexCount;
        submesh.BaseIndex = indexCount;
        submesh.MaterialIndex = mesh->mMaterialIndex;
        submesh.IndexCount = mesh->mNumFaces * 3;
        m_Submeshes.push_back(submesh);

        vertexCount += mesh->mNumVertices;
        indexCount += submesh.IndexCount;

        CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
        CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

        // Vertices
        if(m_IsAnimated)
        {
            for(size_t i = 0; i < mesh->mNumVertices; i++)
            {
                AnimatedVertex vertex;
                vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
                vertex.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

                if(mesh->HasTangentsAndBitangents())
                {
                    vertex.Tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
                    vertex.Binormal = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
                }

                if(mesh->HasTextureCoords(0))
                    vertex.Texcoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};

                m_AnimatedVertices.push_back(vertex);
            }
        }
        else
        {
            for(size_t i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
                vertex.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

                if(mesh->HasTangentsAndBitangents())
                {
                    vertex.Tangent = {mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z};
                    vertex.Binormal = {mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z};
                }

                if(mesh->HasTextureCoords(0))
                    vertex.Texcoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};

                m_StaticVertices.push_back(vertex);
            }
        }

        // Indices
        for(size_t i = 0; i < mesh->mNumFaces; i++)
        {
            CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
            m_Indices.push_back({mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2]});
        }

            
        // == Process materials/textures
        if(mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<ref<texture_2d>> diffuseMaps = load_textures(material, aiTextureType_DIFFUSE, "diffuse");
		    m_textures.insert(m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<ref<texture_2d>> specularMaps = load_textures(material, aiTextureType_SPECULAR, "specular");
		    m_textures.insert(m_textures.end(), specularMaps.begin(), specularMaps.end());
        }
    }


    LOG_CORE_TRACE("NODES:");
    LOG_CORE_TRACE("-----------------------------");
    TraverseNodes(scene->mRootNode);
    LOG_CORE_TRACE("-----------------------------");



    // Bones
    if(m_IsAnimated)
    {
        for(size_t m = 0; m < scene->mNumMeshes; m++)
        {
            aiMesh* mesh = scene->mMeshes[m];
            Submesh& submesh = m_Submeshes[m];

            for(size_t i = 0; i < mesh->mNumBones; i++)
            {
                aiBone* bone = mesh->mBones[i];
                std::string boneName(bone->mName.data);
                int boneIndex = 0;

                if(m_BoneMapping.find(boneName) == m_BoneMapping.end())
                {
                    // Allocate an index for a new bone
                    boneIndex = m_BoneCount;
                    m_BoneCount++;
                    BoneInfo bi;
                    m_BoneInfo.push_back(bi);
                    m_BoneInfo[boneIndex].BoneOffset = Assimp::ToGlm(bone->mOffsetMatrix);
                    m_BoneMapping[boneName] = boneIndex;
                }
                else
                {
                    LOG_CORE_TRACE("Found existing bone in map");
                    boneIndex = m_BoneMapping[boneName];
                }

                for(size_t j = 0; j < bone->mNumWeights; j++)
                {
                    int VertexID = submesh.BaseVertex + bone->mWeights[j].mVertexId;
                    float Weight = bone->mWeights[j].mWeight;
                    m_AnimatedVertices[VertexID].AddBoneData(boneIndex, Weight);
                }
            }
        }
    }

    m_VertexArray = vertex_array::create();
    if(m_IsAnimated)
    {
        auto vb = vertex_buffer::create(m_AnimatedVertices.data(), m_AnimatedVertices.size() * sizeof(AnimatedVertex));
        vb->layout({
            { e_shader_data_type::float3, "a_Position" },
            { e_shader_data_type::float3, "a_Normal" },
            { e_shader_data_type::float3, "a_Tangent" },
            { e_shader_data_type::float3, "a_Binormal" },
            { e_shader_data_type::float2, "a_TexCoord" },
            { e_shader_data_type::int4,   "a_BoneIDs" },
            { e_shader_data_type::float4, "a_BoneWeights" },
        });
        m_VertexArray->set_buffer(vb);
    }
    else
    {
        auto vb = vertex_buffer::create(m_StaticVertices.data(), m_StaticVertices.size() * sizeof(Vertex));
        vb->layout({
            { e_shader_data_type::float3, "a_Position" },
            { e_shader_data_type::float3, "a_Normal" },
            { e_shader_data_type::float3, "a_Tangent" },
            { e_shader_data_type::float3, "a_Binormal" },
            { e_shader_data_type::float2, "a_TexCoord" },
        });
        m_VertexArray->set_buffer(vb);
    }

    auto ib = index_buffer::create(m_Indices.data(), m_Indices.size() * sizeof(Index));
    m_VertexArray->set_buffer(ib);
    m_Scene = scene;
}

void engine::skinned_mesh::TraverseNodes(aiNode* node, int level)
{
    std::string levelText;
    for(int i = 0; i < level; i++)
        levelText += "-";
    LOG_CORE_TRACE("{0}Node name: {1}", levelText, std::string(node->mName.data));
    for(uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        uint32_t mesh = node->mMeshes[i];
        m_Submeshes[mesh].Transform = Assimp::ToGlm(node->mTransformation);
    }

    for(uint32_t i = 0; i < node->mNumChildren; i++)
    {
        aiNode* child = node->mChildren[i];
        TraverseNodes(child, level + 1);
    }
}

void engine::skinned_mesh::DumpVertexBuffer()
{
    // TODO: Convert to ImGui
    LOG_CORE_TRACE("------------------------------------------------------");
    LOG_CORE_TRACE("Vertex Buffer Dump");
    LOG_CORE_TRACE("Mesh: {0}", m_FilePath);
    if(m_IsAnimated)
    {
        for(size_t i = 0; i < m_AnimatedVertices.size(); i++)
        {
            auto& vertex = m_AnimatedVertices[i];
            LOG_CORE_TRACE("Vertex: {0}", i);
            LOG_CORE_TRACE("Position: {0}, {1}, {2}", vertex.Position.x, vertex.Position.y, vertex.Position.z);
            LOG_CORE_TRACE("Normal: {0}, {1}, {2}", vertex.Normal.x, vertex.Normal.y, vertex.Normal.z);
            LOG_CORE_TRACE("Binormal: {0}, {1}, {2}", vertex.Binormal.x, vertex.Binormal.y, vertex.Binormal.z);
            LOG_CORE_TRACE("Tangent: {0}, {1}, {2}", vertex.Tangent.x, vertex.Tangent.y, vertex.Tangent.z);
            LOG_CORE_TRACE("TexCoord: {0}, {1}", vertex.Texcoord.x, vertex.Texcoord.y);
            LOG_CORE_TRACE("--");
        }
    }
    else
    {
        for(size_t i = 0; i < m_StaticVertices.size(); i++)
        {
            auto& vertex = m_StaticVertices[i];
            LOG_CORE_TRACE("Vertex: {0}", i);
            LOG_CORE_TRACE("Position: {0}, {1}, {2}", vertex.Position.x, vertex.Position.y, vertex.Position.z);
            LOG_CORE_TRACE("Normal: {0}, {1}, {2}", vertex.Normal.x, vertex.Normal.y, vertex.Normal.z);
            LOG_CORE_TRACE("Binormal: {0}, {1}, {2}", vertex.Binormal.x, vertex.Binormal.y, vertex.Binormal.z);
            LOG_CORE_TRACE("Tangent: {0}, {1}, {2}", vertex.Tangent.x, vertex.Tangent.y, vertex.Tangent.z);
            LOG_CORE_TRACE("TexCoord: {0}, {1}", vertex.Texcoord.x, vertex.Texcoord.y);
            LOG_CORE_TRACE("--");
        }
    }
    LOG_CORE_TRACE("------------------------------------------------------");
}

void engine::skinned_mesh::BoneTransform(float time)
{
    ReadNodeHierarchy(time, m_Scene->mRootNode, glm::mat4(1.0f));
    m_BoneTransforms.resize(m_BoneCount);
    for(size_t i = 0; i < m_BoneCount; i++)
        m_BoneTransforms[i] = m_BoneInfo[i].FinalTransformation;
}

void engine::skinned_mesh::ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
{
    std::string name(pNode->mName.data);
    const aiAnimation* animation = m_Scene->mAnimations[0];
    glm::mat4 nodeTransform(Assimp::ToGlm(pNode->mTransformation));
    const aiNodeAnim* nodeAnim = FindNodeAnim(animation, name);

    if(nodeAnim)
    {
        glm::vec3 translation = InterpolateTranslation(AnimationTime, nodeAnim);
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));

        glm::quat rotation = InterpolateRotation(AnimationTime, nodeAnim);
        glm::mat4 rotationMatrix = glm::toMat4(rotation);

        glm::vec3 scale = InterpolateScale(AnimationTime, nodeAnim);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x, scale.y, scale.z));

        nodeTransform = translationMatrix * rotationMatrix * scaleMatrix;
    }

    glm::mat4 transform = ParentTransform * nodeTransform;

    if(m_BoneMapping.find(name) != m_BoneMapping.end())
    {
        uint32_t BoneIndex = m_BoneMapping[name];
        m_BoneInfo[BoneIndex].FinalTransformation = m_InverseTransform * transform * m_BoneInfo[BoneIndex].BoneOffset;
    }

    for(uint32_t i = 0; i < pNode->mNumChildren; i++)
        ReadNodeHierarchy(AnimationTime, pNode->mChildren[i], transform);
}

const aiNodeAnim* engine::skinned_mesh::FindNodeAnim(const aiAnimation* animation, const std::string& nodeName)
{
    for(uint32_t i = 0; i < animation->mNumChannels; i++)
    {
        const aiNodeAnim* nodeAnim = animation->mChannels[i];
        if(std::string(nodeAnim->mNodeName.data) == nodeName)
            return nodeAnim;
    }
    return nullptr;
}

uint32_t engine::skinned_mesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    for(uint32_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
    {
        if(AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
            return i;
    }

    return 0;
}

uint32_t engine::skinned_mesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    CORE_ASSERT(pNodeAnim->mNumRotationKeys > 0);

    for(uint32_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
    {
        if(AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
            return i;
    }

    return 0;
}

uint32_t engine::skinned_mesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
    CORE_ASSERT(pNodeAnim->mNumScalingKeys > 0);

    for(uint32_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
    {
        if(AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
            return i;
    }

    return 0;
}

glm::vec3 engine::skinned_mesh::InterpolateTranslation(float animationTime, const aiNodeAnim* nodeAnim)
{
    if(nodeAnim->mNumPositionKeys == 1)
    {
        // No interpolation necessary for single value
        auto v = nodeAnim->mPositionKeys[0].mValue;
        return {v.x, v.y, v.z};
    }

    uint32_t PositionIndex = FindPosition(animationTime, nodeAnim);
    uint32_t NextPositionIndex = (PositionIndex + 1);
    CORE_ASSERT(NextPositionIndex < nodeAnim->mNumPositionKeys);
    ai_real DeltaTime = (float)(nodeAnim->mPositionKeys[NextPositionIndex].mTime - nodeAnim->mPositionKeys[PositionIndex].mTime);
    ai_real Factor = (animationTime - nodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    if(Factor < 0.0f)
        Factor = 0.0f;
    CORE_ASSERT(Factor <= 1.0f, "Factor must be below 1.0f");
    const aiVector3D& Start = nodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = nodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    aiVector3D v_factor = {Factor,Factor,Factor};
    aiVector3D aiVec = Start + Factor * Delta;
    return {aiVec.x, aiVec.y, aiVec.z};
}

glm::quat engine::skinned_mesh::InterpolateRotation(float animationTime, const aiNodeAnim* nodeAnim)
{
    if(nodeAnim->mNumRotationKeys == 1)
    {
        // No interpolation necessary for single value
        auto v = nodeAnim->mRotationKeys[0].mValue;
        return glm::quat(v.w, v.x, v.y, v.z);
    }

    uint32_t RotationIndex = FindRotation(animationTime, nodeAnim);
    uint32_t NextRotationIndex = (RotationIndex + 1);
    CORE_ASSERT(NextRotationIndex < nodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(nodeAnim->mRotationKeys[NextRotationIndex].mTime - nodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (animationTime - (float)nodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    if(Factor < 0.0f)
        Factor = 0.0f;
    CORE_ASSERT(Factor <= 1.0f, "Factor must be below 1.0f");
    const aiQuaternion& StartRotationQ = nodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ = nodeAnim->mRotationKeys[NextRotationIndex].mValue;
    auto q = aiQuaternion();
    aiQuaternion::Interpolate(q, StartRotationQ, EndRotationQ, Factor);
    q = q.Normalize();
    return glm::quat(q.w, q.x, q.y, q.z);
}

glm::vec3 engine::skinned_mesh::InterpolateScale(float animationTime, const aiNodeAnim* nodeAnim)
{
    if(nodeAnim->mNumScalingKeys == 1)
    {
        // No interpolation necessary for single value
        auto v = nodeAnim->mScalingKeys[0].mValue;
        return {v.x, v.y, v.z};
    }

    uint32_t index = FindScaling(animationTime, nodeAnim);
    uint32_t nextIndex = (index + 1);
    CORE_ASSERT(nextIndex < nodeAnim->mNumScalingKeys);
    ai_real deltaTime = (nodeAnim->mScalingKeys[nextIndex].mTime - nodeAnim->mScalingKeys[index].mTime);
    ai_real factor = (animationTime - nodeAnim->mScalingKeys[index].mTime) / deltaTime;
    if(factor < 0.0f)
        factor = 0.0f;
    CORE_ASSERT(factor <= 1.0f, "Factor must be below 1.0f");
    const auto& start = nodeAnim->mScalingKeys[index].mValue;
    const auto& end = nodeAnim->mScalingKeys[nextIndex].mValue;
    auto delta = end - start;
    auto aiVec = start + factor * delta;
    return {aiVec.x, aiVec.y, aiVec.z};
}

std::vector<engine::ref<engine::texture_2d>> engine::skinned_mesh::load_textures(aiMaterial* mat, aiTextureType type, const std::string& type_name) const
{
    static std::vector<ref<texture_2d>> textures_loaded;
    std::vector<ref<texture_2d>> textures;
    for(uint32_t i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString filename;
        mat->GetTexture(type, i, &filename);

        bool skip = false;
        for(uint32_t j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j]->path().data(), filename.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if(!skip)
        {   // if texture hasn't been loaded already, load it
            const std::string full_path = m_directory + std::string(filename.C_Str());
            ref<texture_2d> texture2d = texture_2d::create(full_path);
            textures.push_back(texture2d);
            textures_loaded.push_back(texture2d); // add to loaded textures
        }
    }

    return textures;
}
