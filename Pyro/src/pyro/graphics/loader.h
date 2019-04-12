#pragma once
#include "raw_model.h"

namespace pyro
{
    class loader
    {
    public:
        static raw_model load_model(const std::vector<float> &positions);
        static void cleanup();

    private:
        static unsigned int create_vao();

        static void store_data_in_attributes_list(int attribute_number, const std::vector<float> &data);

        static void bind_vao();
        static void unbind_vao();

    private:
        inline static std::vector<unsigned int> m_vaos{};
        inline static std::vector<unsigned int> m_vbos{};
    };
}
