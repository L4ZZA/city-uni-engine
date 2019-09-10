#pragma once
#include <string>

namespace engine
{
    namespace utils
    {
        static std::string file_extension(const std::string& file_name)
        {
            const auto dot_pos = file_name.find_last_of('.');
            const auto extension_size = file_name.size() - dot_pos;
            std::string extension = file_name.substr(dot_pos + 1, extension_size);
            return extension;
        }
    }
}