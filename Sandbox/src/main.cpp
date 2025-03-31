#include <glad/glad.h>

#include "Saber/Core/Application.h"
#include "Saber/Core/Window.h"
#include "Saber/Saber.h"
#include <GLFW/glfw3.h>

#include "ExampleLayer.h"
#include "Saber/Core/EntryPoint.h"
#include "Sandbox2D.h"
#include "Sandbox3D.h"

class SandBox : public Saber::Application
{
public:
    SandBox() { PushLayer(new Sandbox3D()); }

    ~SandBox() {}
};

Saber::Application *Saber::CreateApplication() { return new SandBox(); }