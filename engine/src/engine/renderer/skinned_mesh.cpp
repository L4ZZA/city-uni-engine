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
//#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "engine/utils/assimp_extensions.h"
#include "renderer.h"
#include "platform/opengl/gl_shader.h"

/*static const uint32_t s_MeshImportFlags =
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
    Shader = m_IsAnimated ? renderer::shaders_library()->get("animated_mesh") : renderer::shaders_library()->get("static_mesh");
    
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
				aiVector3D pos = mesh->mVertices[i];
				double posX = mesh->mVertices[i].x;
				double posY = mesh->mVertices[i].y;
				double posZ = mesh->mVertices[i].z;
				vertex.Position = glm::vec3((float)posX, (float)posX, (float)posX);

				if (abs(posX + posY + posZ) > 0.0f)
					int i = 0;

				glm::vec3 norm = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
				vertex.Normal = norm;

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

void engine::skinned_mesh::on_render(const glm::mat4& transform /*= glm::mat4(1.f))
{
	bool materialOverride = !m_textures.empty();
    if(materialOverride)
    {
        for(auto const& tex : m_textures)
        {
            tex->bind();
        }
    }

    for (Submesh& submesh : m_Submeshes)
	{
		if (m_IsAnimated)
		{
			for (size_t i = 0; i < m_BoneTransforms.size(); i++)
			{
				std::string uniformName = std::string("u_BoneTransforms[") + std::to_string(i) + std::string("]");
				std::static_pointer_cast<gl_shader>(m_MeshShader)->set_uniform(uniformName, m_BoneTransforms[i]);
			}
		}

		if (!materialOverride)
			std::static_pointer_cast<gl_shader>(m_MeshShader)->set_uniform("u_ModelMatrix", transform * submesh.Transform);
		glDrawElementsBaseVertex(GL_TRIANGLES, submesh.IndexCount, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * submesh.BaseIndex), submesh.BaseVertex);
	}
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
*/

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

void engine::skinned_mesh::VertexBoneData::AddBoneData(uint32_t BoneID, float Weight)
{
	for (uint32_t i = 0; i < ARRAY_SIZE_IN_ELEMENTS(IDs); i++) {
		if (Weights[i] == 0.0) {
			IDs[i] = BoneID;
			Weights[i] = Weight;
			return;
		}
	}

	// should never get here - more bones than we have space for
	assert(0);
}

engine::skinned_mesh::skinned_mesh()
{
	m_VAO = 0;
	ZERO_MEM(m_Buffers);
	m_NumBones = 0;
	m_current_animation_index = 0;
}


engine::skinned_mesh::~skinned_mesh()
{
	Clear();
}


void engine::skinned_mesh::Clear()
{
	if (m_Buffers[0] != 0) {
		glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
	}

	if (m_VAO != 0) {
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}


bool engine::skinned_mesh::LoadMesh(const std::string& Filename)
{
	// Release the previously loaded mesh (if it exists)
	Clear();

	// Create the VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	bool Ret = false;

	m_pScene = m_Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (&m_pScene) {
		m_GlobalInverseTransform = glm::inverse(Assimp::ToGlm(m_pScene->mRootNode->mTransformation));
		Ret = InitFromScene(m_pScene, Filename);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
	}

	// Make sure the VAO is not changed from the outside
	glBindVertexArray(0);

	return Ret;
}


bool engine::skinned_mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
{
	AddAnimations(pScene);
	m_Entries.resize(pScene->mNumMeshes);
	m_textures.resize(pScene->mNumMaterials);

	std::vector<glm::vec3> Positions;
	std::vector<glm::vec3> Normals;
	std::vector<glm::vec2> TexCoords;
	std::vector<VertexBoneData> Bones;
	std::vector<uint32_t> Indices;

	uint32_t NumVertices = 0;
	uint32_t NumIndices = 0;

	// Count the number of vertices and indices
	for (uint32_t i = 0; i < m_Entries.size(); i++) {
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
	for (uint32_t i = 0; i < m_Entries.size(); i++) {
		const aiMesh* paiMesh = pScene->mMeshes[i];
		InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
	}

	if (!InitMaterials(pScene, Filename)) {
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

	return glGetError() == GL_NO_ERROR;
}


void engine::skinned_mesh::InitMesh(uint32_t MeshIndex,
	const aiMesh* paiMesh,
	std::vector<glm::vec3>& Positions,
	std::vector<glm::vec3>& Normals,
	std::vector<glm::vec2>& TexCoords,
	std::vector<VertexBoneData>& Bones,
	std::vector<uint32_t>& Indices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (uint32_t i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		Positions.push_back(glm::vec3(pPos->x, pPos->y, pPos->z));
		Normals.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		TexCoords.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));
	}

	LoadBones(MeshIndex, paiMesh, Bones);

	// Populate the index buffer
	for (uint32_t i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& Face = paiMesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		Indices.push_back(Face.mIndices[0]);
		Indices.push_back(Face.mIndices[1]);
		Indices.push_back(Face.mIndices[2]);
	}
}


void engine::skinned_mesh::LoadBones(uint32_t MeshIndex, const aiMesh* pMesh, std::vector<VertexBoneData>& Bones)
{
	for (uint32_t i = 0; i < pMesh->mNumBones; i++) {
		uint32_t BoneIndex = 0;
		std::string BoneName(pMesh->mBones[i]->mName.data);

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
			// Allocate an index for a new bone
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
			m_BoneInfo[BoneIndex].BoneOffset = Assimp::ToGlm(pMesh->mBones[i]->mOffsetMatrix);
			m_BoneMapping[BoneName] = BoneIndex;
		}
		else {
			BoneIndex = m_BoneMapping[BoneName];
		}

		for (uint32_t j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
			uint32_t VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
			Bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}
}


bool engine::skinned_mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
{
	// Extract the directory part from the file name
	std::string::size_type SlashIndex = Filename.find_last_of("/");
	std::string Dir;

	if (SlashIndex == std::string::npos) {
		Dir = ".";
	}
	else if (SlashIndex == 0) {
		Dir = "/";
	}
	else {
		Dir = Filename.substr(0, SlashIndex);
	}

	bool Ret = true;

	// Initialize the materials
	for (uint32_t i = 0; i < pScene->mNumMaterials; i++) {
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		m_textures[i] = NULL;

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;

			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string p(Path.data);

				if (p.substr(0, 2) == ".\\") {
					p = p.substr(2, p.size() - 2);
				}

				std::string FullPath = Dir + "/" + p;

				m_textures[i] = texture_2d::create(FullPath.c_str());
			}
		}
	}

	return Ret;
}


