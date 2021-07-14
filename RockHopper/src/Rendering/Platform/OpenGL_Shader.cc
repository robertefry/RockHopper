
#include "RockHopper/Rendering/Platform/OpenGL_Shader.hh"

#include "RockHopper/Debug.hh"

#include <glad/glad.h>

namespace RockHopper
{

    OpenGL_Shader::~OpenGL_Shader()
    {
        m_RenderThread.push_task([program=m_ShaderProgram]()
        {
            if (program) glDeleteProgram(program);
        });
    }

    OpenGL_Shader::OpenGL_Shader()
    {
    }

    OpenGL_Shader::OpenGL_Shader(OpenGL_Shader&& other)
        : m_ShaderProgram{other.m_ShaderProgram}
    {
        other.m_ShaderProgram = 0;
    }

    OpenGL_Shader& OpenGL_Shader::operator=(OpenGL_Shader&& other)
    {
        std::swap(m_ShaderProgram,other.m_ShaderProgram);
        return *this;
    }

    void OpenGL_Shader::bind()
    {
        glUseProgram(m_ShaderProgram);
    }

    void OpenGL_Shader::unbind()
    {
        glUseProgram(0);
    }

    void OpenGL_Shader::source_shader(Type type, std::string const& source)
    {
        m_ShaderSources[type] = source;
    }

    void OpenGL_Shader::make_program()
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
        if (m_ShaderProgram != 0)
        {
            glDeleteProgram(m_ShaderProgram);
        }
        m_ShaderProgram = program;
    }

} // namespace RockHopper
