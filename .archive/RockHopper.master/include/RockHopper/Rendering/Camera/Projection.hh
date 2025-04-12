
#ifndef __HH_ROCKHOPPER_RENDERING_PROJECTION_
#define __HH_ROCKHOPPER_RENDERING_PROJECTION_

#include "RockHopper/Window/Window.fwd"
#include "RockHopper/Window/WindowEvents.hh"
#include "RockHopper/Utility/Cache/CacheVariable.hh"

#include <glm/glm.hpp>

#include <memory>

namespace RockHopper
{

    class Projection
        : public WindowEvent::ListenerType
    {
    public:
        virtual ~Projection() = default;

        virtual glm::mat4 matrix() const = 0;
        virtual float* data() = 0;

        virtual void on_event(WindowSizeEvent const&) {};
    };

    class ProjectionDefault : public Projection
    {
    public:
        virtual ~ProjectionDefault() = default;

        virtual glm::mat4 matrix() const override;
        virtual float* data() override;

    private:
        glm::mat4 m_Matrix{1.0f};
    };

    class ProjectionPerspective3D : public Projection
    {
    public:
        virtual ~ProjectionPerspective3D() = default;
        explicit ProjectionPerspective3D(float fovy, float znear, float zfar);

        virtual glm::mat4 matrix() const override;
        virtual float* data() override;

        virtual void on_event(WindowSizeEvent const&) override;

    private:
        float m_FovY, m_Aspect, m_zNear, m_zFar;
        CacheVariable<glm::mat4> m_Matrix{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_PROJECTION_ */
