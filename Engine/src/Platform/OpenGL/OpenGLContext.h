#pragma once

#include "Saber/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Saber
{
class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow *windowHandle);
    virtual void Init() override;
    virtual void SwapBuffer() override;

private:
    GLFWwindow *m_WindowHandle;
};

} // namespace Saber
