
#include "RockHopper/Rendering/Transform.hh"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RockHopper
{

    Transform::Transform()
    {
        m_Matrix.set_recache_function([this]()
        {
            return m_Translation * m_Rotation * m_Scale;
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

    void Transform::position(glm::vec3 const& pos)
    {
        m_Translation = glm::translate(glm::mat4{1.0f},pos);
        m_Matrix.mark_dirty();
    }

    void Transform::translate(glm::vec3 const& dif)
    {
        m_Translation = glm::translate(m_Translation,dif);
        m_Matrix.mark_dirty();
    }

    void Transform::rotation(glm::quat const& rot)
    {
        m_Rotation = glm::toMat4(rot);
        m_Matrix.mark_dirty();
    }

    void Transform::rotate(float rad, glm::vec3 const& axis)
    {
        m_Rotation = glm::rotate(m_Rotation,rad,axis);
        m_Matrix.mark_dirty();
    }

    void Transform::scale(glm::vec3 const& vec)
    {
        m_Scale = glm::scale(glm::mat4{1.0f},vec);
        m_Matrix.mark_dirty();
    }

    void Transform::grow(glm::vec3 const& vec)
    {
        m_Scale = glm::scale(m_Scale,vec);
        m_Matrix.mark_dirty();
    }

} // namespace RockHopper
