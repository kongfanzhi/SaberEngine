#pragma once

#include "Light.h"
#include "PerspectiveCamera.h"
#include "Renderer3D.h"

namespace Saber
{
// ����ʾ����
class LightingExample
{
public:
    static void SetupDefaultLighting();
    static void SetupSceneLights();
    static void RenderLitCubes(const PerspectiveCamera &camera, const glm::mat4 &viewMatrix);

private:
    static void SetupDirectionalLight();
    static void SetupPointLights();
    static void SetupMaterial();
};
} // namespace Saber