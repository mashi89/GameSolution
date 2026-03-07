#pragma once
#include "GameState.h"

class MainMenuState : public GameState
{
public:
    void OnEnter()  override;
    void OnExit()   override;
    void OnResume() override;
    void Update(float deltaTime) override;
    void Render()   override;

    GameStateType GetType() const override { return GameStateType::MainMenu; }
    std::string   GetName() const override { return "MainMenu"; }

private:
    int  m_SelectedOption = 0;
    bool m_NeedsRedraw    = true;

    static const int OPTION_START = 0;
    static const int OPTION_EXIT  = 1;
    static const int OPTION_COUNT = 2;
};
