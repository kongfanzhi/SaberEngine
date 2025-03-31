#pragma once

#include "Saber/Renderer/RendererAPI.h"
#include <glad/glad.h>

namespace Saber
{
class OpenGLRendererAPI : public RendererAPI
{
public:
    virtual void Init() override;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    virtual void SetClearColor(const glm::vec4 &color) override;
    virtual void Clear() override;

    virtual void DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount) override;

    /* // 设置背面剔除
    virtual void SetCullFace(bool enable) override;
    // 设置深度测试
    virtual void SetDepthTest(bool enable) override; */
};
} // namespace Saber