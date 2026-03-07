#include "GameManager.h"

GameManager& GameManager::GetInstance()
{
    static GameManager instance;
    return instance;
}

void GameManager::PushState(std::unique_ptr<GameState> state)
{
    if (state)
    {
        state->OnEnter();
        m_States.push(std::move(state));
    }
}

void GameManager::PopState()
{
    if (!m_States.empty())
    {
        m_States.top()->OnExit();
        m_States.pop();
        // Notify the state that is now on top (if any) that it is active again
        if (!m_States.empty())
            m_States.top()->OnResume();
    }
}

void GameManager::ChangeState(std::unique_ptr<GameState> state)
{
    PopState();
    PushState(std::move(state));
}

void GameManager::Update(float deltaTime)
{
    if (!m_States.empty())
        m_States.top()->Update(deltaTime);
}

void GameManager::Render()
{
    if (!m_States.empty())
        m_States.top()->Render();
}

GameState* GameManager::GetCurrentState()
{
    return m_States.empty() ? nullptr : m_States.top().get();
}