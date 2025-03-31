#pragma once

#include "Saber/Core/Timestep.h"
#include "Saber/Events/ApplicationEvent.h"
#include "Saber/Events/MouseEvent.h"
#include "Saber/Renderer/PerspectiveCamera.h"

namespace Saber
{

class PerspectiveCameraController
{
public:
    PerspectiveCameraController(float aspectRatio, bool rotation = false);

    void OnUpdate(Timestep ts);
    void OnEvent(Event &e);

    PerspectiveCamera &GetCamera() { return m_Camera; }
    const PerspectiveCamera &GetCamera() const { return m_Camera; }

    float GetFOV() const { return m_FOV; }
    void SetFOV(float fov)
    {
        m_FOV = fov;
        m_Camera.SetFOV(fov);
    }

    float GetDistance() const { return m_Distance; }
    void SetDistance(float distance) { m_Distance = distance; }

    // 获取相机位置
    const glm::vec3 &GetCameraPosition() const { return m_CameraPosition; }

    // 获取相机旋转
    const glm::vec3 &GetCameraRotation() const { return m_CameraRotation; }

    // 设置相机旋转
    void SetCameraRotation(const glm::vec3 &rotation) { m_CameraRotation = rotation; }

private:
    bool OnMouseScrolled(MouseScrolledEvent &e);
    bool OnWindowResized(WindowResizeEvent &e);

private:
    float m_AspectRatio;
    float m_FOV = 45.0f;      // 视场角，以度为单位
    float m_Distance = 10.0f; // 相机到目标点的距离
    PerspectiveCamera m_Camera;

    bool m_Rotation;

    glm::vec3 m_CameraPosition = {0.0f, 0.0f, -m_Distance};
    glm::vec3 m_CameraRotation = {0.0f, 0.0f, 0.0f}; // Pitch, Yaw, Roll（俯仰角、偏航角、滚转角）
    float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
};

} // namespace Saber