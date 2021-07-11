
#ifndef __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_
#define __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_

#include "RockHopper/Rendering/GraphicsThread.hh"

#include <atomic>

#define ROCKHOPPER_GLFW_VERSION_MAJOR 3
#define ROCKHOPPER_GLFW_VERSION_MINOR 0

class GLFWwindow;

namespace RockHopper
{

    class WindowContext
    {
    public:
        virtual ~WindowContext();
        explicit WindowContext(GraphicsThread const& thread);

        explicit WindowContext(WindowContext const&);
        WindowContext& operator=(WindowContext const&);

        bool is_closed_requested();
        std::future<void> refresh();

        inline auto get_window() -> GLFWwindow* { return m_WindowHandle; }
        inline void set_window(GLFWwindow* handle) { m_WindowHandle = handle; }

        template <typename T_Device, bool T_Enable>
        std::future<void> set_callbacks(T_Device*);

    private:
        GraphicsThread m_GraphicsThread;
        GLFWwindow* m_WindowHandle;

        static inline std::atomic<unsigned> s_NumInstances = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_ */
