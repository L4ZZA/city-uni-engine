#include "pch.h"
#include "renderer.h"
#include "glad/glad.h"
#include "engine/utils/maths.h"
#include "engine/application.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"


void engine::renderer::init(camera& camera, shader &shader)
{
	// TODO: load projection matrix only once
	//shader.bind();
	//auto width = static_cast<float>(application::instance().get_window().width());
	//auto height = static_cast<float>(application::instance().get_window().height());
	//shader.set_uniform("", maths::create_projection_matrix(s_fov, width, height, s_near, s_far));
	//shader.unbind();
}

void engine::renderer::prepare() const
{
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearDepth(1.0f);
	glClearColor(0.7f, 0.7f, 0.9f, 1);
}

// TODO: do the same of 3D for 2D passing a sprite ptr or reference
//void engine::renderer::render(const sprite &entity, shader &shader) const {}

void engine::renderer::render_3d(const model& model, shader& shader) const
{
	auto meshes = model.meshes();
	for(const auto& mesh : meshes)
		render_3d(mesh, shader);
}

void engine::renderer::render_3d(const mesh& mesh, shader& shader) const
{
	auto textures = mesh.textures();
	auto va = mesh.va();
	const auto indices_count = mesh.indices_count();

	uint32 diffuseN = 1, specularN = 1;
	for(int32 i = 0; i < textures.size(); i++)
	{
		std::string num, name = textures[i].type();

		if(name == "diffuse")
			num = std::to_string(diffuseN++);
		else if(name == "specular")
			num = std::to_string(specularN++);

		textures[i].bind(i); // Bind the texture to slot i
		// TODO: Should not need cast
		shader.set_uniform("material." + name + num, i); // Pass slot i to the shader material struct
	}

	// draw mesh
	va.bind();
	glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT, nullptr);
	va.unbind();
}
