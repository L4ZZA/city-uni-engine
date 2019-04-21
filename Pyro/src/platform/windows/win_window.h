#pragma once
#include "pyro/window.h"
#include "GLFW/glfw3.h"

namespace pyro
{
    class win_window : public window
    {
        //---------------------------------------------------------------------

    public:
        win_window(window_props const &props);
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
        virtual void init(window_props const &props);
        virtual void shut_down();

    private:
        GLFWwindow* m_window;

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
