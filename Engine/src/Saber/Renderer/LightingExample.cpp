#include "LightingExample.h"
#include "Saber/pch.h"

namespace Saber
{
void LightingExample::SetupDefaultLighting()
{
    SetupDirectionalLight();
    SetupPointLights();
    SetupMaterial();
}

void LightingExample::SetupSceneLights()
{
    // 清除现有点光源
    LightManager::ClearPointLights();

    // 设置方向光（模拟太阳光）
    DirectionalLight dirLight;
    dirLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    dirLight.Ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    dirLight.Diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    dirLight.Specular = glm::vec3(0.8f, 0.8f, 0.8f);
    LightManager::SetDirectionalLight(dirLight);

    // 添加几个点光源
    PointLight pointLight1;
    pointLight1.Position = glm::vec3(2.0f, 2.0f, 2.0f);
    pointLight1.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    pointLight1.Diffuse = glm::vec3(0.8f, 0.2f, 0.2f); // 红色光
    pointLight1.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLight1.Constant = 1.0f;
    pointLight1.Linear = 0.09f;
    pointLight1.Quadratic = 0.032f;
    LightManager::AddPointLight(pointLight1);

    PointLight pointLight2;
    pointLight2.Position = glm::vec3(-2.0f, 2.0f, -2.0f);
    pointLight2.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    pointLight2.Diffuse = glm::vec3(0.2f, 0.8f, 0.2f); // 绿色光
    pointLight2.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLight2.Constant = 1.0f;
    pointLight2.Linear = 0.09f;
    pointLight2.Quadratic = 0.032f;
    LightManager::AddPointLight(pointLight2);

    PointLight pointLight3;
    pointLight3.Position = glm::vec3(0.0f, 3.0f, 0.0f);
    pointLight3.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    pointLight3.Diffuse = glm::vec3(0.2f, 0.2f, 0.8f); // 蓝色光
    pointLight3.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLight3.Constant = 1.0f;
    pointLight3.Linear = 0.09f;
    pointLight3.Quadratic = 0.032f;
    LightManager::AddPointLight(pointLight3);

    // 设置材质
    Material material;
    material.Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    material.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    material.Specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material.Shininess = 64.0f;
    LightManager::SetMaterial(material);
}

void LightingExample::RenderLitCubes(const PerspectiveCamera &camera, const glm::mat4 &viewMatrix)
{
    // 计算相机位置（从视图矩阵的逆矩阵中提取）
    glm::mat4 invView = glm::inverse(viewMatrix);
    glm::vec3 cameraPos = glm::vec3(invView[3]);

    // 开始光照渲染场景
    Renderer3D::BeginSceneWithLighting(camera, viewMatrix, cameraPos);

    // 渲染一些立方体来展示光照效果
    // 中心立方体
    Renderer3D::DrawCube(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));

    // 围绕中心的立方体
    for (int i = 0; i < 8; i++)
    {
        float angle = i * 45.0f;
        float x = 3.0f * cos(glm::radians(angle));
        float z = 3.0f * sin(glm::radians(angle));

        glm::vec3 position(x, 0.0f, z);
        glm::vec3 rotation(0.0f, angle, 0.0f);

        // 不同颜色的立方体
        glm::vec4 color;
        switch (i % 3)
        {
        case 0:
            color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);
            break; // 红色调
        case 1:
            color = glm::vec4(0.5f, 1.0f, 0.5f, 1.0f);
            break; // 绿色调
        case 2:
            color = glm::vec4(0.5f, 0.5f, 1.0f, 1.0f);
            break; // 蓝色调
        }

        Renderer3D::DrawRotatedCube(position, glm::vec3(0.8f), rotation, color);
    }

    // 地面
    Renderer3D::DrawCube(glm::vec3(0.0f, -2.0f, 0.0f), glm::vec3(10.0f, 0.1f, 10.0f),
                         glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

    Renderer3D::EndScene();
}

void LightingExample::SetupDirectionalLight()
{
    DirectionalLight dirLight;
    dirLight.Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    dirLight.Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    dirLight.Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    dirLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
    LightManager::SetDirectionalLight(dirLight);
}

void LightingExample::SetupPointLights()
{
    LightManager::ClearPointLights();

    PointLight pointLight;
    pointLight.Position = glm::vec3(2.0f, 2.0f, 2.0f);
    pointLight.Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    pointLight.Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    pointLight.Specular = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLight.Constant = 1.0f;
    pointLight.Linear = 0.09f;
    pointLight.Quadratic = 0.032f;
    LightManager::AddPointLight(pointLight);
}

void LightingExample::SetupMaterial()
{
    Material material;
    material.Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    material.Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    material.Specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material.Shininess = 32.0f;
    LightManager::SetMaterial(material);
}
} // namespace Saber