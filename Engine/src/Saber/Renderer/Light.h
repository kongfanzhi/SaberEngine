#pragma once

#include "glm/glm.hpp"

namespace Saber
{
// 方向光结构体
struct DirectionalLight
{
    glm::vec3 Direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    glm::vec3 Ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 Diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 Specular = glm::vec3(1.0f, 1.0f, 1.0f);
};

// 点光源结构体
struct PointLight
{
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 Ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 Diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 Specular = glm::vec3(1.0f, 1.0f, 1.0f);

    float Constant = 1.0f;
    float Linear = 0.09f;
    float Quadratic = 0.032f;
};

// 材质结构体
struct Material
{
    glm::vec3 Ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 Specular = glm::vec3(0.5f, 0.5f, 0.5f);
    float Shininess = 32.0f;
};

// 光照管理器
class LightManager
{
public:
    static void SetDirectionalLight(const DirectionalLight &light);
    static void AddPointLight(const PointLight &light);
    static void ClearPointLights();
    static void SetMaterial(const Material &material);

    static const DirectionalLight &GetDirectionalLight() { return s_DirectionalLight; }
    static const std::vector<PointLight> &GetPointLights() { return s_PointLights; }
    static const Material &GetMaterial() { return s_Material; }

    static constexpr uint32_t MaxPointLights = 4;

private:
    static DirectionalLight s_DirectionalLight;
    static std::vector<PointLight> s_PointLights;
    static Material s_Material;
};
} // namespace Saber