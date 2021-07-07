
#include "RockHopper/Window/Backend/RenderContext.hh"

#include "RockHopper/Debug.hh"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/* ************************************************************************** */
// [Definition] RockHopper::OpenGL_Context
/* ************************************************************************** */

namespace RockHopper
{

    RenderContext::~RenderContext()
    {
    }

    RenderContext::RenderContext(GraphicsThread const& thread)
        : m_GraphicsThread{thread}
    {
    }

    std::future<void> RenderContext::initialize(GLFWwindow* handle)
    {
        return m_GraphicsThread.push_task([handle]()
        {
            // Make the window's context current
            glfwMakeContextCurrent(handle);

            // Initialize GLFW as the GLAD OpenGL Loader
            {
                ROCKHOPPER_INTERNAL_LOG_INFO("Setting GLFW as the GLAD OpenGL loader.");
                int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
                ROCKHOPPER_INTERNAL_ASSERT_FATAL(status,"GLAD failed to set GLFW as our OpenGL Loader!");
            }

            // Load OpenGL using GLAD
            {
                ROCKHOPPER_INTERNAL_LOG_INFO("Loading GLAD OpenGL.");
                int status = gladLoadGL();
                ROCKHOPPER_INTERNAL_ASSERT_FATAL(status,"GLAD failed to load OpenGL!");
            }
        });
    }

    std::future<void> RenderContext::dispose(GLFWwindow* handle)
    {
        return m_GraphicsThread.push_task([]()
        {
            // Push an empty task as an end point for all render tasks.
            // After waiting for this to complete, no more render tasks should be
            // present in the graphics thread.
        });
    }

} // namespace RockHopper
