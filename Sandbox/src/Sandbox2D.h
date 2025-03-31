#pragma once

#include "Saber/Saber.h"

class Sandbox2D : public Saber::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Saber::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Saber::Event &e) override;

private:
    Saber::OrthographicCameraController m_CameraController;

    // Temp
    Saber::Ref<Saber::VertexArray> m_SquareVA;
    Saber::Ref<Saber::Shader> m_FlatColorShader;

    Saber::Ref<Saber::Texture2D> m_CheckerboardTexture;

    glm::vec4 m_SquareColor = {0.2f, 0.3f, 0.8f, 1.0f};
};