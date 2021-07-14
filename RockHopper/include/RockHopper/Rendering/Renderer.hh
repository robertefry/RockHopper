
#ifndef __HH_ROCKHOPPER_RENDERER_
#define __HH_ROCKHOPPER_RENDERER_

#include <memory>

namespace RockHopper
{

    class Renderer
    {
    public:
        virtual ~Renderer() = default;

        virtual void initialize() = 0;
        virtual void dispose() = 0;
        virtual void refresh() = 0;

        enum class API
        {
            None, OpenGL
        };
        static std::unique_ptr<Renderer> Create(API api);
        inline static auto GetAPI() -> API { return s_RenderAPI; }

    private:
        static inline API s_RenderAPI = API::None;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERER_ */
