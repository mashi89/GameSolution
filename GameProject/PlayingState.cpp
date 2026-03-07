#include "PlayingState.h"
#include "GameManager.h"
#include "ConsoleUtils.h"
#include <iostream>

// Screen dimensions for a standard 80x24 console
static const int SCREEN_WIDTH  = 80;
static const int SCREEN_HEIGHT = 24;

// Position of the "pixel" – centre of the screen
static const int PIXEL_X = SCREEN_WIDTH  / 2;
static const int PIXEL_Y = SCREEN_HEIGHT / 2;

// CP437 full-block character used as a single "pixel" on the game screen
static const char BLOCK_CHARACTER = static_cast<char>(219);

void PlayingState::OnEnter()
{
    m_NeedsRedraw = true;
    ConsoleUtils::ShowCursor(false);
}

void PlayingState::OnExit()
{
    ConsoleUtils::ShowCursor(true);
}

void PlayingState::Update(float /*deltaTime*/)
{
    if (!ConsoleUtils::KeyAvailable())
        return;

    int key = ConsoleUtils::ReadKey();

    if (key == ConsoleUtils::KEY_ESC)
    {
        // Return to the main menu
        GameManager::GetInstance().PopState();
        return;  // Do not touch members after PopState (state may be destroyed)
    }
}

void PlayingState::Render()
{
    if (!m_NeedsRedraw)
        return;
    m_NeedsRedraw = false;

    ConsoleUtils::ClearScreen();

    ConsoleUtils::SetCursorPosition(0, 0);
    std::cout << "=== GAME SCREEN ===";

    // Draw one solid-block character as the "pixel"
    ConsoleUtils::SetCursorPosition(PIXEL_X, PIXEL_Y);
    std::cout << BLOCK_CHARACTER;  // CP437 full-block: █

    ConsoleUtils::SetCursorPosition(0, SCREEN_HEIGHT - 1);
    std::cout << "Press ESC to return to Main Menu";

    std::cout << std::flush;
}
