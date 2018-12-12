﻿#pragma once
#include "pyro/window.h"
#include "GLFW/glfw3.h"

namespace pyro
{
    class win_window : public window
    {
        //---------------------------------------------------------------------

    public:
        win_window(window_props const &p_props);
        virtual ~win_window();

        //---------------------------------------------------------------------

        void on_update() override;

        //---------------------------------------------------------------------

        void event_callback(event_callback_fn const& p_callback) override { data_.event_callback = p_callback; }
        void vsync(bool p_enabled) override;
        bool vsync() override;

        //---------------------------------------------------------------------

        unsigned width() const override  { return data_.width_; }
        unsigned height() const override { return data_.height_; }

        //---------------------------------------------------------------------

    private:
        virtual void init(window_props const &p_props);
        virtual void shut_down();

    private:
        GLFWwindow* window_;

        /// \brief struct to pass to the glfw callback user_ptr
        struct window_data
        {
            std::string title_;
            unsigned int width_;
            unsigned int height_;
            bool vsync_;

            event_callback_fn event_callback;
        };

        window_data data_;
    };

}