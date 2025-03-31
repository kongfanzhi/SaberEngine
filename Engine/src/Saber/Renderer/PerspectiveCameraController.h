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

    // ��ȡ���λ��
    const glm::vec3 &GetCameraPosition() const { return m_CameraPosition; }

    // ��ȡ�����ת
    const glm::vec3 &GetCameraRotation() const { return m_CameraRotation; }

    // ���������ת
    void SetCameraRotation(const glm::vec3 &rotation) { m_CameraRotation = rotation; }

private:
    bool OnMouseScrolled(MouseScrolledEvent &e);
    bool OnWindowResized(WindowResizeEvent &e);

private:
    float m_AspectRatio;
    float m_FOV = 45.0f;      // �ӳ��ǣ��Զ�Ϊ��λ
    float m_Distance = 10.0f; // �����Ŀ���ľ���
    PerspectiveCamera m_Camera;

    bool m_Rotation;

    glm::vec3 m_CameraPosition = {0.0f, 0.0f, -m_Distance};
    glm::vec3 m_CameraRotation = {0.0f, 0.0f, 0.0f}; // Pitch, Yaw, Roll�������ǡ�ƫ���ǡ���ת�ǣ�
    float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
};

} // namespace Saber