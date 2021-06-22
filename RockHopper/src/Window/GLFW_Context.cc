
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
            ROCKHOPPER_INTERNAL_LOG_INFO("Initialized GLFW.");
        }
        m_Registered += 1;
        ROCKHOPPER_INTERNAL_LOG_INFO("Registered a new GLFW context - (count = {}).",m_Registered);
    }

    void GLFW_Context::Deregister()
    {
        if (m_Registered == 0)
        {
            ROCKHOPPER_INTERNAL_LOG_ERROR("Trying to remove non-existant GLFW context!");
        }
        m_Registered -= 1;

        ROCKHOPPER_INTERNAL_LOG_INFO("Deregistered a GLFW context - (count = {}).",m_Registered);
        if (m_Registered == 0)
        {
            glfwTerminate();
            ROCKHOPPER_INTERNAL_LOG_INFO("Terminated GLFW.");
        }
    }

} // namespace RockHopper
