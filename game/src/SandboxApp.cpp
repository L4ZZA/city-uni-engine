//#include <engine.h>
//
//#include "platform/opengl/gl_shader.h"
//
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//
//static const std::string textured_vertex_shader_3d = R"(  
//    #version 430  
//  
//    layout(location = 0) in vec3 a_position;  
//    layout(location = 1) in vec2 a_tex_coord;  
//  
//    uniform mat4 u_view_projection;  
//    uniform mat4 u_transform;  
//  
//    out vec2 v_tex_coord;   
//  
//    void main()  
//    {  
//        v_tex_coord = a_tex_coord;  
//        gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);  
//    }  
//)"; 
//
//static const std::string textured_fragment_shader_3d = R"(  
//    #version 430  
//  
//    layout(location = 0) out vec4 o_color;  
//  
//    in vec2 v_tex_coord;  
//  
//    uniform sampler2D u_sampler;   
//  
//    void main()  
//    {  
//        o_color = texture(u_sampler, v_tex_coord); 
//    }  
//)";
//
//class ExampleLayer : public engine::layer
//{
//public:
//    ExampleLayer()
//        : layer("Example"), m_camera_2d(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
//    {
//        m_vertex_array = engine::vertex_array::create();
//
//        float vertices[3 * 7]
//        {
//            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
//             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
//             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
//        };
//
//        auto vertex_buffer = engine::vertex_buffer::create(vertices, sizeof(vertices));
//        engine::buffer_layout layout =
//        {
//            { engine::e_shader_data_type::float3, "a_Position" },
//            { engine::e_shader_data_type::float4, "a_Color" }
//        };
//        vertex_buffer->layout(layout);
//        m_vertex_array->add_buffer(vertex_buffer);
//
//        uint32_t indices[3] = {0, 1, 2};
//        const auto index_buffer = engine::index_buffer::create(indices, sizeof(indices) / sizeof(uint32_t));
//        m_vertex_array->add_buffer(index_buffer);
//
//        m_square_va = engine::vertex_array::create();
//
//        float squareVertices[5 * 4] = {
//            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
//             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
//             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
//            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
//        };
//
//        const auto squareVB = engine::vertex_buffer::create(squareVertices, sizeof(squareVertices));
//        squareVB->layout({
//            { engine::e_shader_data_type::float3, "a_Position" },
//            { engine::e_shader_data_type::float2, "a_TexCoord" }
//        });
//        m_square_va->add_buffer(squareVB);
//
//        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
//        const auto squareIB = engine::index_buffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
//        m_square_va->add_buffer(squareIB);
//
//        std::string vertexSrc = R"(
//			#version 330 core
//			
//			layout(location = 0) in vec3 a_Position;
//			layout(location = 1) in vec4 a_Color;
//
//			uniform mat4 u_ViewProjection;
//			uniform mat4 u_Transform;
//
//			out vec3 v_Position;
//			out vec4 v_Color;
//
//			void main()
//			{
//				v_Position = a_Position;
//				v_Color = a_Color;
//				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
//			}
//		)";
//
//        std::string fragmentSrc = R"(
//			#version 330 core
//			
//			layout(location = 0) out vec4 color;
//
//			in vec3 v_Position;
//			in vec4 v_Color;
//
//			void main()
//			{
//				color = vec4(v_Position * 0.5 + 0.5, 1.0);
//				color = v_Color;
//			}
//		)";
//
//        m_shader = engine::shader::create("VertexPosColor", vertexSrc, fragmentSrc);
//
//        std::string flatColorShaderVertexSrc = R"(
//			#version 330 core
//			
//			layout(location = 0) in vec3 a_Position;
//
//			uniform mat4 u_ViewProjection;
//			uniform mat4 u_Transform;
//
//			out vec3 v_Position;
//
//			void main()
//			{
//				v_Position = a_Position;
//				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
//			}
//		)";
//
//        std::string flatColorShaderFragmentSrc = R"(
//			#version 330 core
//			
//			layout(location = 0) out vec4 color;
//
//			in vec3 v_Position;
//			
//			uniform vec3 u_Color;
//
//			void main()
//			{
//				color = vec4(u_Color, 1.0);
//			}
//		)";
//
//        m_flat_color_shader = engine::shader::create(/*"FlatColor",*/ flatColorShaderVertexSrc, flatColorShaderFragmentSrc);
//
//        auto textureShader = m_shader_library.Load("assets/shaders/Texture.glsl");
//
//        m_texture = engine::texture_2d::create("assets/textures/Checkerboard.png");
//        m_cherno_logo_texture = engine::texture_2d::create("assets/textures/ChernoLogo.png");
//
//        std::dynamic_pointer_cast<engine::gl_shader>(textureShader)->bind();
//        std::dynamic_pointer_cast<engine::gl_shader>(textureShader)->set_uniform("u_Texture", 0);
//    }
//
//    void on_update(const engine::timestep& ts) override
//    {
//        if(engine::input::key_pressed(engine::key_codes::KEY_LEFT))
//            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
//        else if(engine::input::key_pressed(engine::key_codes::KEY_RIGHT))
//            m_CameraPosition.x += m_CameraMoveSpeed * ts;
//
//        if(engine::input::key_pressed(engine::key_codes::KEY_UP))
//            m_CameraPosition.y += m_CameraMoveSpeed * ts;
//        else if(engine::input::key_pressed(engine::key_codes::KEY_DOWN))
//            m_CameraPosition.y -= m_CameraMoveSpeed * ts;
//
//        if(engine::input::key_pressed(engine::key_codes::KEY_A))
//            m_CameraRotation += m_CameraRotationSpeed * ts;
//        if(engine::input::key_pressed(engine::key_codes::KEY_D))
//            m_CameraRotation -= m_CameraRotationSpeed * ts;
//
//        engine::render_command::clear_color({0.1f, 0.1f, 0.1f, 1});
//        engine::render_command::clear();
//
//        m_camera_2d.position(m_CameraPosition);
//        m_camera_2d.rotation(m_CameraRotation);
//
//        engine::renderer::begin_scene(m_camera_2d);
//
//        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
//
//        std::dynamic_pointer_cast<engine::gl_shader>(m_flat_color_shader)->bind();
//        std::dynamic_pointer_cast<engine::gl_shader>(m_flat_color_shader)->set_uniform("u_Color", m_SquareColor);
//
//        for(int y = 0; y < 20; y++)
//        {
//            for(int x = 0; x < 20; x++)
//            {
//                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
//                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
//                engine::renderer::submit(m_flat_color_shader, m_square_va, transform);
//            }
//        }
//
//        auto textureShader = m_shader_library.get("Texture");
//
//        m_texture->bind();
//        engine::renderer::submit(textureShader, m_square_va, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
//        m_cherno_logo_texture->bind();
//        engine::renderer::submit(textureShader, m_square_va, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
//
//        // Triangle
//        // engine::Renderer::Submit(m_Shader, m_VertexArray);
//
//        engine::renderer::end_scene();
//    }
//
//    virtual void on_render() override
//    {
//    }
//
//    void on_event(engine::event& event) override
//    {
//    }
//
//private:
//    //engine::shader_library m_shader_library;
//    engine::ref<engine::shader> m_shader;
//    engine::ref<engine::vertex_array> m_vertex_array;
//
//    engine::ref<engine::shader> m_flat_color_shader;
//    engine::ref<engine::shader> m_texture_shader;
//    engine::ref<engine::vertex_array> m_square_va;
//
//    engine::ref<engine::texture_2d> m_texture;
//    engine::ref<engine::texture_2d> m_cherno_logo_texture;
//
//    engine::orthographic_camera m_camera_2d;
//
//    glm::vec3 m_CameraPosition;
//    float m_CameraMoveSpeed = 5.0f;
//
//    float m_CameraRotation = 0.0f;
//    float m_CameraRotationSpeed = 180.0f;
//
//    glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
//};
