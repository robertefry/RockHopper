
#include "RockHopper/Rendering/Transform.hh"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RockHopper
{

    Transform::Transform()
        : m_Translation{0.0f}
        , m_Rotation{1.0f,0.0f,0.0f,0.0f}
        , m_Scale{1.0f}
    {
        m_Matrix.set_recache_function([this]()
        {
            return get_translation_m() * get_rotation_m() * get_scale_m();
        });
    }

    glm::mat4 Transform::matrix() const
    {
        return *m_Matrix;
    }

    float* Transform::data()
    {
        return glm::value_ptr(*m_Matrix);
    }

    void Transform::translate(glm::vec3 const& vec)
    {
        m_Translation += vec;
        m_Matrix.mark_dirty();
    }

    glm::mat4 Transform::get_translation_m() const
    {
        return glm::translate(glm::mat4{1.0f},m_Translation);
    }

    void Transform::rotate(float rad, glm::vec3 const& axis)
    {
        m_Rotation = glm::rotate(m_Rotation,rad,m_Rotation*axis);
        m_Matrix.mark_dirty();
    }

    glm::mat4 Transform::get_rotation_m() const
    {
        return glm::toMat4(m_Rotation);
    }

    void Transform::scale(float f)
    {
        scale(glm::vec3{f});
    }

    void Transform::scale(glm::vec3 const& vec)
    {
        m_Scale *= vec;
        m_Matrix.mark_dirty();
    }

    glm::mat4 Transform::get_scale_m() const
    {
        return glm::scale(glm::mat4{1.0f},m_Scale);
    }

} // namespace RockHopper
