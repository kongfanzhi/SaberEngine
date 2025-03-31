#pragma once

#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Saber
{
class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
    {
        RecalculateProjection();
        m_ViewMatrix = glm::mat4(1.0f);
        m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
    }

    // ʹ��Ĭ�ϵĽ�Զƽ��
    PerspectiveCamera(float fov, float aspectRatio)
        : m_FOV(fov), m_AspectRatio(aspectRatio), m_NearClip(0.1f), m_FarClip(100.0f)
    {
        RecalculateProjection();
        m_ViewMatrix = glm::mat4(1.0f);
        m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
    }

    void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip)
    {
        m_FOV = fov;
        m_AspectRatio = aspectRatio;
        m_NearClip = nearClip;
        m_FarClip = farClip;
        RecalculateProjection();
        m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
    }

    void SetViewMatrix(const glm::mat4 &viewMatrix)
    {
        m_ViewMatrix = viewMatrix;
        m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
    }

    const glm::mat4 &GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4 &GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    void SetFOV(float fov)
    {
        m_FOV = fov;
        RecalculateProjection();
        m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
    }

    void SetAspectRatio(float aspectRatio)
    {
        m_AspectRatio = aspectRatio;
        RecalculateProjection();
        m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
    }

    void SetNearClip(float nearClip)
    {
        m_NearClip = nearClip;
        RecalculateProjection();
        m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
    }

    void SetFarClip(float farClip)
    {
        m_FarClip = farClip;
        RecalculateProjection();
        m_ViewProjectionMatrix = m_Projection * m_ViewMatrix;
    }

    float GetFOV() const { return m_FOV; }
    float GetAspectRatio() const { return m_AspectRatio; }
    float GetNearClip() const { return m_NearClip; }
    float GetFarClip() const { return m_FarClip; }

private:
    void RecalculateProjection()
    {
        // ʹ���Զ����ͶӰ���󣬽����Ⱦ�������
        float tanHalfFovy = tan(glm::radians(m_FOV) / 2.0f);
        float aspectRatio = m_AspectRatio;

        // ע�⣺glm�������Ⱦ���洢
        m_Projection = glm::mat4(1.0f);

        // ����x��y��������
        float f = 1.0f / tanHalfFovy;
        m_Projection[0][0] = f / aspectRatio;
        m_Projection[1][1] = f;

        // ����zӳ�䣬��Ӧ͸�Ӿ���ĵ����е����к͵����е�����
        // Ĭ��GLMʹ�������ʽ��-(far+near)/(far-near), -2*far*near/(far-near)
        // ȷ��ͶӰ�����������ȷ��
        float A = -(m_FarClip + m_NearClip) / (m_FarClip - m_NearClip);
        float B = -2.0f * m_FarClip * m_NearClip / (m_FarClip - m_NearClip);

        m_Projection[2][2] = A;
        m_Projection[3][2] = B;

        // ����͸�ӳ���
        m_Projection[2][3] = -1.0f;
        m_Projection[3][3] = 0.0f;
    }

private:
    float m_FOV = 45.0f;
    float m_AspectRatio = 1.778f; // 16:9
    float m_NearClip = 0.1f;
    float m_FarClip = 1000.0f;

    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ViewProjectionMatrix;
};
} // namespace Saber