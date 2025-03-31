#pragma once

namespace Saber
{
class GraphicsContext
{
public:
    virtual void Init() = 0;
    virtual void SwapBuffer() = 0;

    static Scope<GraphicsContext> Create(void *window);
};
} // namespace Saber
