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

#pragma once

#include "engine/core.h"
#include "engine/core/timestep.h"
#include "engine/renderer/texture.h"
#include "engine/renderer/shader.h"
#include <assimp/Importer.hpp>

#include <map>
#include <vector>
#include <assimp/scene.h>       // Output data structure
#include "vertex_array.h"

struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;
//namespace Assimp{ struct Importer; }
/*
namespace engine
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Binormal;
		glm::vec2 Texcoord;
	};

	struct AnimatedVertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Binormal;
		glm::vec2 Texcoord;

		uint32_t IDs[4] = { 0, 0,0, 0 };
		float Weights[4]{ 0.0f, 0.0f, 0.0f, 0.0f };

		void AddBoneData(uint32_t BoneID, float Weight)
		{
			for (size_t i = 0; i < 4; i++)
			{
				if (Weights[i] == 0.0)
				{
					IDs[i] = BoneID;
					Weights[i] = Weight;
					return;
				}
			}

			// TODO: Keep top weights
			LOG_CORE_WARN("Vertex has more than four bones/weights affecting it, extra data will be discarded (BoneID={0}, Weight={1})", BoneID, Weight);
		}
	};
    
	static const int NumAttributes = 5;
    
	struct Index
	{
        union
        {
            uint32_t vec[3];
            struct{ uint32_t V1, V2, V3; };
        };
	};
	static_assert(sizeof(Index) == 3 * sizeof(uint32_t));
    
	struct BoneInfo
	{
		glm::mat4 BoneOffset;
		glm::mat4 FinalTransformation;
	};
    
	struct VertexBoneData
	{
		uint32_t IDs[4];
		float Weights[4];

		VertexBoneData()
		{
			memset(IDs, 0, sizeof(IDs));
			memset(Weights, 0, sizeof(Weights));
		};

		void AddBoneData(uint32_t BoneID, float Weight)
		{
			for (size_t i = 0; i < 4; i++)
			{
				if (Weights[i] == 0.0)
				{
					IDs[i] = BoneID;
					Weights[i] = Weight;
					return;
				}
			}
			
			// should never get here - more bones than we have space for
			CORE_ASSERT(false, "[VertexBoneData] Too many bones!");
		}
	};
    
	class Submesh
	{
	public:
		uint32_t BaseVertex;
		uint32_t BaseIndex;
		uint32_t MaterialIndex;
		uint32_t IndexCount;

		glm::mat4 Transform;
	};

    class skinned_mesh
    {
    public:
        skinned_mesh(const std::string &filename);

        void on_update(const timestep &ts);
        void on_render(const glm::mat4 &transform = glm::mat4(1.f));

        void DumpVertexBuffer();
        ref<shader> GetMeshShader() { return m_MeshShader; }
        std::vector<ref<texture>> GetMaterials() { return m_textures; }
        const std::string& GetFilePath() const { return m_FilePath; }

    private:
        void BoneTransform(float time);
        void ReadNodeHierarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
        void TraverseNodes(aiNode* node, int level = 0);

		const aiNodeAnim* FindNodeAnim(const aiAnimation* animation, const std::string& nodeName);
		uint32_t FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
		uint32_t FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		uint32_t FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		glm::vec3 InterpolateTranslation(float animationTime, const aiNodeAnim* nodeAnim);
		glm::quat InterpolateRotation(float animationTime, const aiNodeAnim* nodeAnim);
		glm::vec3 InterpolateScale(float animationTime, const aiNodeAnim* nodeAnim);
        std::vector<ref<texture_2d>> load_textures(aiMaterial* mat, aiTextureType type, const std::string& type_name) const;
		
        
	private:
		std::vector<Submesh> m_Submeshes;
		
		std::unique_ptr<Assimp::Importer> m_Importer;

		glm::mat4 m_InverseTransform;

		uint32_t m_BoneCount = 0;
		std::vector<BoneInfo> m_BoneInfo;

		ref<vertex_array> m_VertexArray;

		std::vector<Vertex> m_StaticVertices;
		std::vector<AnimatedVertex> m_AnimatedVertices;
		std::vector<Index> m_Indices;
		std::unordered_map<std::string, uint32_t> m_BoneMapping;
		std::vector<glm::mat4> m_BoneTransforms;
		const aiScene* m_Scene;

		// Materials
		ref<shader>  m_MeshShader;
		//ref<texture> m_Material;
		std::vector<ref<texture>> m_textures;

		// Animation
		bool m_IsAnimated = false;
		float m_AnimationTime = 0.0f;
		float m_WorldTime = 0.0f;
		float m_TimeMultiplier = 1.0f;
		bool m_AnimationPlaying = true;

		std::string m_FilePath;
        std::string m_directory;
    };
}*/

