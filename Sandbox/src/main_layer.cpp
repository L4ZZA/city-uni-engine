#include "main_layer.h"
#include "pyro/key_codes.h"
#include "pyro/events/key_event.h"
#include "pyro/input.h"
#include "pyro/graphics/loader.h"
#include "pyro/utils/maths.h"
#include "pyro/graphics/obj_loader.h"

//std::vector<float> vertices
//{
//	-0.5f,  0.5f, 0.f,//v0
//	-0.5f, -0.5f, 0.f,//v1
//	 0.5f, -0.5f, 0.f,//v2
//	 0.5f,  0.5f, 0.f,//v3
//};
//std::vector<unsigned int> indices
//{
//	0,1,3,//top left triangle (v0, v1, v3)
//	3,1,2//bottom right triangle (v3, v1, v2)
//};
//std::vector<float> tex_coords
//{
//	0,0,
//	0,1,
//	1,1,
//	1,0,
//};

std::vector<float> vertices
{
	-0.5f,0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f,0.5f,-0.5f,

	-0.5f,0.5f,0.5f,
	-0.5f,-0.5f,0.5f,
	0.5f,-0.5f,0.5f,
	0.5f,0.5f,0.5f,

	0.5f,0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,0.5f,
	0.5f,0.5f,0.5f,

	-0.5f,0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,0.5f,
	-0.5f,0.5f,0.5f,

	-0.5f,0.5f,0.5f,
	-0.5f,0.5f,-0.5f,
	0.5f,0.5f,-0.5f,
	0.5f,0.5f,0.5f,

	-0.5f,-0.5f,0.5f,
	-0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,0.5f
};

std::vector<float> tex_coords
{
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0
};

std::vector<unsigned int> indices
{
	0,1,3,
	3,1,2,
	4,5,7,
	7,5,6,
	8,9,11,
	11,9,10,
	12,13,15,
	15,13,14,
	16,17,19,
	19,17,18,
	20,21,23,
	23,21,22

};

main_layer::main_layer(const pyro::static_shader& shader)
{
	m_static_shader = shader;
}

void main_layer::on_attach()
{
	m_model = pyro::loader::load_model(vertices, tex_coords, indices);
	m_texture = pyro::loader::load_texture("res/textures/face.png");
	m_static_model.create(m_model, m_texture);
	m_entity.create(m_static_model, { 0,0,-5 });
	pyro::obj_loader::load_obj("stall");
}

void main_layer::on_detach()
{
	m_static_shader.cleanup();
	pyro::loader::cleanup();
}

void main_layer::on_update(double dt)
{
	float f_dt = static_cast<float>(dt);
	m_entity.increase_rotation({f_dt, f_dt, 0.f});
	m_camera.move(dt);
}

void main_layer::on_render(pyro::renderer& renderer)
{
	m_static_shader.start();
	m_static_shader.load_view(pyro::maths::create_view_matrix(m_camera));
	renderer.render(m_entity, m_static_shader);
	m_static_shader.stop();
}

void main_layer::on_event(pyro::event& event)
{
}
