﻿#pragma once

#include "window.h"
#include "layers_stack.h"
#include "events/event.h"
#include "events/application_event.h"
#include "imgui/imgui_layer.h"
#include "graphics/renderer.h"
#include "graphics/shaders/static_shader.h"


namespace pyro
{
	class timer;

    class PYRO_API application
    {
    public:
        application();
        virtual ~application();

        /// \brief
        void run();

        /// \brief
        void on_event(event &event);

        /// \brief Adds a layer to the stack.
        void push_layer(layer *layer);
        /// \brief Adds a overlay to the stack.
        void push_overlay(layer *overlay);

        /// \brief Returns a reference to the application window.
        window& get_window() const { return *m_window; }
        /// \brief Returns a reference to the application.
        static application& instance() { return *s_instance; }

    private:
        bool on_window_close(window_closed_event &event);

    private:
        std::unique_ptr<window>         m_window;
        timer* 							m_timer{ nullptr };
        bool                            m_running{ true };
        layers_stack                    m_layers_stack;

    protected:
        renderer                        m_renderer;
		static_shader					m_static_shader;

    private:
        static application*             s_instance;
        inline static const double		s_fps = 60.0;
        inline static const double		s_secondsPerFrame = 1.0 / s_fps;

    };


    application* create_application();
}
