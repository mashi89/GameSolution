#include "Application.h"
#include "GameManager.h"
#include "MainMenuState.h"
#include <chrono>
#include <memory>
#include <thread>

Application::Application()
    : m_Running(false)
    , m_DeltaTime(0.0f)
    , m_LastTime(std::chrono::high_resolution_clock::now())
{
}

Application::~Application()
{
    Shutdown();
}

bool Application::Initialize()
{
    m_LastTime = std::chrono::high_resolution_clock::now();
    m_Running  = true;

    // Push the main menu as the first game state
    GameManager::GetInstance().PushState(std::make_unique<MainMenuState>());
    return true;
}

void Application::Run()
{
    while (m_Running)
    {
        // Compute delta time
        auto now      = std::chrono::high_resolution_clock::now();
        m_DeltaTime   = std::chrono::duration<float>(now - m_LastTime).count();
        m_LastTime    = now;

        Update(m_DeltaTime);
        Render();

        // Cap to ~60 fps to avoid burning CPU
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Application::Shutdown()
{
    m_Running = false;
}

void Application::Update(float deltaTime)
{
    GameManager::GetInstance().Update(deltaTime);

    // When the state stack is empty (e.g. "Exit Game" was chosen) stop the loop
    if (GameManager::GetInstance().GetCurrentState() == nullptr)
        m_Running = false;
}

void Application::Render()
{
    GameManager::GetInstance().Render();
}

