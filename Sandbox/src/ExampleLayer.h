#pragma once
#include "Saber/Saber.h"

class ExampleLayer : public Saber::Layer
{

public:
    ExampleLayer();
    virtual ~ExampleLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Saber::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Saber::Event &e) override;

private:
    Saber::ShaderLibrary m_ShaderLibrary;
    Saber::Ref<Saber::Shader> m_Shader;
    Saber::Ref<Saber::VertexArray> m_VertexArray;

    Saber::Ref<Saber::Shader> m_FlatColorShader;
    Saber::Ref<Saber::VertexArray> m_SquareVA;

    // Saber::Ref<Saber::Texture2D> m_Texture, m_ChernoLogoTexture;

    Saber::OrthographicCameraController m_CameraController;
    glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};
};
