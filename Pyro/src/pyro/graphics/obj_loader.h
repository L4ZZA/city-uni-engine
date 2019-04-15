#pragma once
#include "models/raw_model.h"
#include "loader.h"
#include <fstream>

namespace pyro
{
	class obj_loader
	{
	public:
		static std::vector<std::string> split(const std::string &s, char delimiter)
		{
			std::vector<std::string> tokens;
			std::string token;
			std::istringstream tokenStream(s);
			while(std::getline(tokenStream, token, delimiter))
			{
				tokens.push_back(token);
			}
			return tokens;
		}
		template <typename T>
		static std::vector<T> parse(const std::vector<std::string> &components)
		{
			std::vector<T> comps;
			for(const auto& comp : components)
			{
				CORE_ASSERT(comp.empty(), "Should not be empty.");
				T actual = atof(comp.c_str());
				T c = static_cast<T>(actual);
				comps.push_back(c);
			}
			return comps;
		}

		static raw_model load_obj(const std::string &file_name)
		{
			std::string full_path = "res/models/" + file_name + ".obj";
			std::ifstream stream(full_path);

			// prep data containers
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec2> tex_coords;
			std::vector<glm::vec3> normals;
			std::vector<unsigned int> indices;

			std::string line;
			std::stringstream ss;
			while(getline(stream, line))
			{
				// vertex position
				if(line.compare(0, 2, "v ") == 0)
				{
					// split from the first non-blank char after the identifier
					std::vector<std::string> components = split(line.substr(2), ' ');
					// parse components to floats
					auto f_comps = parse<float>(components);
					// compose vertex
					glm::vec3 vertex{f_comps[0],f_comps[1],f_comps[2]};
					// add to container
					vertices.push_back(vertex);
				}
				// tex coordinates
				if(line.compare(0, 3, "vt ") == 0)
				{
					// split from the first non-blank char after the identifier
					std::vector<std::string> components = split(line.substr(4), ' ');
					// parse components to floats
					auto f_comps = parse<float>(components);
					// compose tex coord
					glm::vec2 coord{f_comps[0],f_comps[1]};
					// add to container
					tex_coords.push_back(coord);
				}
				// normal
				if(line.compare(0, 3, "vn ") == 0)
				{
					// split from the first non-blank char after the identifier
					std::vector<std::string> components = split(line.substr(3), ' ');
					// parse components to floats
					auto f_comps = parse<float>(components);
					// compose normal
					glm::vec3 normal{f_comps[0],f_comps[1],f_comps[2]};
					// add to container
					normals.push_back(normal);
				}
				// face
				if(line.compare(0, 2, "f ") == 0)
				{
					// TODO: see if I need to convert all containers to float vectors
					break;
				}
			}

			return raw_model();
		}
	};
}
