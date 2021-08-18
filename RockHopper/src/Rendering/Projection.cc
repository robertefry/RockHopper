
#include "RockHopper/Rendering/Projection.hh"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace RockHopper
{

    glm::mat4 ProjectionDefault::matrix() const
    {
        return m_Matrix;
    }

    float* ProjectionDefault::data()
    {
        return glm::value_ptr(m_Matrix);
    }

    ProjectionPerspective3D::ProjectionPerspective3D(float fovy, float znear, float zfar)
        : m_FovY{fovy}, m_Aspect{1.0f}, m_zNear{znear}, m_zFar{zfar}
    {
        m_Matrix.set_recache_function([this]()
        {
            return glm::perspective(m_FovY,m_Aspect,m_zNear,m_zFar);
        });
        m_Matrix.mark_dirty();
    }

    glm::mat4 ProjectionPerspective3D::matrix() const
    {
        return *m_Matrix;
    }

    float* ProjectionPerspective3D::data()
    {
        return glm::value_ptr(*m_Matrix);
    }

    void ProjectionPerspective3D::on_event(WindowSizeEvent const& event)
    {
        m_Aspect = event.width / event.height;
        m_Matrix.mark_dirty();
    }

} // namespace RockHopper
