
#ifndef __HH_ROCKHOPPER_RENDERING_TRANSFORM_
#define __HH_ROCKHOPPER_RENDERING_TRANSFORM_

#include "RockHopper/Utility/Cache/CacheVariable.hh"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iosfwd>
#include <glm/gtx/string_cast.hpp>

namespace RockHopper
{

    class Transform
    {
    public:
        virtual ~Transform() = default;
        explicit Transform();

        auto matrix() const -> glm::mat4;
        auto data() -> float*;

        void translate(glm::vec3 const& vec);
        glm::vec3 get_translation() const { return m_Translation; }
        glm::mat4 get_translation_m() const;

        void rotate(float rad, glm::vec3 const& axis);
        glm::quat get_rotation() const { return m_Rotation; }
        glm::mat4 get_rotation_m() const;

        void scale(float f);
        void scale(glm::vec3 const& vec);
        glm::vec3 get_scale() const { return m_Scale; }
        glm::mat4 get_scale_m() const;

    private:
        glm::vec3 m_Translation;
        glm::quat m_Rotation;
        glm::vec3 m_Scale;

        CacheVariable<glm::mat4> m_Matrix{1.0f};
    };

} // namespace RockHopper

namespace std
{
    using namespace RockHopper;

    template <typename T_Char>
    std::basic_ostream<T_Char>& operator<<(std::basic_ostream<T_Char>& ostr, Transform const& transform)
    {
        return ostr
            << "t:" << glm::to_string(transform.get_translation()) << "\n"
            << "r:" << glm::to_string(transform.get_rotation()) << "\n"
            << "s:" << glm::to_string(transform.get_scale());
    }

} // namespace std

#endif /* __HH_ROCKHOPPER_RENDERING_TRANSFORM_ */
