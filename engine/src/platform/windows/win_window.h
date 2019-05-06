#pragma once
#include "engine/window.h"
#include "engine/renderer/graphics_context.h"
#include "GLFW/glfw3.h"

namespace engine
{
    class win_window : public window
    {
        //---------------------------------------------------------------------

    public:
        win_window(const std::string& name, window_props const &props);
        virtual ~win_window();

        //---------------------------------------------------------------------

        void on_update() override;

        //---------------------------------------------------------------------

        void event_callback(event_callback_fn const& callback) override { m_data.event_callback = callback; }
        void vsync(bool enabled) override;
        bool vsync() override;

        virtual void* native_window() const override { return m_window; }

        //---------------------------------------------------------------------

        unsigned int width() const override { return m_data.width; }
        unsigned int height() const override { return m_data.height; }

        //---------------------------------------------------------------------

    private:
        virtual void init(const std::string& name, window_props const &props);
        virtual void shut_down();

    private:
        GLFWwindow* m_window;
		graphics_context* m_context;

        /// \brief struct to pass to the glfw callback user_ptr
        struct window_data
        {
            std::string title;
            unsigned int width;
            unsigned int height;
            bool vsync;

            event_callback_fn event_callback;
        };

        window_data m_data;
    };

}
