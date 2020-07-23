#include "main_layer.h"
#include "platform/opengl/gl_shader.h"

#include "engine/events/key_event.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>


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

    auto static_mesh_shader = engine::renderer::shaders_library()->get("static_mesh");
    auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");

    std::dynamic_pointer_cast<engine::gl_shader>(static_mesh_shader)->bind();
    std::dynamic_pointer_cast<engine::gl_shader>(static_mesh_shader)->set_uniform("u_sampler", 0);

    // set color texture unit
    std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->bind();
    std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gColorMap", 0);

    m_directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_directionalLight.AmbientIntensity = 0.55f;
    m_directionalLight.DiffuseIntensity = 0.9f;
    m_directionalLight.Direction = glm::vec3(1.0f, 0.0, 0.0);

    std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gDirectionalLight.Base.Color", m_directionalLight.Color);
    std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gDirectionalLight.Base.AmbientIntensity", m_directionalLight.AmbientIntensity);
    std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gDirectionalLight.Direction", glm::normalize(m_directionalLight.Direction));
    std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gDirectionalLight.Base.DiffuseIntensity", m_directionalLight.DiffuseIntensity);
    std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gMatSpecularIntensity", 0.f);
    std::dynamic_pointer_cast<engine::gl_shader>(animated_mesh_shader)->set_uniform("gSpecularPower", 0.f);



    m_texture = engine::texture_2d::create("assets/textures/checkerboard.png");
    m_face_texture = engine::texture_2d::create("assets/textures/face.png");
    m_skinned_mesh = engine::skinned_mesh::create("assets/models/animated/mannequin/free3Dmodel.dae");
    m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/walking.dae");
    m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/idle.dae");
    m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/jump.dae");
    m_skinned_mesh->LoadAnimationFile("assets/models/animated/mannequin/standard_run.dae");

    //"assets/models/animated/mannequin/idle.dae";
    //engine::ref <engine::model> bob = engine::model::create("assets/models/animated/mannequin/free3Dmodel.obj");
    // skybox texture from http://www.vwall.it/wp-content/plugins/canvasio3dpro/inc/resource/cubeMaps/
    m_skybox = engine::skybox::create(50.f,
        std::vector<engine::ref<engine::texture_2d>>{ engine::texture_2d::create("assets/textures/skybox/SkyboxFront.bmp"),
        engine::texture_2d::create("assets/textures/skybox/SkyboxRight.bmp"),
        engine::texture_2d::create("assets/textures/skybox/SkyboxBack.bmp"),
        engine::texture_2d::create("assets/textures/skybox/SkyboxLeft.bmp"),
        engine::texture_2d::create("assets/textures/skybox/SkyboxTop.bmp"),
        engine::texture_2d::create("assets/textures/skybox/SkyboxBottom.bmp")});

    //// Moss texture based on this image available under CC - BY 2.0 by Robert Benner : http://www.flickr.com/photos/mullica/5750625959/in/photostream/
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
    m_cube = engine::game_object::create(cuboid_props);

    m_skinned_mesh->switch_animation(1);

    const aiNodeAnim *nodeAnim = m_skinned_mesh->animations().at(1)->mChannels[0];
    aiVector3D animStart = nodeAnim->mPositionKeys[0].mValue;
    aiVector3D animEnd = nodeAnim->mPositionKeys[nodeAnim->mNumPositionKeys - 1].mValue;
    m_anim_displacement = (float)(animEnd - animStart).Length();
}

example_layer::~example_layer()
{
}

void example_layer::on_update(const engine::timestep &time_step)
{
    m_3d_camera.on_update(time_step);

    m_running_time += time_step;

    m_skinned_mesh->on_update(m_running_time);

    m_anim_timer += time_step;
    if(m_anim_timer > m_skinned_mesh->animations().at(1)->mDuration) m_anim_timer -= m_skinned_mesh->animations().at(1)->mDuration;
}

void example_layer::on_render()
{
    engine::render_command::clear_color({ 0.2f, 0.3f, 0.3f, 1.0f });
    engine::render_command::clear();

    const auto mesh_shader = engine::renderer::shaders_library()->get("static_mesh");
    engine::renderer::begin_scene(m_3d_camera, mesh_shader);

    // TODO - delete method and use -> engine::renderer::submit() instead
    glm::mat4 skybox_tranform(1.0f);
    skybox_tranform = glm::translate(skybox_tranform, m_3d_camera.position());
    engine::renderer::submit(mesh_shader, m_skybox, skybox_tranform);

    for(const auto &object : m_game_objects)
    {
        engine::renderer::submit(mesh_shader, object);
    }

    engine::renderer::end_scene();


    const auto animated_mesh_shader = engine::renderer::shaders_library()->get("animated_mesh");
    engine::renderer::begin_scene(m_3d_camera, animated_mesh_shader);

    glm::mat4 aniTransform = glm::mat4(1.0f);



    aniTransform = glm::translate(aniTransform, glm::vec3(0.0f, 1.0f, -5.0f - (m_anim_timer / m_skinned_mesh->animations().at(1)->mDuration) * (m_anim_displacement * 0.01f)));
    aniTransform = glm::scale(aniTransform, glm::vec3(0.01f));

    m_skinned_mesh->on_render(aniTransform, animated_mesh_shader);

    engine::renderer::end_scene();
}

void example_layer::on_event(engine::event &event)
{
    if(event.event_type() == engine::event_type_e::key_pressed)
    {
        auto &e = dynamic_cast<engine::key_pressed_event &>(event);
        if(e.key_code() == engine::key_codes::KEY_TAB)
        {
            engine::render_command::toggle_wireframe();
        }
        //PYRO_TRACE("{0}", static_cast<char>(e.key_code())); 
    }
}

