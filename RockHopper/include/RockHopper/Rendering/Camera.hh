
#ifndef __HH_ROCKHOPPER_RENDERING_CAMERA_
#define __HH_ROCKHOPPER_RENDERING_CAMERA_

#include "RockHopper/Utility/CacheVariable.hh"

#include <glm/glm.hpp>

namespace RockHopper
{

    class Camera
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

        virtual glm::mat4 matrix() const;
        virtual float* data();

        virtual void position(glm::vec3 const&);
        virtual void translate(glm::vec3 const&);

        virtual void rotation(glm::vec3 const&);
        virtual void rotate_x(float rad);
        virtual void rotate_y(float rad);
        virtual void rotate_z(float rad);

    private:
        glm::vec3 m_Position{};
        Camera::Frame m_Frame{};

        CacheVariable<glm::mat4> m_ViewMatrix{1.0f};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_CAMERA_ */
