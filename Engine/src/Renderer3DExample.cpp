#include "Saber/Renderer/PerspectiveCamera.h"
#include "Saber/Renderer/RenderCommand.h"
#include "Saber/Renderer/Renderer3D.h"
#include "glm/gtc/matrix_transform.hpp"

using namespace Saber;

// 示例代码：展示如何使用Renderer3D
void Renderer3DExample()
{
    // 初始化渲染器
    Renderer3D::Init();

    // 创建透视摄像机
    PerspectiveCamera camera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);

    // 创建摄像机变换矩阵（位置和旋转）
    glm::mat4 cameraTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

    // 加载纹理
    Ref<Texture2D> texture = Texture2D::Create("assets/textures/example.png");

    // 渲染循环中的代码
    {
        // 清除屏幕
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        RenderCommand::Clear();

        // 开始场景渲染
        Renderer3D::BeginScene(camera, cameraTransform);

        // 绘制一些立方体

        // 1. 使用颜色绘制立方体
        Renderer3D::DrawCube({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}); // 红色立方体

        // 2. 使用纹理绘制立方体
        Renderer3D::DrawCube({2.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, texture);

        // 3. 绘制旋转的立方体（颜色）
        Renderer3D::DrawRotatedCube({-2.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {45.0f, 45.0f, 0.0f},
                                    {0.0f, 1.0f, 0.0f, 1.0f}); // 绿色旋转立方体

        // 4. 绘制旋转的立方体（纹理）
        Renderer3D::DrawRotatedCube({0.0f, 2.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 45.0f, 45.0f}, texture, 2.0f);

        // 结束场景渲染
        Renderer3D::EndScene();
    }

    // 获取统计信息
    Renderer3D::Statistics stats = Renderer3D::GetStats();

    // 输出统计信息
    printf("Renderer3D Stats:\n");
    printf("  Draw Calls: %d\n", stats.DrawCalls);
    printf("  Cube Count: %d\n", stats.CubeCount);

    // 关闭渲染器
    Renderer3D::Shutdown();
}