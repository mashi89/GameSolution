#include "ConsoleUtils.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>

void ConsoleUtils::ClearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE)
        return;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
        return;

    DWORD cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    COORD homeCoords = { 0, 0 };
    DWORD count;

    FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count);
    SetConsoleCursorPosition(hConsole, homeCoords);
}

void ConsoleUtils::SetCursorPosition(int x, int y)
{
    COORD coord = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ConsoleUtils::ShowCursor(bool visible)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(hConsole, &info);
    info.bVisible = visible ? TRUE : FALSE;
    SetConsoleCursorInfo(hConsole, &info);
}

bool ConsoleUtils::KeyAvailable()
{
    return _kbhit() != 0;
}

int ConsoleUtils::ReadKey()
{
    int ch = _getch();
    // Arrow keys and other special keys produce a 0 or 224 prefix byte
    if (ch == 0 || ch == 224)
    {
        int second = _getch();
        return 256 + second;
    }
    return ch;
}

#else
// ---- POSIX (Linux / macOS) fallback ----------------------------------------
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <iostream>

static void setRawMode(bool enable)
{
    static struct termios orig;
    if (enable)
    {
        tcgetattr(STDIN_FILENO, &orig);
        struct termios raw = orig;
        raw.c_lflag &= ~(ICANON | ECHO);
        raw.c_cc[VMIN]  = 0;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }
    else
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &orig);
    }
}

void ConsoleUtils::ClearScreen()
{
    std::cout << "\033[2J\033[H" << std::flush;
}

void ConsoleUtils::SetCursorPosition(int x, int y)
{
    // ANSI: row and column are 1-based
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}

void ConsoleUtils::ShowCursor(bool visible)
{
    if (visible)
        printf("\033[?25h");
    else
        printf("\033[?25l");
    fflush(stdout);
}

bool ConsoleUtils::KeyAvailable()
{
    setRawMode(true);
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    int ch = getchar();
    fcntl(STDIN_FILENO, F_SETFL, flags);
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        setRawMode(false);
        return true;
    }
    setRawMode(false);
    return false;
}

int ConsoleUtils::ReadKey()
{
    setRawMode(true);
    int ch = getchar();
    // Check for ESC sequence (arrow keys: ESC [ A/B/C/D)
    if (ch == 27)
    {
        int ch2 = getchar();
        if (ch2 == '[')
        {
            int ch3 = getchar();
            setRawMode(false);
            if (ch3 == 'A') return KEY_ARROW_UP;
            if (ch3 == 'B') return KEY_ARROW_DOWN;
            return KEY_ESC;
        }
        setRawMode(false);
        return KEY_ESC;
    }
    setRawMode(false);
    return ch;
}
#endif
