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
