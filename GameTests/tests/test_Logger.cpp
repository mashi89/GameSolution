#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <cstdio>
#include <filesystem>
#include "Logger.h"

// Each test uses a unique temporary file so that singleton state does not
// interfere between runs.
static std::string ReadFile(const std::string& path)
{
    std::ifstream f(path);
    return std::string(std::istreambuf_iterator<char>(f),
                       std::istreambuf_iterator<char>());
}

class LoggerTest : public ::testing::Test
{
protected:
    std::string logPath;

    void SetUp() override
    {
        logPath = (std::filesystem::temp_directory_path() /
                   ("test_logger_" +
                    std::to_string(reinterpret_cast<uintptr_t>(this)) +
                    ".log")).string();
        Logger::GetInstance().Init(logPath);
    }

    void TearDown() override
    {
        std::remove(logPath.c_str());
    }
};

TEST_F(LoggerTest, InfoLevelWrittenToFile)
{
    Logger::GetInstance().Info("hello info");
    std::string contents = ReadFile(logPath);
    EXPECT_NE(contents.find("[INFO]"), std::string::npos);
    EXPECT_NE(contents.find("hello info"), std::string::npos);
}

TEST_F(LoggerTest, WarningLevelWrittenToFile)
{
    Logger::GetInstance().Warning("watch out");
    std::string contents = ReadFile(logPath);
    EXPECT_NE(contents.find("[WARNING]"), std::string::npos);
    EXPECT_NE(contents.find("watch out"), std::string::npos);
}

TEST_F(LoggerTest, ErrorLevelWrittenToFile)
{
    Logger::GetInstance().Error("something broke");
    std::string contents = ReadFile(logPath);
    EXPECT_NE(contents.find("[ERROR]"), std::string::npos);
    EXPECT_NE(contents.find("something broke"), std::string::npos);
}

TEST_F(LoggerTest, MultipleMessagesAppended)
{
    Logger::GetInstance().Info("first");
    Logger::GetInstance().Warning("second");
    Logger::GetInstance().Error("third");
    std::string contents = ReadFile(logPath);
    EXPECT_NE(contents.find("first"),  std::string::npos);
    EXPECT_NE(contents.find("second"), std::string::npos);
    EXPECT_NE(contents.find("third"),  std::string::npos);
}

TEST_F(LoggerTest, LogWithExplicitLevel)
{
    Logger::GetInstance().Log(LogLevel::Info, "explicit info");
    std::string contents = ReadFile(logPath);
    EXPECT_NE(contents.find("[INFO]"),       std::string::npos);
    EXPECT_NE(contents.find("explicit info"), std::string::npos);
}
