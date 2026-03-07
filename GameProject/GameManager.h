#pragma once
#include "GameState.h"
#include <memory>
#include <stack>

class GameManager
{
public:
    static GameManager& GetInstance();
    void PushState(std::unique_ptr<GameState> state);
    void PopState();
    void ChangeState(std::unique_ptr<GameState> state);
    void Update(float deltaTime);
    void Render();
    GameState* GetCurrentState();

private:
    GameManager() = default;
    std::stack<std::unique_ptr<GameState>> m_States;
};

// inline implementations
inline GameManager& GameManager::GetInstance()
{
    static GameManager instance;
    return instance;
}

inline void GameManager::PushState(std::unique_ptr<GameState> state)
{
    if (state)
    {
        state->OnEnter();
        m_States.push(std::move(state));
    }
}

inline void GameManager::PopState()
{
    if (!m_States.empty())
    {
        m_States.top()->OnExit();
        m_States.pop();
    }
}

inline void GameManager::ChangeState(std::unique_ptr<GameState> state)
{
    PopState();
    PushState(std::move(state));
}

inline void GameManager::Update(float deltaTime)
{
    if (!m_States.empty())
        m_States.top()->Update(deltaTime);
}

inline void GameManager::Render()
{
    if (!m_States.empty())
        m_States.top()->Render();
}

inline GameState* GameManager::GetCurrentState()
{
    return m_States.empty() ? nullptr : m_States.top().get();
}

