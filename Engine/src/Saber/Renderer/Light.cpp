#include "Light.h"
#include "Saber/pch.h"

namespace Saber
{
// 静态成员变量定义
DirectionalLight LightManager::s_DirectionalLight;
std::vector<PointLight> LightManager::s_PointLights;
Material LightManager::s_Material;

void LightManager::SetDirectionalLight(const DirectionalLight &light) { s_DirectionalLight = light; }

void LightManager::AddPointLight(const PointLight &light)
{
    if (s_PointLights.size() < MaxPointLights)
    {
        s_PointLights.push_back(light);
    }
}

void LightManager::ClearPointLights() { s_PointLights.clear(); }

void LightManager::SetMaterial(const Material &material) { s_Material = material; }
} // namespace Saber