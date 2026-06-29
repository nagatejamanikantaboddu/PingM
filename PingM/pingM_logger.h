#pragma once

#include <fstream>
#include <string>

enum class LogLevel
{
    INIT,
    INFO,
    WARNING,
    ERROR,
    FATAL,
    SHUTDOWN
};

class PingMLogger
{
public:
    bool init();
    void info(const std::string &message);
    void warning(const std::string &message);
    void error(const std::string &message);
    void fatal(const std::string &message);
    void shutdown();

private:
    std::ofstream log_file;

    void write(LogLevel level, const std::string &message);
};

extern PingMLogger g_logger;