void engine::skinned_mesh::on_render(const glm::mat4& transform /*= glm::mat4(1.f)*/, const ref<shader>& meshShader )
{
	glBindVertexArray(m_VAO);
	std::static_pointer_cast<gl_shader>(meshShader)->set_uniform("u_ModelMatrix", transform);
	for (size_t i = 0; i < m_BoneTransforms.size(); i++)
	{
		std::string uniformName = std::string("gBones[") + std::to_string(i) + std::string("]");
		std::static_pointer_cast<gl_shader>(meshShader)->set_uniform(uniformName, m_BoneTransforms[i]);
	}

	for (uint32_t i = 0; i < m_Entries.size(); i++) {
		const uint32_t MaterialIndex = m_Entries[i].MaterialIndex;

		assert(MaterialIndex < m_textures.size());

		if (m_textures[MaterialIndex]) {
			m_textures[MaterialIndex]->bind();
		}

		//if (!materialOverride)
			

		glDrawElementsBaseVertex(GL_TRIANGLES,
			m_Entries[i].NumIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(uint32_t) * m_Entries[i].BaseIndex),
			m_Entries[i].BaseVertex);
	}

	// Make sure the VAO is not changed from the outside    
	glBindVertexArray(0);
}








void engine::skinned_mesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
{
	std::string name(pNode->mName.data);
	const aiAnimation* animation = m_pAnimations[m_current_animation_index];
	glm::mat4 nodeTransform(Assimp::ToGlm(pNode->mTransformation));
	const aiNodeAnim* nodeAnim = FindNodeAnim(animation, name);

	if (nodeAnim)
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

	if (m_BoneMapping.find(name) != m_BoneMapping.end())
	{
		uint32_t BoneIndex = m_BoneMapping[name];
		m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * transform * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (uint32_t i = 0; i < pNode->mNumChildren; i++)
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], transform);
}

void engine::skinned_mesh::on_update(const timestep& ts)
{
	if (m_AnimationPlaying)
	{
		if (m_pAnimations[m_current_animation_index])
		{

			float TicksPerSecond = (float)(m_pAnimations[m_current_animation_index]->mTicksPerSecond != 0 ? m_pAnimations[m_current_animation_index]->mTicksPerSecond : 25.0f);// *m_TimeMultiplier;
			float TimeInTicks = ts * TicksPerSecond;
			float AnimationTime = fmod(TimeInTicks, (float)m_pAnimations[m_current_animation_index]->mDuration);

			BoneTransform(AnimationTime);
		}

		
	}
}

void engine::skinned_mesh::BoneTransform(float time)
{
	glm::mat4 Identity = glm::mat4(1.f);

	ReadNodeHeirarchy(time, m_pScene->mRootNode, Identity);

	m_BoneTransforms.resize(m_NumBones);

	for (uint32_t i = 0; i < m_NumBones; i++) {
		m_BoneTransforms[i] = m_BoneInfo[i].FinalTransformation;
	}
}

