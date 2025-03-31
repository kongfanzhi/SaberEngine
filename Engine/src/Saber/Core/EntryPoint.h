#pragma once

#include "Application.h"
#include "Log.h"

extern Saber::Application *Saber::CreateApplication();

int main(int argc, char **argv)
{
    Saber::Log::Init();
    SB_CORE_WARN("CoreLog");
    SB_ERROR("ClientLog");

    auto app = Saber::CreateApplication();
    app->Run();
    delete app;
}
