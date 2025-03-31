#include <glad/glad.h>

#include "OpenGLContext.h"
#include "Saber/pch.h"

#include <GL/GL.h>
#include <GLFW/glfw3.h>

namespace Saber
{
OpenGLContext::OpenGLContext(GLFWwindow *windowHandle) : m_WindowHandle(windowHandle)
{
    SB_CORE_ASSERT(windowHandle, "Window handle is null!");
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_WindowHandle);
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    SB_CORE_ASSERT(status, "Failed to initialize Glad!");

    SB_CORE_INFO("OpenGL Info:");
    SB_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
    SB_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
    SB_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
}

void OpenGLContext::SwapBuffer() { glfwSwapBuffers(m_WindowHandle); }
} // namespace Saber
