#pragma once
#include <string>
#include <fstream>

enum class LogLevel { Info, Warning, Error };

class Logger
{
public:
    static Logger& GetInstance();
    void Init(const std::string& filename = "game.log");
    void Log(LogLevel level, const std::string& message);
    void Info   (const std::string& m) { Log(LogLevel::Info,    m); }
    void Warning(const std::string& m) { Log(LogLevel::Warning, m); }
    void Error  (const std::string& m) { Log(LogLevel::Error,   m); }

private:
    Logger() = default;
    std::ofstream m_File;
    std::string LevelToString(LogLevel level);
};
