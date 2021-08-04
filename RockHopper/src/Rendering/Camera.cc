
#include "RockHopper/Rendering/Camera.hh"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RockHopper
{

    Camera::Camera()
    {
        m_ViewMatrix.set_recalculate_function([this]()
        {
            return glm::lookAt(m_Position,-m_Frame.axis_z,m_Frame.axis_y);
        });
    }

    glm::mat4 Camera::matrix() const
    {
        return *m_ViewMatrix;
    }

    float* Camera::data() const
    {
        return glm::value_ptr(*m_ViewMatrix);
    }

    void Camera::position(glm::vec3 const& vec)
    {
        m_Position = vec;
        m_ViewMatrix.mark_dirty();
    }

    void Camera::translate(glm::vec3 const& vec)
    {
        m_Position += vec;
        m_ViewMatrix.mark_dirty();
    }

    void Camera::rotate_x(float rad)
    {
        m_Frame.axis_y = glm::rotate(m_Frame.axis_y,rad,m_Frame.axis_x);
        m_Frame.axis_z = glm::rotate(m_Frame.axis_z,rad,m_Frame.axis_x);
        m_ViewMatrix.mark_dirty();
    }

    void Camera::rotate_y(float rad)
    {
        m_Frame.axis_x = glm::rotate(m_Frame.axis_x,rad,m_Frame.axis_y);
        m_Frame.axis_z = glm::rotate(m_Frame.axis_z,rad,m_Frame.axis_y);
        m_ViewMatrix.mark_dirty();
    }

    void Camera::rotate_z(float rad)
    {
        m_Frame.axis_x = glm::rotate(m_Frame.axis_x,rad,m_Frame.axis_z);
        m_Frame.axis_y = glm::rotate(m_Frame.axis_y,rad,m_Frame.axis_z);
        m_ViewMatrix.mark_dirty();
    }

} // namespace RockHopper
