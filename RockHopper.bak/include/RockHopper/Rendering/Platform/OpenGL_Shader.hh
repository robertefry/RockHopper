
#ifndef __HH_ROCKHOPPER_RENDERING_OPENGL_SHADER_
#define __HH_ROCKHOPPER_RENDERING_OPENGL_SHADER_

#include "RockHopper/Rendering/Shader.hh"

#include <cstdint>
#include <unordered_map>

namespace RockHopper
{

    class OpenGL_Shader : public Shader
    {
    public:
        virtual ~OpenGL_Shader();
        explicit OpenGL_Shader();

        explicit OpenGL_Shader(OpenGL_Shader&&);
        OpenGL_Shader& operator=(OpenGL_Shader&&);

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void source_shader(Type type, std::string const& source) override;
        virtual void make_program() override;

        virtual void def_uniform(std::string const& name, UniformType type, size_t size) override;
        virtual void set_uniform(std::string const& name, float* data) override;

    private:
        uint32_t m_ShaderProgram{};
        std::unordered_map<Type,std::string> m_ShaderSources{};

        struct UniformData
        {
            uint64_t location;
            UniformType type;
            size_t size;
        };
        std::unordered_map<std::string,UniformData> m_UniformCache{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_OPENGL_SHADER_ */
