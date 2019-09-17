#include "main_layer.h"
#include "platform/opengl/gl_shader.h"

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include "engine/events/key_event.h"


// TODO - move all shaders into shader library
static const std::string vertex_shader = R"(
    #version 430

    layout(location = 0) in vec3 a_position;
    layout(location = 1) in vec4 a_color;

    uniform mat4 u_view_projection;
    uniform mat4 u_transform;

    out vec3 v_position;
    out vec4 v_color;

    void main()
    {
        v_position = a_position;
        v_color = a_color;
        gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0);
    }
)";

static const std::string fragment_shader = R"(
    #version 430

    layout(location = 0) out vec4 o_color;

    in vec3 v_position;
    in vec4 v_color;

    void main()
    {
        o_color = v_color;
    }
)";

static const std::string flat_color_vertex_shader = R"(
    #version 430 
 
    layout(location = 0) in vec3 a_position; 
 
    uniform mat4 u_view_projection; 
    uniform mat4 u_transform; 
 
    out vec3 v_position; 
 
    void main() 
    { 
        v_position = a_position; 
        gl_Position = u_view_projection * u_transform * vec4(a_position, 1.0); 
    } 
)"; 

static const std::string flat_color_fragment_shader = R"( 
    #version 430 
 
    layout(location = 0) out vec4 o_color; 
 
    in vec3 v_position; 
     
    uniform vec3 u_color; 
 
    void main() 
    { 
        o_color = vec4(u_color, 1.f); 
    } 
)"; 

