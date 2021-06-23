
#include "GLFW_Context.hh"

#include "RockHopper/Debug.hh"

#include <GLFW/glfw3.h>

namespace RockHopper
{

    void GLFW_Context::Register()
    {
        ROCKHOPPER_INTERNAL_ASSERT_FATAL((not m_Registered),"Trying to initialize GLFW more than once!");

        int status = glfwInit();
        ROCKHOPPER_INTERNAL_ASSERT_FATAL((status == GLFW_TRUE),"Failed to initialize GLFW!");
        ROCKHOPPER_INTERNAL_LOG_INFO("Initialized GLFW.");
        m_Registered = true;
    }

    void GLFW_Context::Deregister()
    {
        ROCKHOPPER_INTERNAL_ASSERT_FATAL(m_Registered,"Trying to terminate non-existent GLFW!");

        glfwTerminate();
        ROCKHOPPER_INTERNAL_LOG_INFO("Terminated GLFW.");
        m_Registered = false;
    }

} // namespace RockHopper
