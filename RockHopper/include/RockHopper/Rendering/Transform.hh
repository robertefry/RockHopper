
#ifndef __HH_ROCKHOPPER_RENDERING_TRANSFORM_
#define __HH_ROCKHOPPER_RENDERING_TRANSFORM_

#include "RockHopper/Utility/CacheVariable.hh"

#include <glm/glm.hpp>

namespace RockHopper
{

    class Transform
    {
    public:
        virtual ~Transform() = default;
        explicit Transform();

        auto matrix() const -> glm::mat4;
        auto data() -> float*;

        void position(glm::vec3 const&);
        void translate(glm::vec3 const&);

        void rotation(glm::quat const&);
        void rotate(float rad, glm::vec3 const& axis);

        void scale(glm::vec3 const&);
        void grow(glm::vec3 const&);

        auto get_translation() const { return m_Translation; }
        auto get_rotation() const { return m_Rotation; }
        auto get_scale() const { return m_Scale; }

    public:
        glm::mat4 m_Translation{1.0f}, m_Rotation{1.0f}, m_Scale{1.0f};
        CacheVariable<glm::mat4> m_Matrix{1.0f};

    public:
        static inline glm::vec3 const AXIS_X {1.0f,0.0f,0.0f};
        static inline glm::vec3 const AXIS_Y {0.0f,1.0f,0.0f};
        static inline glm::vec3 const AXIS_Z {0.0f,0.0f,1.0f};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_TRANSFORM_ */
