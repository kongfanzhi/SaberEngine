#include "Sandbox3D.h"

#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGl/OpenGLShader.h"
#include "imgui/imgui.h"

#include "Saber/Renderer/LightingExample.h"
#include "Saber/Renderer/Renderer3D.h"

#include <filesystem>

#include <glad/glad.h>

Sandbox3D::Sandbox3D() : Layer("Sandbox3D"), m_CameraController(1280.0f / 720.0f, true) {}

void Sandbox3D::OnAttach()
{
    SB_PROFILE_FUNCTION();

    // 输出当前工作目录
    std::filesystem::path currentPath = std::filesystem::current_path();
    printf("Current working directory: %s\n", currentPath.string().c_str());

    // 初始化3D渲染器
    Saber::Renderer3D::Init();
    Saber::LightingExample::SetupSceneLights();

    m_CheckerboardTexture = Saber::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox3D::OnDetach()
{
    SB_PROFILE_FUNCTION();
    // 关闭3D渲染器
    Saber::Renderer3D::Shutdown();
}

void Sandbox3D::OnUpdate(Saber::Timestep ts)
{
    SB_PROFILE_FUNCTION();

    m_CameraController.OnUpdate(ts);
    // Render
    Saber::Renderer3D::ResetStats();
    {
        SB_PROFILE_SCOPE("Renderer Prep");
        Saber::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.0f});
        Saber::RenderCommand::Clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        SB_PROFILE_SCOPE("Renderer Draw");
        // 开始场景渲染
        // 使用相机的视图矩阵作为变换矩阵，不使用逆矩阵
        // Saber::Renderer3D::BeginScene(m_CameraController.GetCamera(),
        // m_CameraController.GetCamera().GetViewMatrix());
        Saber::LightingExample::RenderLitCubes(m_CameraController.GetCamera(),
                                               m_CameraController.GetCamera().GetViewMatrix());
        // 绘制一些立方体 - 调整z坐标使其在裁剪平面内

        /* // 1. 使用颜色绘制立方体
        Saber::Renderer3D::DrawCube({0.0f, 0.0f, -5.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}); // 红色立方体

        // 2. 使用纹理绘制立方体
        Saber::Renderer3D::DrawCube({2.0f, 0.0f, -8.0f}, {1.0f, 1.0f, 1.0f}, m_CheckerboardTexture);

        // 3. 绘制旋转的立方体（颜色）
        Saber::Renderer3D::DrawRotatedCube({-2.0f, 0.0f, -12.0f}, {1.0f, 1.0f, 1.0f}, {45.0f, 45.0f, 0.0f},
                                           {0.0f, 1.0f, 0.0f, 1.0f}); // 绿色旋转立方体

        // 4. 绘制旋转的立方体（纹理）
        Saber::Renderer3D::DrawRotatedCube({0.0f, 2.0f, -15.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 45.0f, 45.0f},
                                           m_CheckerboardTexture, 2.0f);

        // 结束场景渲染
        Saber::Renderer3D::EndScene(); */
    }
}

void Sandbox3D::OnImGuiRender()
{
    SB_PROFILE_FUNCTION();

    ImGui::Begin("相机设置");

    // FOV控制
    float fov = m_CameraController.GetFOV();
    if (ImGui::SliderFloat("视场角", &fov, 10.0f, 90.0f))
    {
        m_CameraController.SetFOV(fov);
    }

    // 距离控制
    float distance = m_CameraController.GetDistance();
    if (ImGui::SliderFloat("距离", &distance, 1.0f, 20.0f))
    {
        m_CameraController.SetDistance(distance);
    }

    // 添加相机旋转控制
    glm::vec3 rotation = m_CameraController.GetCameraRotation();
    bool rotationChanged = false;

    rotationChanged |= ImGui::SliderFloat("俯仰角", &rotation.x, -89.0f, 89.0f);
    rotationChanged |= ImGui::SliderFloat("偏航角", &rotation.y, -180.0f, 180.0f);

    if (rotationChanged)
    {
        m_CameraController.SetCameraRotation(rotation);
    }

    // 添加相机位置显示
    glm::vec3 position = m_CameraController.GetCameraPosition();
    ImGui::Text("位置: %.2f, %.2f, %.2f", position.x, position.y, position.z);

    ImGui::End();

    ImGui::Begin("Renderer3D Stats");
    auto stats = Saber::Renderer3D::GetStats();
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Cube Count: %d", stats.CubeCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    ImGui::End();
}

void Sandbox3D::OnEvent(Saber::Event &e)
{
    SB_PROFILE_FUNCTION();
    m_CameraController.OnEvent(e);
}
