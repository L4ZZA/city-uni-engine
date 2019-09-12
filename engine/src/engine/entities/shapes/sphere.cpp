#include "pch.h"
#include "sphere.h"
#include <engine.h>

engine::sphere::sphere(glm::vec3 position, bool is_static, uint32_t stacks, uint32_t slices, float radius, const std::vector<ref<texture_2d>> textures)
	: game_object(is_static, 1, glm::vec3(radius)), m_stacks(stacks), m_slices(slices), m_radius(radius)
{
	std::vector<float> vertices;
	std::vector<uint32_t> indices;

	const float sliceStep = 2 * PI / slices;
	const float stackStep = PI / stacks;
	const float lengthInv = 1.0f / radius;

	for (uint32_t i = 0; i <= stacks; ++i)
	{
		const float stackAngle = PI / 2 - i * stackStep;  // starting from pi/2 to -pi/2
		float xy = radius * cosf(stackAngle);             // r * cos(u)
		float z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sliceCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (uint32_t j = 0; j <= slices; ++j)
		{
			const float sliceAngle = j * sliceStep;       // starting from 0 to 2pi

			// vertex position (x, y, z)
			const float pos_x = xy * cosf(sliceAngle);
			const float pos_y = xy * sinf(sliceAngle);
			const float pos_z = z;
			vertices.push_back(pos_x);    // r * cos(u) * cos(v)
			vertices.push_back(pos_x);    // r * cos(u) * sin(v)
			vertices.push_back(pos_z);

			// normalized vertex normal (x, y, z)
			vertices.push_back(pos_x * lengthInv);
			vertices.push_back(pos_y * lengthInv);
			vertices.push_back(pos_z * lengthInv);

			// vertex tex coord (x, y) range between [0, 1]
			vertices.push_back(1.0f - static_cast<float>(j) / slices);
			vertices.push_back(static_cast<float>(i) / stacks);
		}
	}
	float* sphere_vertices = &vertices[0];
	const engine::ref<engine::vertex_buffer> sphere_vb(engine::vertex_buffer::create(sphere_vertices, sizeof(sphere_vertices)));

	for (uint32_t i = 0; i < stacks; ++i)
	{
		uint32_t k1 = i * (slices + 1);	  // beginning of current stack
		uint32_t k2 = k1 + slices + 1;      // beginning of next stack

		for (uint32_t j = 0; j < slices; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stacks - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}
	uint32_t* sphere_indices = &indices[0];

	const engine::ref<engine::index_buffer> sphere_ib(engine::index_buffer::create(sphere_indices, sizeof(sphere_indices) / sizeof(uint32_t)));

	sphere_vb->layout({
		{engine::e_shader_data_type::float3, "a_position"},
		{engine::e_shader_data_type::float3, "a_normal"},
		{engine::e_shader_data_type::float2, "a_tex_coord"},
		});

	const engine::ref<engine::vertex_array> sphere_va = engine::vertex_array::create();
	sphere_va->add_buffer(sphere_vb);
	sphere_va->add_buffer(sphere_ib);

	set_textures(textures);
	set_va(sphere_va);
}

engine::sphere::~sphere() {}
