#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace pyro
{
	static std::string read_all(const std::string &file_path)
	{
		std::fstream file_stream;
		file_stream.open(file_path);

		if (!file_stream.is_open())
		{
			return "Could not open file.";
		}


		std::string line;
		std::stringstream content_stream;
		while (std::getline(file_stream, line))
		{
			content_stream << line << "\n";
		}

		auto content = content_stream.str();
		if (content.empty())
			return "Something went wrong.";

		return content;
	}

	static std::vector<std::string> read_lines(const std::string &file_path)
	{
		std::ifstream stream(file_path);

		std::string line;
		std::vector<std::string> lines;
		while (getline(stream, line))
		{
			auto s = line + "\n";
			lines.push_back(s);
		}

		return lines;
	}
}