const aiNodeAnim* engine::skinned_mesh::FindNodeAnim(const aiAnimation* animation, const std::string& nodeName)
{
	for (uint32_t i = 0; i < animation->mNumChannels; i++)
	{
		const aiNodeAnim* nodeAnim = animation->mChannels[i];
		if (std::string(nodeAnim->mNodeName.data) == nodeName)
			return nodeAnim;
	}
	return nullptr;
}

uint32_t engine::skinned_mesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (uint32_t i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime)
			return i;
	}

	return 0;
}

uint32_t engine::skinned_mesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	CORE_ASSERT(pNodeAnim->mNumRotationKeys > 0);

	for (uint32_t i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime)
			return i;
	}

	return 0;
}

uint32_t engine::skinned_mesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	CORE_ASSERT(pNodeAnim->mNumScalingKeys > 0);

	for (uint32_t i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
	{
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime)
			return i;
	}

	return 0;
}

glm::vec3 engine::skinned_mesh::InterpolateTranslation(float animationTime, const aiNodeAnim* nodeAnim)
{
	if (nodeAnim->mNumPositionKeys == 1)
	{
		// No interpolation necessary for single value
		auto v = nodeAnim->mPositionKeys[0].mValue;
		return { v.x, v.y, v.z };
	}

	uint32_t PositionIndex = FindPosition(animationTime, nodeAnim);
	uint32_t NextPositionIndex = (PositionIndex + 1);
	CORE_ASSERT(NextPositionIndex < nodeAnim->mNumPositionKeys);
	ai_real DeltaTime = (float)(nodeAnim->mPositionKeys[NextPositionIndex].mTime - nodeAnim->mPositionKeys[PositionIndex].mTime);
	ai_real Factor = (animationTime - nodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	if (Factor < 0.0f)
		Factor = 0.0f;
	CORE_ASSERT(Factor <= 1.0f, "Factor must be below 1.0f");
	const aiVector3D& Start = nodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = nodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	aiVector3D v_factor = { Factor,Factor,Factor };
	aiVector3D aiVec = Start + Factor * Delta;
	return { aiVec.x, aiVec.y, aiVec.z };
}

glm::quat engine::skinned_mesh::InterpolateRotation(float animationTime, const aiNodeAnim* nodeAnim)
{
	if (nodeAnim->mNumRotationKeys == 1)
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
	if (Factor < 0.0f)
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
	if (nodeAnim->mNumScalingKeys == 1)
	{
		// No interpolation necessary for single value
		auto v = nodeAnim->mScalingKeys[0].mValue;
		return { v.x, v.y, v.z };
	}

	uint32_t index = FindScaling(animationTime, nodeAnim);
	uint32_t nextIndex = (index + 1);
	CORE_ASSERT(nextIndex < nodeAnim->mNumScalingKeys);
	ai_real deltaTime = (nodeAnim->mScalingKeys[nextIndex].mTime - nodeAnim->mScalingKeys[index].mTime);
	ai_real factor = (animationTime - nodeAnim->mScalingKeys[index].mTime) / deltaTime;
	if (factor < 0.0f)
		factor = 0.0f;
	CORE_ASSERT(factor <= 1.0f, "Factor must be below 1.0f");
	const auto& start = nodeAnim->mScalingKeys[index].mValue;
	const auto& end = nodeAnim->mScalingKeys[nextIndex].mValue;
	auto delta = end - start;
	auto aiVec = start + factor * delta;
	return { aiVec.x, aiVec.y, aiVec.z };
}

void engine::skinned_mesh::AddAnimations(const aiScene* pScene)
{
	uint32_t numAnimations = pScene->mNumAnimations;
	for (uint32_t i = 0; i != numAnimations; i++)
	{
		if (pScene->mAnimations[i])
		{
			m_pAnimations.push_back(pScene->mAnimations[i]);
		}
	}
}

void engine::skinned_mesh::LoadAnimationFile(const std::string& Filename)
{
	engine::ref<engine::Animation> animation = engine::Animation::create();
	animation->m_pScene = animation->m_aImporter.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	if (animation->m_pScene) {
		AddAnimations(animation->m_pScene);
	}
	else {
		printf("Error parsing '%s': '%s'\n", Filename.c_str(), animation->m_aImporter.GetErrorString());
	}
	m_extra_animations.push_back(animation);
}




engine::ref<engine::skinned_mesh> engine::skinned_mesh::create(const std::string& Filename)
{
	engine::ref<engine::skinned_mesh> mesh = std::make_shared<engine::skinned_mesh>();
	mesh->LoadMesh(Filename);
	return mesh;
}

engine::ref<engine::Animation> engine::Animation::create()
{
	engine::ref<engine::Animation> animation = std::make_shared<engine::Animation>();
	return animation;
}