example_layer::example_layer() 
    :m_2d_camera(-1.6f, 1.6f, -0.9f, 0.9f), 
    m_3d_camera(engine::application::window().width(), engine::application::window().height()) 
{
    // hide the mouse and lock it inside the window
    //engine::input::anchor_mouse(true);
    engine::application::window().hide_mouse_cursor();

 //   //======= triangle =========
 //
 //   float vertices[3 * 7]
 //   {
 //       -.5f, -.5f, .0f,    .8f, .2f, .8f, 1.0f,
 //        .5f, -.5f, .0f,    .2f, .3f, .8f, 1.0f,
 //        .0f,  .5f, .0f,    .8f, .8f, .2f, 1.0f,
 //   };
 //
 //   engine::ref<engine::vertex_buffer> triangle_vb(engine::vertex_buffer::create(vertices, sizeof(vertices))); 
 //
 //   const engine::buffer_layout layout 
 //   { 
 //       {engine::e_shader_data_type::float3, "a_position"},  
 //       {engine::e_shader_data_type::float2, "a_tex_coord"},  
 //   }; 
 //   triangle_vb->layout(layout); 
 //
 //   uint32_t indices[3]{0,1,2}; 
 //   const engine::ref<engine::index_buffer> triangle_ib(engine::index_buffer::create(indices, sizeof(indices) / sizeof(uint32_t))); 
 //
 //   m_triangle_va = engine::vertex_array::create();
 //   m_triangle_va->add_buffer(triangle_vb); 
 //   m_triangle_va->add_buffer(triangle_ib); 
 //
 //   //======== rectangle ========= 
 //
 //   float rect_vertices[] 
 //   { 
 //       -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 
 //        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 
 //        0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 
 //       -0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 
 //   }; 
 //
 //   const engine::ref<engine::vertex_buffer> rect_vb(engine::vertex_buffer::create(rect_vertices, sizeof(rect_vertices))); 
 //
 //   uint32_t rect_indices[]{0,1,2, 2,3,0}; 
 //   const engine::ref<engine::index_buffer> rect_ib(engine::index_buffer::create(rect_indices, sizeof(rect_indices) / sizeof(uint32_t))); 
 //
 //   rect_vb->layout({ 
 //       {engine::e_shader_data_type::float3, "a_position"}, 
 //       {engine::e_shader_data_type::float2, "a_tex_coord"}, 
 //   }); 
 //
 //   m_rect_va = engine::vertex_array::create(); 
 //   m_rect_va->add_buffer(rect_vb); 
 //   m_rect_va->add_buffer(rect_ib); 
 //
 //   //======== cube ========= 
 //
 //   float cube_vertices[] 
 //   { 
 //       // FRONT 
 //       -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // A 
 //        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // B 
 //        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // C 
 //       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // D 
 //
 //       // BACK 
 //       -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // E 
 //        0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // F 
 //        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // G 
 //       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // H 
 //       
 //       // LEFT  
 //       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // E = 8 
 //       -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // H = 9 
 //       -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // D = 10 
 //       -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // A = 11 
 //       
 //       // RIGHT 
 //        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // F = 12 
 //        0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // G = 13 
 //        0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // C = 14 
 //        0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // B = 15 
 //       
 //       // BOTTOM 
 //       -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // E = 16 
 //        0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // F = 17 
 //        0.5f, -0.5f,  0.5f,  1.0f, 1.0f, // B = 18 
 //       -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, // A = 19 
 //       
 //       // TOP 
 //       -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, // G = 20 
 //        0.5f,  0.5f, -0.5f,  1.0f, 0.0f, // H = 21 
 //        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // D = 22 
 //       -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // C = 23 
 //   }; 
 //
 //   const engine::ref<engine::vertex_buffer> cube_vb(engine::vertex_buffer::create(cube_vertices, sizeof(cube_vertices))); 
 //
 //   uint32_t cube_indices[] 
 //   { 
 //       0, 1, 2,    2, 3, 0, // FRONT 
 //       5, 4, 7,    7, 6, 5, // BACK 
 //       8,11,10,   10, 9, 8, // LEFT 
 //       15,12,13,   13,14,15, // RIGHT 
 //       16,17,18,   18,19,16, // BOTTOM 
 //       21,20,23,   23,22,21  // TOP 
 //
 //   }; 
 //   const engine::ref<engine::index_buffer> cube_ib(engine::index_buffer::create(cube_indices, sizeof(cube_indices) / sizeof(uint32_t))); 
 //
 //   cube_vb->layout({ 
 //       {engine::e_shader_data_type::float3, "a_position"}, 
 //       {engine::e_shader_data_type::float2, "a_tex_coord"}, 
 //   }); 
 //
 //   m_cube_va = engine::vertex_array::create(); 
 //   m_cube_va->add_buffer(cube_vb); 
 //   m_cube_va->add_buffer(cube_ib);

    //TODO - move shaders to files and put them into shader library
    m_color_shader = engine::shader::create("vertex_color_shader", vertex_shader, fragment_shader);
    m_flat_color_shader = engine::shader::create("uniform_color_shader", flat_color_vertex_shader, flat_color_fragment_shader);
    auto mesh_shader = engine::renderer::shaders_library()->get("mesh_static");

    std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->bind();
    std::dynamic_pointer_cast<engine::gl_shader>(mesh_shader)->set_uniform("u_sampler", 0);

	m_texture = engine::texture_2d::create("assets/textures/checkerboard.png");
	m_face_texture = engine::texture_2d::create("assets/textures/face.png");

	// skybox texture from http://www.vwall.it/wp-content/plugins/canvasio3dpro/inc/resource/cubeMaps/
	m_skybox = engine::skybox::create(50.f,
		std::vector<engine::ref<engine::texture_2d>>{ engine::texture_2d::create("assets/textures/skybox.jpg") });

	// Moss texture based on this image available under CC - BY 2.0 by Robert Benner : http://www.flickr.com/photos/mullica/5750625959/in/photostream/
	std::vector<engine::ref<engine::texture_2d>> terrain_textures = { engine::texture_2d::create("assets/textures/moss2.png") };
	engine::ref<engine::terrain> terrain_shape = engine::terrain::create(100.f, 0.5f, 100.f);
	engine::game_object_properties terrain_props;
	terrain_props.meshes = { terrain_shape->mesh() };
	terrain_props.textures = terrain_textures;
	m_game_objects.push_back(engine::game_object::create(terrain_props));

	engine::ref<engine::cuboid> cuboid_shape = engine::cuboid::create(glm::vec3(0.5f), false);
	engine::game_object_properties cuboid_props;
	cuboid_props.position = { 0.f, 5.f, -5.f };
	cuboid_props.meshes = { cuboid_shape->mesh() };
	cuboid_props.textures = { m_texture };
	m_game_objects.push_back(engine::game_object::create(cuboid_props));

	// dragon texture from http://www.myfreetextures.com/four-dragon-scale-background-textures/
	engine::ref <engine::model> dragon_model = engine::model::create("assets/models/dragon.obj");
	engine::ref<engine::texture_2d> dragon_texture = engine::texture_2d::create("assets/textures/dragon.png");

	engine::game_object_properties dragon_props;
	//dragon_props.meshes = dragon_model->meshes();
	dragon_props.textures = { dragon_texture };
	dragon_props.scale = 1.f / dragon_model->size();
	//first dragon object
	dragon_props.position = { 2.f, 1.f, -2.f };
	m_game_objects.push_back(engine::game_object::create(dragon_props));

	//second dragon object
	dragon_props.position = { -2.f, 1.f, -2.f };
	m_game_objects.push_back(engine::game_object::create(dragon_props));

	m_manager = engine::bullet_manager::create(m_game_objects);
}

