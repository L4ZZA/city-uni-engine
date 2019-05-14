#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace engine
{
	static std::string read_file(const std::string &file_path)
	{
		std::fstream file_stream;
		file_stream.open(file_path);

		if(!file_stream.is_open())
		{
			return "Could not open file.";
		}


		std::string line;
		std::stringstream content_stream;
		while(std::getline(file_stream, line))
		{
			content_stream << line << "\n";
		}

		auto content = content_stream.str();
		if(content.empty())
			return "Something went wrong.";

		return content;
	}

	static std::vector<std::string> read_lines(const std::string &file_path)
	{
		std::ifstream stream(file_path);

		std::string line;
		std::vector<std::string> lines;
		while(getline(stream, line))
		{
			auto s = line + "\n";
			lines.push_back(s);
		}

		return lines;
	}

	static std::string read_file_c(const char* file_path)
	{
		FILE *file = fopen(file_path, "rt");
		fseek(file, 0, SEEK_END);
		const unsigned long length = ftell(file);
		char *data = new char[length + 1];
		memset(data, 0, length + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, length, file);
		fclose(file);

		std::string result(data);
		delete[] data;
		return result;
	}

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

	/// \brief https://stackoverflow.com/a/24327749/6120464
	static std::vector<std::string> split_fast(std::string const& original, char separator)
	{
		std::vector<std::string> results;
		std::string::const_iterator start = original.begin();
		std::string::const_iterator end = original.end();
		std::string::const_iterator next = std::find(start, end, separator);
		while(next != end)
		{
			results.push_back(std::string(start, next));
			start = next + 1;
			next = std::find(start, end, separator);
		}
		results.push_back(std::string(start, next));
		return results;
	}
}
