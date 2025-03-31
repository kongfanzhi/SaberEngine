#pragma once

#include "Saber/Core/Core.h"
#include "Saber/Core/Window.h"
#include "Saber/Events/ApplicationEvent.h"
#include <memory>

#include "Saber/Core/LayerStack.h"
#include "Saber/ImGui/ImGuiLayer.h"

namespace Saber
{

class Application
{
public:
    Application(const std::string &name = "Saber App");
    virtual ~Application();

    void Run();
    void OnEvent(Event &e);

    void PushLayer(Layer *layer);
    void PushOverlay(Layer *layer);

    inline Window &GetWindow() { return *m_Window; }
    inline static Application &Get() { return *s_Instance; }

private:
    bool OnWindowClose(WindowCloseEvent &e);
    bool OnWindowResize(WindowResizeEvent &e);

    std::unique_ptr<Window> m_Window;
    ImGuiLayer *m_ImGuiLayer;
    bool m_Running = true;
    LayerStack m_LayerStack;

    float m_LastFrameTime = 0.0f;
    bool m_Minimized = false;

private:
    static Application *s_Instance;
};

Application *CreateApplication();
} // namespace Saber