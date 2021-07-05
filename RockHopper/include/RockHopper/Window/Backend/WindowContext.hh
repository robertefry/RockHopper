
#ifndef __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_
#define __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_

#include "RockHopper/Window/Backend/GraphicsThread.hh"

#include <atomic>

#define ROCKHOPPER_GLFW_VERSION_MAJOR 3
#define ROCKHOPPER_GLFW_VERSION_MINOR 0

namespace RockHopper
{

    class WindowContext
    {
    public:
        virtual ~WindowContext();
        explicit WindowContext(GraphicsThread const& thread, void** handle);

        explicit WindowContext(WindowContext const&);
        WindowContext& operator=(WindowContext const&);

        template <typename T_Device, bool T_Enable>
        void set_callbacks(T_Device*);

    private:
        GraphicsThread m_GraphicsThread;
        void** m_Handle;

        static inline std::atomic<unsigned> s_NumInstances = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_WINDOW_BACKEND_GLFW_CONTEXT_ */
