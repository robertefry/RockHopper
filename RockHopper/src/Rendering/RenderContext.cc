
#include "RockHopper/Rendering/RenderContext.hh"

#include "RockHopper/Debug.hh"

#include <glad/glad.h>

/* ************************************************************************** */
// [Definition] RockHopper::OpenGL_Context
/* ************************************************************************** */

namespace RockHopper
{

    void RenderContext::initialize()
    {
        ROCKHOPPER_INTERNAL_LOG_INFO("Loading GLAD OpenGL.");
        int status = gladLoadGL();
        ROCKHOPPER_INTERNAL_ASSERT_FATAL(status,"GLAD failed to load OpenGL!");

        const GLubyte* version = glGetString(GL_VERSION);
        ROCKHOPPER_INTERNAL_LOG_INFO("Initialized GLAD OpenGL {}.",version);
    }

    void RenderContext::dispose()
    {
    }

    void RenderContext::refresh()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

} // namespace RockHopper
