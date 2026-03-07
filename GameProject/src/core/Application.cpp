#include "include/core/Application.h"
#include "include/utils/Logger.h"

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
    Logger::GetInstance().Info("Application initialized.");
    return true;
}

void Application::Run()
{
    while (m_Running)
    {
        Update();
        Render();
    }
}

void Application::Shutdown()
{
    m_Running = false;
}

void Application::Update()
{
}

void Application::Render()
{
}
