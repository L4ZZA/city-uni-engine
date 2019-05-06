﻿#pragma once

namespace engine
{
    class vertex_array
    {
    public:
        vertex_array() = default;
        ~vertex_array();

		void create();
        void bind() const;
        void unbind() const;

    private:
        unsigned int m_id{0};

    };
}
