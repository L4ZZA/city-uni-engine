#include "pch.h"
#include "obj_loader.h"
#include "engine/utils/file_utils.h"
#include "loader.h"

void engine::obj_loader::process_vertex(
	const std::vector<std::string> &components,
	std::vector<unsigned int> &indices,
	std::vector<glm::vec2> &tex_coords,
	std::vector<glm::vec3> &normals,
	std::vector<float> &textures_array,
	std::vector<float> &normals_array)
{
	// the first element of the triad is the vertex index 
	// (-1 because the obj starts at 1 and we start at 0)
	const auto current_vertex_index = parse<unsigned int>(components[0]) - 1;
	indices.push_back(current_vertex_index);

	// the second is the index of the texture in the texture list (aka our container)
	const glm::vec2 current_tex = tex_coords[parse<int>(components[1]) - 1];
	textures_array[current_vertex_index * 2 + 0] = current_tex.x;
	textures_array[current_vertex_index * 2 + 1] = 1.f - current_tex.y;// blender starts from the bottom left

	// the second is the index of the normal in the normals list (aka our container)
	const glm::vec3 current_normal = normals[parse<int>(components[2]) - 1];
	normals_array[current_vertex_index * 3 + 0] = current_normal.x;
	normals_array[current_vertex_index * 3 + 1] = current_normal.y;
	normals_array[current_vertex_index * 3 + 2] = current_normal.z;
}

engine::raw_model engine::obj_loader::load_obj(const std::string& file_name)
{
	const std::string full_path = "res/models/" + file_name + ".obj";
	const std::string content = read_file(full_path);
	std::istringstream stream(content);

	// prep data containers
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> tex_coords;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

	std::vector<float> textures_array;
	std::vector<float> normals_array;

	std::string line;
	const std::stringstream ss;
	while(getline(stream, line))
	{
		// vertex position
		if(line.compare(0, 2, "v ") == 0)
		{
			// split from the first non-blank char after the identifier
			const std::vector<std::string> components = split(line.substr(2), ' ');
			// parse components to floats
			const auto f_comps = parse<float>(components);
			// compose vertex
			const glm::vec3 vertex{ f_comps[0],f_comps[1],f_comps[2] };
			// add to container
			vertices.push_back(vertex);
		}
		// tex coordinates
		if(line.compare(0, 3, "vt ") == 0)
		{
			// split from the first non-blank char after the identifier
			const std::vector<std::string> components = split(line.substr(4), ' ');
			// parse components to floats
			const auto f_comps = parse<float>(components);
			// compose tex coord
			const glm::vec2 coord{ f_comps[0],f_comps[1] };
			// add to container
			tex_coords.push_back(coord);
		}
		// normal
		if(line.compare(0, 3, "vn ") == 0)
		{
			// split from the first non-blank char after the identifier
			const std::vector<std::string> components = split(line.substr(3), ' ');
			// parse components to floats
			auto f_comps = parse<float>(components);
			// compose normal
			const glm::vec3 normal{ f_comps[0],f_comps[1],f_comps[2] };
			// add to container
			normals.push_back(normal);
		}
		// face
		static bool first = true;
		if(line.compare(0, 2, "f ") == 0)
		{
			if(first)
			{
				textures_array = std::vector<float>(vertices.size() * 2);
				normals_array = std::vector<float>(vertices.size() * 3);
				first = false;
			}
			// split from the first non-blank char after the identifier
			const std::vector<std::string> these_vertices = split(line.substr(2), ' ');
			const std::vector<std::string> vertex1 = split(these_vertices[0], '/');
			const std::vector<std::string> vertex2 = split(these_vertices[1], '/');
			const std::vector<std::string> vertex3 = split(these_vertices[2], '/');

			process_vertex(vertex1, indices, tex_coords, normals, textures_array, normals_array);
			process_vertex(vertex2, indices, tex_coords, normals, textures_array, normals_array);
			process_vertex(vertex3, indices, tex_coords, normals, textures_array, normals_array);

		}
	}

	// convert the vertices list into float array
	std::vector<float> vertices_array;

	// convert the indices list into int array

	int index = 0;
	for(auto vec : vertices)
	{
		vertices_array.push_back(vec.x);
		vertices_array.push_back(vec.y);
		vertices_array.push_back(vec.z);
	}

	return loader::load_model(vertices_array, textures_array, normals_array, indices);
}
