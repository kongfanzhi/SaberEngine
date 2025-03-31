#include "PerspectiveCameraController.h"
#include "Saber/pch.h"

#include "Saber/Core/Input.h"
#include "Saber/Core/KeyCodes.h"

namespace Saber
{

PerspectiveCameraController::PerspectiveCameraController(float aspectRatio, bool rotation)
    : m_AspectRatio(aspectRatio), m_Camera(m_FOV, m_AspectRatio, 0.01f, 100.0f), m_Rotation(rotation)
{
    m_CameraPosition = {0.0f, 0.0f, 0.0f};
}

void PerspectiveCameraController::OnUpdate(Timestep ts)
{
    // ������ת����
    if (m_Rotation)
    {
        if (Input::IsKeyPressed(SB_KEY_Q))
            m_CameraRotation.y -= m_CameraRotationSpeed * ts; // ��ת
        if (Input::IsKeyPressed(SB_KEY_E))
            m_CameraRotation.y += m_CameraRotationSpeed * ts; // ��ת
        if (Input::IsKeyPressed(SB_KEY_R))
            m_CameraRotation.x += m_CameraRotationSpeed * ts; // ����
        if (Input::IsKeyPressed(SB_KEY_F))
            m_CameraRotation.x -= m_CameraRotationSpeed * ts; // �¸�

        // ���Ƹ����Ƕȣ�-89�� �� 89�㣩
        m_CameraRotation.x = std::clamp(m_CameraRotation.x, -89.0f, 89.0f);

        // ȷ��ƫ������-180��180֮��
        if (m_CameraRotation.y > 180.0f)
            m_CameraRotation.y -= 360.0f;
        else if (m_CameraRotation.y < -180.0f)
            m_CameraRotation.y += 360.0f;
    }

    // ֱ��ʹ�����Ǻ�����������ķ�������
    float yawRad = glm::radians(m_CameraRotation.y);
    float pitchRad = glm::radians(m_CameraRotation.x);

    // ����ǰ����
    glm::vec3 forward;
    forward.x = -sin(yawRad) * cos(pitchRad);
    forward.y = sin(pitchRad);
    forward.z = -cos(yawRad) * cos(pitchRad);
    forward = glm::normalize(forward);

    // ���������� (���ǰ����������������)
    glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward));

    // ���������� (�����������ǰ����)
    glm::vec3 up = glm::normalize(glm::cross(forward, right));

    // �����ƶ�����
    float movementSpeed = m_CameraTranslationSpeed * static_cast<float>(ts);
    if (Input::IsKeyPressed(SB_KEY_W))
        m_CameraPosition += forward * movementSpeed;
    if (Input::IsKeyPressed(SB_KEY_S))
        m_CameraPosition -= forward * movementSpeed;
    if (Input::IsKeyPressed(SB_KEY_A))
        m_CameraPosition += right * movementSpeed;
    if (Input::IsKeyPressed(SB_KEY_D))
        m_CameraPosition -= right * movementSpeed;
    if (Input::IsKeyPressed(SB_KEY_SPACE))
        m_CameraPosition += glm::vec3(0.0f, 1.0f, 0.0f) * movementSpeed;
    if (Input::IsKeyPressed(SB_KEY_LEFT_SHIFT))
        m_CameraPosition -= glm::vec3(0.0f, 1.0f, 0.0f) * movementSpeed;

    // �����������ͼ����
    glm::mat4 viewMatrix = glm::lookAt(m_CameraPosition, m_CameraPosition + forward, up);
    m_Camera.SetViewMatrix(viewMatrix);
}

void PerspectiveCameraController::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseScrolledEvent>(SB_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(SB_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
}

bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent &e)
{
    // ʹ�������ֵ���FOV
    m_FOV -= e.GetYOffset() * 2.0f;
    m_FOV = std::max(std::min(m_FOV, 90.0f), 10.0f); // ����FOV��Χ��10-90��֮��
    m_Camera.SetPerspective(m_FOV, m_AspectRatio, 0.01f, 100.0f);
    return false;
}

bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent &e)
{
    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    m_Camera.SetPerspective(m_FOV, m_AspectRatio, 0.01f, 100.0f);
    return false;
}

} // namespace Saber