example_layer::~example_layer() {}

void example_layer::on_update(const engine::timestep& time_step) 
{
    m_3d_camera.on_update(time_step);

	m_game_objects.at(2)->turn_towards(glm::cross(m_game_objects.at(2)->position() -
		glm::vec3(m_3d_camera.position().x, m_game_objects.at(2)->position().y,
			m_3d_camera.position().z), glm::vec3(0.f,1.f,0.f)));

	m_game_objects.at(3)->turn_towards(glm::cross(m_game_objects.at(2)->position() -
		glm::vec3(m_3d_camera.position().x, m_game_objects.at(2)->position().y,
			m_3d_camera.position().z), glm::vec3(0.f, 1.f, 0.f)));

	//m_manager->dynamics_world_update(m_game_objects, timer.elapsed());

    /*if(engine::input::key_pressed(engine::key_codes::KEY_LEFT)) // left 
        m_rect_pos.x -= m_rect_speed * timestep; 
    else if(engine::input::key_pressed(engine::key_codes::KEY_RIGHT)) // right 
        m_rect_pos.x += m_rect_speed * timestep; 

    if(engine::input::key_pressed(engine::key_codes::KEY_DOWN)) // down 
        m_rect_pos.y -= m_rect_speed * timestep; 
    else if(engine::input::key_pressed(engine::key_codes::KEY_UP)) // up 
        m_rect_pos.y += m_rect_speed * timestep; */
} 

void example_layer::on_render() 
{
    engine::render_command::clear_color({0.2f, 0.3f, 0.3f, 1.0f}); 
    engine::render_command::clear();

    const auto textured_shader = engine::renderer::shaders_library()->get("mesh_static");
    engine::renderer::begin_scene(m_3d_camera, textured_shader); 

    /*std::vector<glm::vec3> cubePositions 
    { 
        glm::vec3(0.0f,  0.0f,  0.0f), 
        glm::vec3(2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f), 
        glm::vec3(-3.8f, -2.0f, -12.3f), 
        glm::vec3(2.4f, -0.4f, -3.5f), 
        glm::vec3(-1.7f,  3.0f, -7.5f), 
        glm::vec3(1.3f, -2.0f, -2.5f), 
        glm::vec3(1.5f,  2.0f, -2.5f), 
        glm::vec3(1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f) 
    };*/

    /*for(uint32_t i = 0; i < cubePositions.size(); i++) 
    { 
        glm::mat4 transform(1.0f); 
        transform = glm::translate(transform, cubePositions[i]); 
        float angle = 20.0f * i; 
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)); 
        //transform = glm::scale(transform, glm::vec3(1.5f)); 
        // big square 
        m_texture->bind(); 
        engine::renderer::submit(m_textured_shader, m_cube_va, transform); 
        m_face_texture->bind(); 
        engine::renderer::submit(m_textured_shader, m_cube_va, transform); 
    }*/

    // TODO - delete method and use -> engine::renderer::submit() instead
	glm::mat4 skybox_tranform(1.0f);
	skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
	for (const auto& texture : m_skybox->textures())
	{
		texture->bind();
	}
	engine::renderer::submit(textured_shader, m_skybox->mesh(), skybox_tranform);

	for (const auto& object : m_game_objects)
	{
		engine::renderer::submit(textured_shader, object);
	}

    engine::renderer::end_scene();

	//engine::renderer::begin_scene(m_2d_camera, m_flat_color_shader); 

    //static auto scale = glm::scale(glm::mat4(1), glm::vec3(0.1f)); 

    //std::dynamic_pointer_cast<engine::gl_shader>(m_flat_color_shader)->set_uniform("u_color", m_rect_color); 

    //for(int y = 0; y < 20; y++) 
    //    for(int x = 0; x < 20; x++) 
    //    { 
    //        glm::vec3 pos(x * 0.11f, y * 0.11f, 0); 
    //        auto transform = glm::translate(glm::mat4(1), m_rect_pos + pos) * scale; 
    //        engine::renderer::submit(m_flat_color_shader, m_rect_va, transform); 
    //    } 

    //// triangle 
    ////engine::renderer::submit(m_triangle_shader, m_vertex_array); 

    //engine::renderer::end_scene(); 
} 

void example_layer::on_event(engine::event& event) 
{ 
    if(event.event_type() == engine::event_type_e::key_pressed) 
    { 
        auto& e = dynamic_cast<engine::key_pressed_event&>(event); 
        if(e.key_code() == engine::key_codes::KEY_TAB) 
        { 
            engine::render_command::toggle_wireframe(); 
        } 
        //PYRO_TRACE("{0}", static_cast<char>(e.key_code())); 
    } 

}
