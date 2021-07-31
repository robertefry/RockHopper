
#include "RockHopper/Rendering/Platform/OpenGL_Shader.hh"

#include "RockHopper/Debug.hh"

#include <glad/glad.h>

namespace RockHopper
{

    OpenGL_Shader::~OpenGL_Shader()
    {
        if (m_ShaderProgram) glDeleteProgram(m_ShaderProgram);
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

    void OpenGL_Shader::bind() const
    {
        glUseProgram(m_ShaderProgram);
    }

    void OpenGL_Shader::unbind() const
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
        m_UniformCache.clear();
    }

    void OpenGL_Shader::def_uniform(std::string const& name, UniformType type, size_t size)
    {
        uint64_t location = glGetUniformLocation(m_ShaderProgram,name.c_str());
        ROCKHOPPER_ASSERT_FATAL((location != -1), "Uniform '{}' not found!", name);
        m_UniformCache[name] = UniformData{location,type,size};
    }

    void OpenGL_Shader::set_uniform(std::string const& name, float* data)
    {
        ROCKHOPPER_INTERNAL_ASSERT_ERROR(m_UniformCache.contains(name),"Uniform {} not found!",name);
        UniformData uniform_data = m_UniformCache.at(name);

        switch (uniform_data.type)
        {
            case UniformType::SCALAR: {
                glUniform1fv(uniform_data.location,uniform_data.size,data);
            } break;
            case UniformType::VEC2: {
                glUniform2fv(uniform_data.location,uniform_data.size,data);
            } break;
            case UniformType::VEC3: {
                glUniform3fv(uniform_data.location,uniform_data.size,data);
            } break;
            case UniformType::VEC4: {
                glUniform4fv(uniform_data.location,uniform_data.size,data);
            } break;
            case UniformType::MAT22: {
                glUniformMatrix2fv(uniform_data.location,uniform_data.size,false,data);
            } break;
            case UniformType::MAT23: {
                glUniformMatrix2x3fv(uniform_data.location,uniform_data.size,false,data);
            } break;
            case UniformType::MAT24: {
                glUniformMatrix2x4fv(uniform_data.location,uniform_data.size,false,data);
            } break;
            case UniformType::MAT32: {
                glUniformMatrix3x2fv(uniform_data.location,uniform_data.size,false,data);
            } break;
            case UniformType::MAT33: {
                glUniformMatrix3fv(uniform_data.location,uniform_data.size,false,data);
            } break;
            case UniformType::MAT34: {
                glUniformMatrix3x4fv(uniform_data.location,uniform_data.size,false,data);
            } break;
            case UniformType::MAT42: {
                glUniformMatrix4x2fv(uniform_data.location,uniform_data.size,false,data);
            } break;
            case UniformType::MAT43: {
                glUniformMatrix4x3fv(uniform_data.location,uniform_data.size,false,data);
            } break;
            case UniformType::MAT44: {
                glUniformMatrix4fv(uniform_data.location,uniform_data.size,false,data);
            } break;
        }
    }

} // namespace RockHopper
