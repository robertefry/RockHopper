
#ifndef __HH_ROCKHOPPER_RENDERER_
#define __HH_ROCKHOPPER_RENDERER_

#include "RockHopper/Rendering/Mesh.fwd"
#include "RockHopper/Rendering/Shader.fwd"

#include "RockHopper/Engine/Engine.hh"
#include "RockHopper/Utility/TaskQueue.hh"

#include <memory>
#include <atomic>

namespace RockHopper
{

    class RenderThread
    {
        class Instance : public EngineThread, public TaskQueue
        {
        public:
            virtual ~Instance() = default;
            explicit Instance() : EngineThread{"RenderThread"} {}
        private:
            virtual void run() override;
        };

    public:
        virtual ~RenderThread();
        explicit RenderThread();

        explicit RenderThread(RenderThread const&);
        RenderThread& operator=(RenderThread const&);

        template <typename T_Func, typename... T_Args>
        auto push_task(T_Func&& func, T_Args&&... args)
            -> std::future<typename std::invoke_result<T_Func,T_Args...>::type>
        {
            return s_InstancePtr->push_task(
                std::forward<T_Func>(func), std::forward<T_Args>(args)...
            );
        }
        template <typename T_Func, typename... T_Args>
        auto wait_task(T_Func&& func, T_Args&&... args)
            -> std::invoke_result<T_Func,T_Args...>::type
        {
            return s_InstancePtr->wait_task(
                std::forward<T_Func>(func), std::forward<T_Args>(args)...
            );
        }

    private:
        static inline Instance* s_InstancePtr{};
        static inline std::atomic<unsigned> s_NumInstances = 0;
    };

    class Renderer
    {
    public:
        enum class API
        {
            None, OpenGL
        };

    protected:
        explicit Renderer() = default;

    public:
        virtual ~Renderer() = default;
        static void Create(API api);

        virtual void initialize() = 0;
        virtual void dispose() = 0;
        virtual void refresh() = 0;

        virtual void scene_begin() = 0;
        virtual void scene_end() = 0;
        virtual void submit(Shader const&, Mesh const&) = 0;

        inline static auto GetInstance() -> std::unique_ptr<Renderer>& { return s_Instance; }
        inline static auto GetAPI() -> API { return s_RenderAPI; }

    private:
        static inline std::unique_ptr<Renderer> s_Instance{};
        static inline API s_RenderAPI = API::None;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERER_ */
