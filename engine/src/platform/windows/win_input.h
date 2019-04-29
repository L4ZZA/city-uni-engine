﻿#pragma once

#include "engine/input.h"

namespace engine
{
    class win_input : public input
    {
    public:
		static void init();
		static void shutdown();
        bool key_pressed_impl(int key_code) const override;
        bool mouse_button_pressed_impl(int button) const override;

    protected:
        std::pair<float, float> mouse_position_impl() const override;
        float mouse_x_impl() const override;
        float mouse_y_impl() const override;
    };
}