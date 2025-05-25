#pragma once

#include "Light.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"

namespace Saber
{
class Camera;

class Renderer3D
{
public:
    static void Init();
    static void Shutdown();
    static void BeginScene(const Camera &camera, const glm::mat4 &transform = glm::mat4(1.0f));
    static void BeginSceneWithLighting(const Camera &camera, const glm::mat4 &transform = glm::mat4(1.0f),
                                       const glm::vec3 &viewPos = glm::vec3(0.0f));
    static void EndScene();
    static void Flush();

    // 光照渲染模式切换
    static void EnableLighting(bool enable = true);
    static bool IsLightingEnabled();

    // 基本立方体绘制
    static void DrawCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec4 &color);
    static void DrawCube(const glm::vec3 &position, const glm::vec3 &size, const Ref<Texture2D> &texture,
                         float tilingFactor = 1.0f, const glm::vec4 &tintColor = glm::vec4(1.0f));

    // 旋转立方体绘制
    static void DrawRotatedCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec3 &rotation,
                                const glm::vec4 &color);
    static void DrawRotatedCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec3 &rotation,
                                const Ref<Texture2D> &texture, float tilingFactor = 1.0f,
                                const glm::vec4 &tintColor = glm::vec4(1.0f));

    // 统计信息
    struct Statistics
    {
        uint32_t DrawCalls = 0;
        uint32_t CubeCount = 0;

        uint32_t GetTotalVertexCount() { return CubeCount * 8; }
        uint32_t GetTotalIndexCount() { return CubeCount * 36; }
    };
    static void ResetStats();
    static Statistics GetStats();

private:
    static void FlushAndReset();
};
} // namespace Saber