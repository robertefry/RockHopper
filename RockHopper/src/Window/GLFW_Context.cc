
#include "GLFW_Context.hh"

#include "RockHopper/Debug.hh"

#include <GLFW/glfw3.h>

namespace RockHopper
{

    void GLFW_Context::Register()
    {
        if (m_Registered == 0)
        {
            int status = glfwInit();
            ROCKHOPPER_INTERNAL_ASSERT_FATAL((status == GLFW_TRUE), "Failed to initialize GLFW!");
        }
        m_Registered += 1;
    }

    void GLFW_Context::Deregister()
    {
        if (m_Registered == 0)
        {
            ROCKHOPPER_INTERNAL_LOG_ERROR("Trying to remove non-existant GLFW context!");
        }
        m_Registered -= 1;

        if (m_Registered == 0)
        {
            glfwTerminate();
        }
    }

} // namespace RockHopper
