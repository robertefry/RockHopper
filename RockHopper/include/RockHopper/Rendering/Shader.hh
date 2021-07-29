
#ifndef __HH_ROCKHOPPER_RENDERING_SHADER_
#define __HH_ROCKHOPPER_RENDERING_SHADER_

#include <memory>

namespace RockHopper
{

    class Shader
    {
    public:
        virtual ~Shader() = default;
        explicit Shader() = default;

        static std::unique_ptr<Shader> Create();

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        enum class Type { VERTEX, GEOMETRY, FRAGMENT };
        virtual void source_shader(Type type, std::string const& source) = 0;
        virtual void make_program() = 0;

        enum class UniformType
        {
            SCALAR, VEC2, VEC3, VEC4,
            MAT22, MAT23, MAT24, MAT32, MAT33, MAT34, MAT42, MAT43, MAT44
        };
        virtual void set_uniform(UniformType type, std::string const& name, size_t size, float* data) = 0;
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_SHADER_ */
