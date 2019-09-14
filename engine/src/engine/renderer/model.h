#pragma once
#include "engine/entities/game_object.h"

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;

namespace engine
{
    class mesh;
    class texture_2d;

    /// \brief
    class model final : public game_object
    {
    public:
        model(const std::string& path, const bool is_static);
        ~model();

        // TODO - getters should always have a const specifier before the curly brackets since they don't modify any class member 
		glm::vec3 size() { return m_size; }


    private: // methods
        void process_node(aiNode* node, const aiScene* scene);
        ref<engine::mesh> process_mesh(aiMesh* mesh, const aiScene* scene);
        std::vector<ref<texture_2d>> load_textures(aiMaterial* mat, aiTextureType type, const std::string& type_name) const;

        // TODO - put comment in brief on what this function does since it doesn't only compare values, but also changes some other values. Update name accordingly.
        ///\brief 
		void min_max_compare(glm::vec3 point);

    private: // fields
        std::string m_directory;
        std::string m_path;

        // TODO - member variables need to be prefixed with "m_"
		glm::vec3 min_point;
		glm::vec3 max_point;
		bool first_point = true;
		glm::vec3 m_size;
    };
}
