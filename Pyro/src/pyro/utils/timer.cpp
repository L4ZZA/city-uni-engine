#include "pyro_pch.h"
#include "timer.h"
#include <GLFW/glfw3.h>

void pyro::timer::Start()
{
    m_started = true;
    // current time in seconds
    m_last_frame = glfwGetTime();
}

double pyro::timer::Elapsed()
{
    if (!m_started)
        return 0.0;

    // current time in seconds
    double currentFrame = glfwGetTime();

    m_delta_time = currentFrame - m_last_frame;
    m_last_frame = currentFrame;

    return m_delta_time;
}
