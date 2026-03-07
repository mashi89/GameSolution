#include "include/game/GameManager.h"

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
    }
}

void GameManager::ChangeState(std::unique_ptr<GameState> state)
{
    if (!m_States.empty())
    {
        m_States.top()->OnExit();
        m_States.pop();
    }
    if (state)
    {
        state->OnEnter();
        m_States.push(std::move(state));
    }
}

void GameManager::Update(float deltaTime)
{
    if (!m_States.empty())
    {
        m_States.top()->Update(deltaTime);
    }
}

void GameManager::Render()
{
    if (!m_States.empty())
    {
        m_States.top()->Render();
    }
}

GameState* GameManager::GetCurrentState()
{
    if (m_States.empty())
        return nullptr;
    return m_States.top().get();
}
