
#ifndef __HH_ROCKHOPPER_RENDERING_SHADER_
#define __HH_ROCKHOPPER_RENDERING_SHADER_

#include "RockHopper/Window/Backend/GraphicsThread.hh"

#include <unordered_map>
#include <optional>
#include <string>
#include <mutex>

namespace RockHopper
{

    class Shader
    {
    public:
        virtual ~Shader();
        explicit Shader();

        explicit Shader(Shader&&);
        Shader& operator=(Shader&&);

        enum class Type { VERTEX, GEOMETRY, FRAGMENT };
        void source_shader(Type type, std::string const& source);
        void make_program();

        void bind() const;
        void unbind() const;

    private:
        GraphicsThread m_GraphicsThread{};
        uint64_t m_ShaderProgram{};
        std::mutex m_Mutex{};

        std::unordered_map<Type,std::string> m_ShaderSources{};
    };

} // namespace RockHopper

#endif /* __HH_ROCKHOPPER_RENDERING_SHADER_ */
