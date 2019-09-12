#include "pch.h"
#include "skybox.h"
#include <engine.h>

engine::skybox::skybox(float size, glm::vec3 position, float rotation, const std::vector<ref<texture_2d>> textures) : game_object(true, 0, glm::vec3(size)), m_size(size)
{
	float skybox_vertices[]
	{
		//front
		//                                 position														  normal					  tex coord       
		-1.f * size, -1.f * size,  1.f * size,		  0.0f,  0.0f, -1.0f,		1.f / 4.f, 1.f / 3.f,
		 1.f * size, -1.f * size,  1.f * size,	 	  0.0f,  0.0f, -1.0f,		2.f / 4.f, 1.f / 3.f,
		 1.f * size,  1.f * size,  1.f * size,		  0.0f,  0.0f, -1.0f,		2.f / 4.f, 2.f / 3.f,
		-1.f * size,  1.f * size,  1.f * size,		  0.0f,  0.0f, -1.0f,		1.f / 4.f, 2.f / 3.f,

		//right
		 1.f * size, -1.f * size,  1.f * size,		 -1.0f,  0.0f,  0.0f,		2.f / 4.f, 1.f / 3.f,
		 1.f * size, -1.f * size, -1.f * size,		 -1.0f,  0.0f,  0.0f,		3.f / 4.f, 1.f / 3.f,
		 1.f * size,  1.f * size, -1.f * size,		 -1.0f,  0.0f,  0.0f,		3.f / 4.f, 2.f / 3.f,
		 1.f * size,  1.f * size,  1.f * size,		 -1.0f,  0.0f,  0.0f,		2.f / 4.f, 2.f / 3.f,

		//back
		 1.f * size, -1.f * size, -1.f * size,		  0.0f,  0.0f,  1.0f,		3.f / 4.f, 1.f / 3.f,
		-1.f * size, -1.f * size, -1.f * size,		  0.0f,  0.0f,  1.0f,		4.f / 4.f, 1.f / 3.f,
		-1.f * size,  1.f * size, -1.f * size,		  0.0f,  0.0f,  1.0f,		4.f / 4.f, 2.f / 3.f,
		 1.f * size,  1.f * size, -1.f * size,		  0.0f,  0.0f,  1.0f,		3.f / 4.f, 2.f / 3.f,

		//left
		-1.f * size, -1.f * size, -1.f * size,		  1.0f,  0.0f,  0.0f,	    0.f / 4.f, 1.f / 3.f,
		-1.f * size, -1.f * size,  1.f * size,		  1.0f,  0.0f,  0.0f,		1.f / 4.f, 1.f / 3.f,
		-1.f * size,  1.f * size,  1.f * size,		  1.0f,  0.0f,  0.0f,		1.f / 4.f, 2.f / 3.f,
		-1.f * size,  1.f * size, -1.f * size,		  1.0f,  0.0f,  0.0f,		0.f / 4.f, 2.f / 3.f,

		//top
		-1.f * size,  1.f * size,  1.f * size,		  0.0f, -1.0f,  0.0f,		1.f / 4.f, 2.f / 3.f,
		 1.f * size,  1.f * size,  1.f * size,		  0.0f, -1.0f,  0.0f,		2.f / 4.f, 2.f / 3.f,
		 1.f * size,  1.f * size, -1.f * size,		  0.0f, -1.0f,  0.0f,		2.f / 4.f, 3.f / 3.f,
		-1.f * size,  1.f * size, -1.f * size,		  0.0f, -1.0f,  0.0f,		1.f / 4.f, 3.f / 3.f,

		//bottom
		-1.f * size, -1.f * size, -1.f * size,		  0.0f,  1.0f,  0.0f,		1.f / 4.f, 0.f / 3.f,
		 1.f * size, -1.f * size, -1.f * size,		  0.0f,  1.0f,  0.0f,		2.f / 4.f, 0.f / 3.f,
		 1.f * size, -1.f * size,  1.f * size,		  0.0f,  1.0f,  0.0f,		2.f / 4.f, 1.f / 3.f,
		-1.f * size, -1.f * size,  1.f * size,		  0.0f,  1.0f,  0.0f,		1.f / 4.f, 1.f / 3.f,
	};

	const engine::ref<engine::vertex_buffer> skybox_vb(engine::vertex_buffer::create(skybox_vertices, sizeof(skybox_vertices)));

	uint32_t skybox_indices[]
	{
		 0,  2,  1,		  0,  3,  2,  //front
		 4,  6,  5,		  4,  7,  6,  //right
		 8, 10,  9,		  8, 11, 10,  //back
		12, 14, 13,		 12, 15, 14,  //left
		16, 18, 17,		 16, 19, 18,  //upper
		20, 22, 21,		 20, 23, 22 //bottom
	};

	const engine::ref<engine::index_buffer> skybox_ib(engine::index_buffer::create(skybox_indices, sizeof(skybox_indices) / sizeof(uint32_t)));

	skybox_vb->layout({
		{engine::e_shader_data_type::float3, "a_position"},
		{engine::e_shader_data_type::float3, "a_normal"},
		{engine::e_shader_data_type::float2, "a_tex_coord"},
		});

	const engine::ref<engine::vertex_array> skybox_va = engine::vertex_array::create();
	skybox_va->add_buffer(skybox_vb);
	skybox_va->add_buffer(skybox_ib);

	set_textures(textures);
	set_va(skybox_va);

	set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
	set_rotation_amount(rotation);
	set_position(position);
}

engine::skybox::~skybox() {}

void engine::skybox::update(glm::vec3 position, float rotation)
{
	set_position(position);
	set_rotation_amount(rotation);
}
