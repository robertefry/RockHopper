
#include "RockHopper/UI/Window/GLFW_Context.hh"

#include "RockHopper/Debug.hh"

#include <GLFW/glfw3.h>

/* ************************************************************************** */
// RockHopper::UI::GLFW::GLFW_Context::Impl
/* ************************************************************************** */

namespace RockHopper::UI::GLFW
{

    class GLFW_Context::Impl
    {
        friend GLFW_Context;

    public:
        virtual ~Impl()
        {
            m_Renderer->wait_task([]()
            {
                ROCKHOPPER_INTERNAL_LOG_INFO("GLFW terminating");
                glfwTerminate();
            });
        }
        explicit Impl(
            std::shared_ptr<GX::Renderer> const& renderer
        )
            : m_Renderer{renderer}
        {
            m_Renderer->wait_task([]()
            {
                ROCKHOPPER_INTERNAL_LOG_INFO("GLFW initializing");
                glfwSetErrorCallback(GLFW_ErrorCallback);

                int status = glfwInit();
                ROCKHOPPER_INTERNAL_ASSERT_FATAL(status,"GLFW initialization failed");

                // Setup GLFW Window Hints
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,ROCKHOPPER_GLFW_VERSION_MAJOR);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,ROCKHOPPER_GLFW_VERSION_MINOR);
            });
        }

    private:
        static void GLFW_ErrorCallback(int error, const char* description)
        {
            ROCKHOPPER_INTERNAL_LOG_ERROR("GLFW Error {}: {}", error, description);
        }

    private:
        std::shared_ptr<GX::Renderer> m_Renderer;
    };

} // namespace RockHopper::UI::GLFW

/* ************************************************************************** */
// [Implementation] RockHopper::UI::GLFW::GLFW_Context
/* ************************************************************************** */

namespace RockHopper::UI::GLFW
{

    GLFW_Context::~GLFW_Context()
    {
        if (s_UseCount == 1) delete s_Impl;
        --s_UseCount;
    }

    GLFW_Context::GLFW_Context(
        std::shared_ptr<GX::Renderer> const& renderer )
    {
        if (s_UseCount == 0) s_Impl = new Impl{renderer};
        ++s_UseCount;
    }

    GLFW_Context::GLFW_Context(GLFW_Context const&)
    {
        ++s_UseCount;
    }

    GLFW_Context& GLFW_Context::operator=(GLFW_Context const&)
    {
        // nop
        return *this;
    }

    GLFW_Context::GLFW_Context(GLFW_Context&&)
    {
        ++s_UseCount;
    }

    GLFW_Context& GLFW_Context::operator=(GLFW_Context&&)
    {
        // nop
        return *this;
    }

} // namespace RockHopper::UI::GLFW
