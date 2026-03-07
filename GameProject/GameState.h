#pragma once
#include <string>

enum class GameStateType { None, MainMenu, Playing, Paused, GameOver };

class GameState
{
public:
    virtual ~GameState() = default;
    virtual void OnEnter() = 0;
    virtual void OnExit()  = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render() = 0;
    virtual GameStateType GetType() const = 0;
    virtual std::string   GetName() const = 0;
};