namespace engine {

	struct Animation;

	class skinned_mesh
	{
	public:
		skinned_mesh();

		~skinned_mesh();

		void on_update(const timestep& ts);

		void on_render(const glm::mat4& transform, const ref<shader>& meshShader);

		static ref<skinned_mesh> create(const std::string& Filename);

		uint32_t NumBones() const
		{
			return m_NumBones;
		}

		void BoneTransform(float time);

		void switch_animation(uint32_t index) { m_current_animation_index = index; }

		void LoadAnimationFile(const std::string& Filename);

		const std::vector<aiAnimation*> animations() { return m_pAnimations; }

	private:
#define NUM_BONES_PER_VEREX 4
#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define SAFE_DELETE(p) if (p) { delete p; p = NULL; }
		static const uint32_t MAX_BONES = 100;

		

		struct BoneInfo
		{
			glm::mat4 BoneOffset;
			glm::mat4 FinalTransformation;

			BoneInfo()
			{
				BoneOffset = glm::mat4(0.f);
				FinalTransformation = glm::mat4(0.f);
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
				ZERO_MEM(IDs);
				ZERO_MEM(Weights);
			}

			void AddBoneData(uint32_t BoneID, float Weight);
		};

		

		void SetBoneTransform(uint32_t Index, const glm::mat4& Transform);

		bool LoadMesh(const std::string& Filename);

		glm::vec3 InterpolateScale(float AnimationTime, const aiNodeAnim* pNodeAnim);
		glm::quat InterpolateRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		glm::vec3 InterpolateTranslation(float AnimationTime, const aiNodeAnim* pNodeAnim);
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

		void AddAnimations(const aiScene* pScene);
		


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
		uint32_t m_Buffers[NUM_VBs];

		struct MeshEntry {
			MeshEntry()
			{
				NumIndices = 0;
				BaseVertex = 0;
				BaseIndex = 0;
				MaterialIndex = INVALID_MATERIAL;
			}

			unsigned int NumIndices;
			unsigned int BaseVertex;
			unsigned int BaseIndex;
			unsigned int MaterialIndex;
		};

		std::vector<MeshEntry> m_Entries;
		std::vector<ref<texture_2d>> m_textures;
		std::vector<glm::mat4> m_BoneTransforms;

		std::map<std::string, uint32_t> m_BoneMapping; // maps a bone name to its index
		uint32_t m_NumBones;
		std::vector<BoneInfo> m_BoneInfo;
		glm::mat4 m_GlobalInverseTransform;

		const aiScene* m_pScene;
		std::vector<engine::ref<engine::Animation>>		m_extra_animations;
		std::vector<aiAnimation*> m_pAnimations;
		uint32_t m_current_animation_index;
		Assimp::Importer m_Importer;

		bool m_AnimationPlaying = true;
	};

	struct Animation
	{
		Assimp::Importer m_aImporter;
		const aiScene* m_pScene;
		std::vector<aiAnimation*> m_pAnimations;


		Animation()
		{
			m_pScene = NULL;
		}
		static ref<Animation> create();
	};
}

