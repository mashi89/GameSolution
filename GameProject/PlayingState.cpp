#include "PlayingState.h"
#include "GameManager.h"
#include "ConsoleUtils.h"
#include <iostream>

// Screen dimensions for a standard 80x24 console
static const int SCREEN_WIDTH  = 80;
static const int SCREEN_HEIGHT = 24;

// CP437 full-block character used as a single "pixel" on the game screen
static const char BLOCK_CHARACTER = static_cast<char>(219);

void PlayingState::OnEnter()
{
    m_PixelX = SCREEN_WIDTH  / 2;
    m_PixelY = SCREEN_HEIGHT / 2;
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

    int newX = m_PixelX;
    int newY = m_PixelY;

    if (key == ConsoleUtils::KEY_ARROW_UP)         newY -= 1;
    else if (key == ConsoleUtils::KEY_ARROW_DOWN)  newY += 1;
    else if (key == ConsoleUtils::KEY_ARROW_LEFT)  newX -= 1;
    else if (key == ConsoleUtils::KEY_ARROW_RIGHT) newX += 1;

    // Clamp to the playable area (leave row 0 for title, row SCREEN_HEIGHT-1 for hint)
    if (newX < 0)                newX = 0;
    if (newX >= SCREEN_WIDTH)    newX = SCREEN_WIDTH  - 1;
    if (newY < 1)                newY = 1;
    if (newY >= SCREEN_HEIGHT - 1) newY = SCREEN_HEIGHT - 2;

    if (newX != m_PixelX || newY != m_PixelY)
    {
        m_PixelX = newX;
        m_PixelY = newY;
        m_NeedsRedraw = true;
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
    ConsoleUtils::SetCursorPosition(m_PixelX, m_PixelY);
    std::cout << BLOCK_CHARACTER;  // CP437 full-block: █

    ConsoleUtils::SetCursorPosition(0, SCREEN_HEIGHT - 1);
    std::cout << "Use arrow keys to move | Press ESC to return to Main Menu";

    std::cout << std::flush;
}
