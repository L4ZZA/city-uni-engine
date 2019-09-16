#include "pch.h"
#include "skybox.h"
#include <engine.h>

engine::skybox::skybox(float size, std::vector<engine::ref<engine::texture_2d>> textures) : m_size(size), m_textures(textures)
{
	std::vector<engine::mesh::vertex> skybox_vertices
	{
		//front
		//                position								 normal						 tex coord       
		{ {-1.f * m_size, -1.f * m_size,  1.f * m_size },		{ 0.0f, 0.0f,  -1.0f },		{ 1.f / 4.f, 1.f / 3.f } },
		{ { 1.f * m_size, -1.f * m_size,  1.f * m_size },		{ 0.0f, 0.0f,  -1.0f },		{ 2.f / 4.f, 1.f / 3.f } },
		{ { 1.f * m_size,  1.f * m_size,  1.f * m_size },		{ 0.0f, 0.0f,  -1.0f },		{ 2.f / 4.f, 2.f / 3.f } },
		{ {-1.f * m_size,  1.f * m_size,  1.f * m_size },		{ 0.0f, 0.0f,  -1.0f },		{ 1.f / 4.f, 2.f / 3.f } },

		//right
		{ { 1.f * m_size, -1.f * m_size,  1.f * m_size },		{ -1.0f, 0.0f, 0.0f},		{ 2.f / 4.f, 1.f / 3.f} },
		{ { 1.f * m_size, -1.f * m_size, -1.f * m_size },		{ -1.0f, 0.0f, 0.0f},		{ 3.f / 4.f, 1.f / 3.f} },
		{ { 1.f * m_size,  1.f * m_size, -1.f * m_size },		{ -1.0f, 0.0f, 0.0f},		{ 3.f / 4.f, 2.f / 3.f} },
		{ { 1.f * m_size,  1.f * m_size,  1.f * m_size },		{ -1.0f, 0.0f, 0.0f},		{ 2.f / 4.f, 2.f / 3.f} },

		//back
		{ { 1.f * m_size, -1.f * m_size, -1.f * m_size },		{ 0.0f, 0.0f, 1.0f },		{ 3.f / 4.f, 1.f / 3.f} },
		{ {-1.f * m_size, -1.f * m_size, -1.f * m_size },		{ 0.0f, 0.0f, 1.0f },		{ 4.f / 4.f, 1.f / 3.f} },
		{ {-1.f * m_size,  1.f * m_size, -1.f * m_size },		{ 0.0f, 0.0f, 1.0f },		{ 4.f / 4.f, 2.f / 3.f} },
		{ { 1.f * m_size,  1.f * m_size, -1.f * m_size },		{ 0.0f, 0.0f, 1.0f },		{ 3.f / 4.f, 2.f / 3.f} },

		//left
		{ {-1.f * m_size, -1.f * m_size, -1.f * m_size },		{1.0f, 0.0f, 0.0f},	    	{ 0.f / 4.f, 1.f / 3.f} },
		{ {-1.f * m_size, -1.f * m_size,  1.f * m_size },		{1.0f, 0.0f, 0.0f},		    { 1.f / 4.f, 1.f / 3.f} },
		{ {-1.f * m_size,  1.f * m_size,  1.f * m_size },		{1.0f, 0.0f, 0.0f},		    { 1.f / 4.f, 2.f / 3.f} },
		{ {-1.f * m_size,  1.f * m_size, -1.f * m_size },		{1.0f, 0.0f, 0.0f},		    { 0.f / 4.f, 2.f / 3.f} },

		//top
		{ {-1.f * m_size,  1.f * m_size,  1.f * m_size },		{ 0.0f,  -1.0f, 0.0f},		{ 1.f / 4.f, 2.f / 3.f} },
		{ { 1.f * m_size,  1.f * m_size,  1.f * m_size },		{ 0.0f,  -1.0f, 0.0f},		{ 2.f / 4.f, 2.f / 3.f} },
		{ { 1.f * m_size,  1.f * m_size, -1.f * m_size },		{ 0.0f,  -1.0f, 0.0f},		{ 2.f / 4.f, 3.f / 3.f} },
		{ {-1.f * m_size,  1.f * m_size, -1.f * m_size },		{ 0.0f,  -1.0f, 0.0f},		{ 1.f / 4.f, 3.f / 3.f} },

		//bottom
		{ {-1.f * m_size, -1.f * m_size, -1.f * m_size },		{ 0.0f, 1.0f, 0.0f},		{ 1.f / 4.f, 0.f / 3.f} },
		{ { 1.f * m_size, -1.f * m_size, -1.f * m_size },		{ 0.0f, 1.0f, 0.0f},		{ 2.f / 4.f, 0.f / 3.f} },
		{ { 1.f * m_size, -1.f * m_size,  1.f * m_size },		{ 0.0f, 1.0f, 0.0f},		{ 2.f / 4.f, 1.f / 3.f} },
		{ {-1.f * m_size, -1.f * m_size,  1.f * m_size },		{ 0.0f, 1.0f, 0.0f},		{ 1.f / 4.f, 1.f / 3.f} },
	};

	std::vector<uint32_t> skybox_indices
	{
		 0,  2,  1,		  0,  3,  2,  //front
		 4,  6,  5,		  4,  7,  6,  //right
		 8, 10,  9,		  8, 11, 10,  //back
		12, 14, 13,		 12, 15, 14,  //left
		16, 18, 17,		 16, 19, 18,  //upper
		20, 22, 21,		 20, 23, 22 //bottom
	};

	m_mesh = engine::mesh::create(skybox_vertices, skybox_indices);
}

engine::skybox::~skybox() {}

engine::ref<engine::skybox> engine::skybox::create(float size, std::vector<engine::ref<engine::texture_2d>> textures)
{
	return std::make_shared<engine::skybox>(size, textures);
}
