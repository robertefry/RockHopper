
#include "RockHopper/Rendering/Shader.hh"

#include "RockHopper/Debug.hh"

#include <glad/glad.h>

namespace RockHopper
{

    Shader::~Shader()
    {
        m_RenderThread.wait_task([&]()
        {
            if (m_ShaderProgram) glDeleteProgram(m_ShaderProgram);
        });
    }

    Shader::Shader()
    {
    }

    Shader::Shader(Shader&& other)
    {
        *this = std::move(other);
    }

    Shader& Shader::operator=(Shader&& other)
    {
        std::scoped_lock lock {m_Mutex,other.m_Mutex};

        m_ShaderProgram = other.m_ShaderProgram;
        other.m_ShaderProgram = 0;

        m_ShaderSources = std::move(other.m_ShaderSources);

        return *this;
    }

    void Shader::source_shader(Type type, std::string const& source)
    {
        m_ShaderSources[type] = source;
    }

    void Shader::make_program()
    {
        std::function const CompileShader = [](Type type, std::string const& source) -> GLuint
        {
            GLuint const gl_type = [&]()
            {
                switch (type)
                {
                    case Type::VERTEX: return GL_VERTEX_SHADER;
                    case Type::GEOMETRY: return GL_GEOMETRY_SHADER;
                    case Type::FRAGMENT: return GL_FRAGMENT_SHADER;
                }
                ROCKHOPPER_INTERNAL_LOG_FATAL("Trying to compile a shader of unknown type.");
                return 0;
            }();

            // Create a fresh shader handle
            GLuint shader = glCreateShader(gl_type);

            // Send the shader source code to OpenGL
            GLchar const* gl_source = (GLchar const*)source.c_str();
            glShaderSource(shader,1,&gl_source,0);

            // Compiler the shader
            glCompileShader(shader);

            // Print an error message if compilation failed
            GLint succsess = 0;
            glGetShaderiv(shader,GL_COMPILE_STATUS,&succsess);
            if (succsess == GL_FALSE)
            {
                GLint length = 0;
                glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&length);

                std::vector<GLchar> infolog (length);
                glGetShaderInfoLog(shader,length,&length,&infolog[0]);

                glDeleteShader(shader);

                std::string error = std::string{infolog.begin(),infolog.end()};
                ROCKHOPPER_INTERNAL_LOG_FATAL("Shader compilation failed: {}.",error);
            }

            return shader;
        };
        m_RenderThread.push_task([&,CompileShader]()
        {
            // Compile the sourced shaders
            std::vector<GLuint> shaders;
            for (auto const& [type,source] : m_ShaderSources)
            {
                GLuint shader = CompileShader(type,source);
                shaders.push_back(shader);
            }

            // Create a fresh program handle
            GLuint program = glCreateProgram();

            // Attach the shaders to the program
            for (GLuint const& shader : shaders)
            {
                glAttachShader(program,shader);
            }

            // Link the program
            glLinkProgram(program);

            // Print an error message if compilation failed
            GLint succsess = 0;
            glGetProgramiv(program,GL_LINK_STATUS,&succsess);
            if (succsess == GL_FALSE)
            {
                GLint length = 0;
                glGetProgramiv(program,GL_LINK_STATUS,&succsess);

                std::vector<GLchar> infolog (length);
                glGetProgramInfoLog(program,length,&length,&infolog[0]);

                for (GLuint const& shader : shaders)
                {
                    glDeleteShader(shader);
                }
                glDeleteProgram(program);

                std::string error = std::string{infolog.begin(),infolog.end()};
                ROCKHOPPER_INTERNAL_LOG_FATAL("Shader linking failed: {}.",error);
            }

            // Detach and delete the compiled shaders after a successful link
            for (GLuint const& shader : shaders)
            {
                glDetachShader(program,shader);
                glDeleteShader(shader);
            }

            // Save the compiled program
            std::unique_lock lock {m_Mutex};
            if (m_ShaderProgram != 0)
            {
                glDeleteProgram(m_ShaderProgram);
            }
            m_ShaderProgram = program;
        });
    }

    void Shader::bind() const
    {
        glUseProgram(m_ShaderProgram);
    }

    void Shader::unbind() const
    {
        glUseProgram(0);
    }

} // namespace RockHopper
