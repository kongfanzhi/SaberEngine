#include "Saber/Core/Window.h"
#include "Saber/pch.h"

#ifdef SB_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Saber
{

Scope<Window> Window::Create(const WindowProps &props)
{
#ifdef SB_PLATFORM_WINDOWS
    return CreateScope<WindowsWindow>(props);
#else
    SB_CORE_ASSERT(false, "Unknown platform!");
    return nullptr;
#endif
}

} // namespace Saber