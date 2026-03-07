#pragma once

namespace ConsoleUtils
{
    // Key code constants
    static const int KEY_ESC        = 27;
    static const int KEY_ENTER      = 13;
    static const int KEY_ARROW_UP   = 256 + 72;
    static const int KEY_ARROW_DOWN = 256 + 80;

    // Clear the console screen
    void ClearScreen();

    // Move the cursor to column x, row y (0-based)
    void SetCursorPosition(int x, int y);

    // Show or hide the blinking cursor
    void ShowCursor(bool visible);

    // Non-blocking check: returns true if a key is waiting to be read
    bool KeyAvailable();

    // Blocking read of the next key.
    // Arrow-key sequences are collapsed: up -> KEY_ARROW_UP, down -> KEY_ARROW_DOWN.
    int  ReadKey();
}
