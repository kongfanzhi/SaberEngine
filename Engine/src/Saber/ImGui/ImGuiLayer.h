#pragma once

#include "Saber/Core/Layer.h"
#include "Saber/Events/ApplicationEvent.h"
#include "Saber/Events/KeyEvent.h"
#include "Saber/Events/MouseEvent.h"

namespace Saber
{
class ImGuiLayer : public Layer
{
public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void Begin();
    void End();

private:
    float m_Time = 0.0f;
};
} // namespace Saber
