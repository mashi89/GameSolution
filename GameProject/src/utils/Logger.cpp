#include "include/utils/Logger.h"
#include <iostream>
#include <ctime>

Logger& Logger::GetInstance()
{
    static Logger instance;
    return instance;
}

void Logger::Init(const std::string& filename)
{
    m_File.open(filename, std::ios::out | std::ios::app);
}

void Logger::Log(LogLevel level, const std::string& message)
{
    std::string entry = "[" + LevelToString(level) + "] " + message;
    std::cout << entry << "\n";
    if (m_File.is_open())
    {
        m_File << entry << "\n";
        m_File.flush();
    }
}

std::string Logger::LevelToString(LogLevel level)
{
    switch (level)
    {
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error:   return "ERROR";
        default:                return "UNKNOWN";
    }
}
