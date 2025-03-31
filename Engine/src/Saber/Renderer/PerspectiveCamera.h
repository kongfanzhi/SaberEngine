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

    // 使用默认的近远平面
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
        // 使用自定义的投影矩阵，解决深度精度问题
        float tanHalfFovy = tan(glm::radians(m_FOV) / 2.0f);
        float aspectRatio = m_AspectRatio;

        // 注意：glm是列优先矩阵存储
        m_Projection = glm::mat4(1.0f);

        // 设置x和y缩放因子
        float f = 1.0f / tanHalfFovy;
        m_Projection[0][0] = f / aspectRatio;
        m_Projection[1][1] = f;

        // 设置z映射，对应透视矩阵的第三行第三列和第四行第三列
        // 默认GLM使用这个公式：-(far+near)/(far-near), -2*far*near/(far-near)
        // 确保投影矩阵计算是正确的
        float A = -(m_FarClip + m_NearClip) / (m_FarClip - m_NearClip);
        float B = -2.0f * m_FarClip * m_NearClip / (m_FarClip - m_NearClip);

        m_Projection[2][2] = A;
        m_Projection[3][2] = B;

        // 设置透视除法
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