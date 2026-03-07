#include "Application.h"
#include "Logger.h"

int main()
{
    Logger::GetInstance().Init("game.log");
    Logger::GetInstance().Info("GameSolution starting...");

    Application app;
    if (!app.Initialize())
    {
        Logger::GetInstance().Error("Failed to initialize application.");
        return -1;
    }

    app.Run();

    Logger::GetInstance().Info("GameSolution exiting cleanly.");
    return 0;
}
