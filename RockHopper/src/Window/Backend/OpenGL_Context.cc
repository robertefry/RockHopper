
#include "RockHopper/Window/Backend/OpenGL_Context.hh"

#include "RockHopper/Debug.hh"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/* ************************************************************************** */
// [Definition] RockHopper::OpenGL_Context
/* ************************************************************************** */

namespace RockHopper
{

    void OpenGL_Context::Register()
    {
        if (m_Registered == 0)
        {
            // Initialize GLFW as the GLAD OpenGL Loader
            {
                int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
                ROCKHOPPER_INTERNAL_ASSERT_FATAL(status,"GLAD failed to set GLFW as our OpenGL Loader!");
                ROCKHOPPER_INTERNAL_LOG_INFO("GLAD succsessfully set GLFW as our OpenGL Loader.");
            }

            // Load OpenGL using GLAD
            {
                int status = gladLoadGL();
                ROCKHOPPER_INTERNAL_ASSERT_FATAL(status,"GLAD failed to load OpenGL!");
                ROCKHOPPER_INTERNAL_LOG_INFO("GLAD succsessfully loaded OpenGL.");
            }
        }
        m_Registered += 1;
    }

    void OpenGL_Context::Deregister()
    {
        m_Registered -= 1;
    }

} // namespace RockHopper
