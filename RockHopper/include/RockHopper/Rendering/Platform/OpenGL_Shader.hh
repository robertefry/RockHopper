
#ifndef __HH_ROCKHOPPER_RENDERING_OPENGL_SHADER_
#define __HH_ROCKHOPPER_RENDERING_OPENGL_SHADER_

#include "RockHopper/Rendering/Renderer.hh"
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

    private:
        uint32_t m_ShaderProgram{};
        std::unordered_map<Type,std::string> m_ShaderSources{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_OPENGL_SHADER_ */
