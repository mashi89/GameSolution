#pragma once
#include "GameState.h"

class PlayingState : public GameState
{
public:
    void OnEnter()  override;
    void OnExit()   override;
    void Update(float deltaTime) override;
    void Render()   override;

    GameStateType GetType() const override { return GameStateType::Playing; }
    std::string   GetName() const override { return "Playing"; }

private:
    bool m_NeedsRedraw = true;
};
