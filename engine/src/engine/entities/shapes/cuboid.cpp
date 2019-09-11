#include "pch.h"
#include "cuboid.h"
#include <engine.h>

engine::cuboid::cuboid(glm::vec3 position, bool is_static, glm::vec3 half_extents, const std::vector<ref<texture_2d>> textures, bool inwards)
	: game_object(position, is_static, 0.0f, glm::vec3(half_extents)), m_half_extents(half_extents)
{
	float orientation = 1;
	if (inwards)
		orientation = -1;

	float cuboid_vertices[]
	{
		//front
		//                                 position													normal					     tex coord       
		-1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z,		 0.0f, 0.0f,  1.0f * orientation,		 0.f, 0.f,
		 1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z,		 0.0f, 0.0f,  1.0f * orientation,		 1.f, 0.f,
		 1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z,		 0.0f, 0.0f,  1.0f * orientation,		 1.f, 1.f,
		-1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z,		 0.0f, 0.0f,  1.0f * orientation,		 0.f, 1.f,

		//right
		 1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z,		 1.0f * orientation, 0.0f, 0.0f,		 0.f, 0.f,
		 1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z,		 1.0f * orientation, 0.0f, 0.0f,		 1.f, 0.f,
		 1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z,		 1.0f * orientation, 0.0f, 0.0f,		 1.f, 1.f,
		 1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z,		 1.0f * orientation, 0.0f, 0.0f,		 0.f, 1.f,

		//back
		 1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z,		 0.0f, 0.0f, -1.0f * orientation,		 0.f, 0.f,
		-1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z,		 0.0f, 0.0f, -1.0f * orientation,		 1.f, 0.f,
		-1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z,		 0.0f, 0.0f, -1.0f * orientation,		 1.f, 1.f,
		 1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z,		 0.0f, 0.0f, -1.0f * orientation,		 0.f, 1.f,

		//left
		-1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z,		-1.0f * orientation, 0.0f, 0.0f,		 0.f, 0.f,
		-1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z,		-1.0f * orientation, 0.0f, 0.0f,		 1.f, 0.f,
		-1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z,		-1.0f * orientation, 0.0f, 0.0f,		 1.f, 1.f,
		-1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z,		-1.0f * orientation, 0.0f, 0.0f,		 0.f, 1.f,

		//top
		-1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z,		 0.0f,  1.0f * orientation, 0.0f,		 0.f, 0.f,
		 1.f * m_half_extents.x,  1.f * m_half_extents.y,  1.f * m_half_extents.z,		 0.0f,  1.0f * orientation, 0.0f,		 1.f, 0.f,
		 1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z,		 0.0f,  1.0f * orientation, 0.0f,		 1.f, 1.f,
		-1.f * m_half_extents.x,  1.f * m_half_extents.y, -1.f * m_half_extents.z,		 0.0f,  1.0f * orientation, 0.0f,		 0.f, 1.f,

		//bottom
		-1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z,		 0.0f, -1.0f * orientation, 0.0f,		 0.f, 0.f,
		 1.f * m_half_extents.x, -1.f * m_half_extents.y, -1.f * m_half_extents.z,		 0.0f, -1.0f * orientation, 0.0f,		 1.f, 0.f,
		 1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z,		 0.0f, -1.0f * orientation, 0.0f,		 1.f, 1.f,
		-1.f * m_half_extents.x, -1.f * m_half_extents.y,  1.f * m_half_extents.z,		 0.0f, -1.0f * orientation, 0.0f,		 0.f, 1.f,

	};

	const engine::ref<engine::vertex_buffer> cuboid_vb(engine::vertex_buffer::create(cuboid_vertices, sizeof(cuboid_vertices)));

	uint32_t cuboid_indices[]
	{
		 0,  1,  2,		 0,  2,  3,  //front
		 4,  5,  6,		 4,  6,  7,  //right
		 8,  9, 10,		 8, 10, 11,  //back
		12, 13, 14,		12, 14, 15,  //left
		16, 17, 18,		16, 18, 19,  //upper
		20, 21, 22,		20, 22, 23   //bottom
	};

	const engine::ref<engine::index_buffer> cuboid_ib(engine::index_buffer::create(cuboid_indices, sizeof(cuboid_indices) / sizeof(uint32_t)));

	cuboid_vb->layout({
		{engine::e_shader_data_type::float3, "a_position"},
		{engine::e_shader_data_type::float3, "a_normal"},
		{engine::e_shader_data_type::float2, "a_tex_coord"},
		});

	const engine::ref<engine::vertex_array> m_cube_va = engine::vertex_array::create();
	m_cube_va->add_buffer(cuboid_vb);
	m_cube_va->add_buffer(cuboid_ib);

	set_textures(textures);
	set_va(m_cube_va);
}

engine::cuboid::~cuboid() {}
