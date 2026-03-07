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

// Inline implementations
inline Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

inline void Logger::Init(const std::string& filename)
{
    if (m_File.is_open())
        m_File.close();
    m_File.open(filename, std::ios::app);
}

inline void Logger::Log(LogLevel level, const std::string& message)
{
    if (m_File)
        m_File << "[" << LevelToString(level) << "] " << message << std::endl;
}

inline std::string Logger::LevelToString(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error:   return "ERROR";
    }
    return "UNKNOWN";
}
