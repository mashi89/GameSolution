#include "MainMenuState.h"
#include "PlayingState.h"
#include "GameManager.h"
#include "ConsoleUtils.h"
#include <iostream>
#include <memory>

void MainMenuState::OnEnter()
{
    m_SelectedOption = OPTION_START;
    m_NeedsRedraw    = true;
    ConsoleUtils::ShowCursor(false);
}

void MainMenuState::OnExit()
{
    ConsoleUtils::ShowCursor(true);
}

void MainMenuState::OnResume()
{
    // Called when PlayingState is popped and the menu becomes visible again
    m_SelectedOption = OPTION_START;
    m_NeedsRedraw    = true;
    ConsoleUtils::ShowCursor(false);
}

void MainMenuState::Update(float /*deltaTime*/)
{
    if (!ConsoleUtils::KeyAvailable())
        return;

    int key = ConsoleUtils::ReadKey();

    switch (key)
    {
    case ConsoleUtils::KEY_ARROW_UP:
        m_SelectedOption = (m_SelectedOption - 1 + OPTION_COUNT) % OPTION_COUNT;
        m_NeedsRedraw = true;
        break;

    case ConsoleUtils::KEY_ARROW_DOWN:
        m_SelectedOption = (m_SelectedOption + 1) % OPTION_COUNT;
        m_NeedsRedraw = true;
        break;

    case ConsoleUtils::KEY_ENTER:
        if (m_SelectedOption == OPTION_START)
        {
            GameManager::GetInstance().PushState(std::make_unique<PlayingState>());
        }
        else if (m_SelectedOption == OPTION_EXIT)
        {
            // Pop this state; an empty stack signals the application to exit.
            // Do not access any members after PopState – this object may be destroyed.
            GameManager::GetInstance().PopState();
            return;
        }
        break;

    default:
        break;
    }
}

void MainMenuState::Render()
{
    if (!m_NeedsRedraw)
        return;
    m_NeedsRedraw = false;

    ConsoleUtils::ClearScreen();

    ConsoleUtils::SetCursorPosition(10, 3);
    std::cout << "================================";
    ConsoleUtils::SetCursorPosition(10, 4);
    std::cout << "          MAIN  MENU            ";
    ConsoleUtils::SetCursorPosition(10, 5);
    std::cout << "================================";

    ConsoleUtils::SetCursorPosition(10, 8);
    std::cout << (m_SelectedOption == OPTION_START ? " > " : "   ") << "Start Game";

    ConsoleUtils::SetCursorPosition(10, 10);
    std::cout << (m_SelectedOption == OPTION_EXIT ? " > " : "   ") << "Exit Game";

    ConsoleUtils::SetCursorPosition(10, 14);
    std::cout << "Use Up/Down Arrow and Enter to select";

    std::cout << std::flush;
}
