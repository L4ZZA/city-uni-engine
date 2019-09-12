#pragma once

#include "render_command.h"
#include "shader.h"
#include "model.h"
#include "camera.h"

namespace engine
{
    class renderer
    {
    public:
        static void begin_scene(camera& camera, const ref<shader>& shader);
        static void end_scene();

        static void submit(
            const ref<shader>& shader, 
            const ref<vertex_array>& vertex_array, 
            const glm::mat4& transform = glm::mat4(1.f));
        static void submit(
            const ref<shader>& shader, 
            const ref<mesh>& mesh,
            const glm::mat4& transform = glm::mat4(1.f));
        static void submit(
            const ref<shader>& shader, 
            const ref<game_object>& object,
            const glm::mat4& transform = glm::mat4(1.f));

        inline static renderer_api::e_api api() { return renderer_api::api(); }

    private:
        struct scene_data
        {
            glm::mat4 view_projection_matrix; 
            ref<shader> shader; 
        };

        static scene_data* s_scene_data;
    };
}
