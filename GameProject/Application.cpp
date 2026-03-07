#include "Application.h"

Application::Application()
    : m_Running(false)
{
}

Application::~Application()
{
    Shutdown();
}

bool Application::Initialize()
{
    m_Running = true;
    return true;
}

void Application::Run()
{
    while (m_Running)
    {
        Update();
        Render();
        m_Running = false;
    }
}

void Application::Shutdown()
{
    m_Running = false;
}

void Application::Update() {}
void Application::Render() {}
