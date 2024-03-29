
#ifndef __HH_ROCKHOPPER_RENDERER_
#define __HH_ROCKHOPPER_RENDERER_

#include "RockHopper/Rendering/Camera/Camera.fwd"
#include "RockHopper/Rendering/Shader.fwd"
#include "RockHopper/Rendering/Mesh.fwd"
#include "RockHopper/Rendering/Transform.fwd"

#include "RockHopper/Engine/EngineThread.hh"

#include <thread-tools/task_queue.hh>

#include <memory>
#include <atomic>

namespace RockHopper
{

    class Renderer : public EngineThread
        , public thd::task_queue
    {
    public:
        virtual ~Renderer();
        explicit Renderer(std::string const& api);

        enum class API
        {
            None, OpenGL
        };
        static void Create(API api);
        static void Destroy();

        virtual void viewport(float x, float y, float w, float h) = 0;

        virtual void scene_begin(Camera*) = 0;
        virtual void scene_end() = 0;

        virtual void submit(Shader&, Mesh&);
        virtual void submit(Shader&, Mesh&, Transform&) = 0;

        static auto GetAPI() -> API { return s_RendererAPI; }
        static auto GetInstance() -> auto& { return s_Instance; }

    private:
        virtual void init() = 0;
        virtual void dispose() = 0;
        virtual void tick() override;

    private:
        static inline std::unique_ptr<Renderer> s_Instance{};
        static inline API s_RendererAPI = API::None;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERER_ */
