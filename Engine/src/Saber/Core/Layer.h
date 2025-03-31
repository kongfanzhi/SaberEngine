#pragma once

#include "Saber/Core/Core.h"
#include "Saber/Core/Timestep.h"
#include "Saber/Events/Event.h"

namespace Saber
{
class SABER_API Layer
{
public:
    Layer(const std::string &name = "Layer");
    virtual ~Layer();

    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep ts) {}
    virtual void OnImGuiRender() {}
    virtual void OnEvent(Event &event) {}

    inline const std::string &GetName() const { return m_DebugName; }

private:
    std::string m_DebugName;
};
} // namespace Saber
