#pragma once

#include "GLFW/glfw3.h"
#include "Saber/Core/Window.h"
#include "Saber/Renderer/GraphicsContext.h"

namespace Saber
{
class WindowsWindow : public Window
{
public:
    WindowsWindow(const WindowProps &props);
    virtual ~WindowsWindow() override;

    virtual void OnUpdate() override;

    virtual unsigned int GetWidth() const override { return m_Data.Width; };
    virtual unsigned int GetHeight() const override { return m_Data.Height; };

    virtual void SetEventCallback(const EventCallbackFn &callback) override { m_Data.EventCallback = callback; };
    virtual void SetVSync(bool enabled) override;
    virtual bool IsVSync() const override;

    inline virtual void *GetNativeWindow() const override { return m_Window; }

private:
    virtual void Init(const WindowProps &props);
    virtual void Shutdown();

private:
    GLFWwindow *m_Window;
    Scope<GraphicsContext> m_Context;

    struct WindowData
    {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;
        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};
} // namespace Saber
