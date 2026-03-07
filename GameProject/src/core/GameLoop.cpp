#include "include/core/GameLoop.h"

GameLoop::GameLoop()
    : m_Running(false)
    , m_DeltaTime(0.0f)
    , m_LastTime(std::chrono::high_resolution_clock::now())
{
}

void GameLoop::Start()
{
    m_Running  = true;
    m_LastTime = std::chrono::high_resolution_clock::now();
}

void GameLoop::Stop()
{
    m_Running = false;
}
