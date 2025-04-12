
#ifndef __HH_ROCKHOPPER_RENDERING_CAMERA_
#define __HH_ROCKHOPPER_RENDERING_CAMERA_

#include "RockHopper/Window/WindowEvents.hh"
#include "RockHopper/Rendering/Camera/Projection.hh"
#include "RockHopper/Utility/Cache/CacheVariable.hh"

#include <glm/glm.hpp>

#include <memory>

namespace RockHopper
{

    class Camera
        : public WindowEvent::ListenerType
    {
    public:
        struct Frame
        {
            glm::vec3 axis_x {1,0,0};
            glm::vec3 axis_y {0,1,0};
            glm::vec3 axis_z {0,0,1};
        };

        virtual ~Camera() = default;
        explicit Camera();

        virtual void projection(std::shared_ptr<Projection> const&);

        virtual glm::mat4 matrix() const;
        virtual float* data();

        virtual void position(glm::vec3 const&);
        virtual void translate(glm::vec3 const&);

        virtual void rotation(glm::vec3 const&);
        virtual void rotate_x(float rad);
        virtual void rotate_y(float rad);
        virtual void rotate_z(float rad);

    protected:
        void on_event(WindowSizeEvent const&);

    private:
        glm::vec3 m_Position{};
        Camera::Frame m_Frame{};

        std::shared_ptr<Projection> m_Projection{};
        CacheVariable<glm::mat4> m_ViewMatrix{1.0f};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_CAMERA_ */
