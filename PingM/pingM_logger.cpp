#include "pingM_logger.h"

#include <iostream>

PingMLogger g_logger;

namespace
{
const char *to_string(LogLevel level)
{
    switch (level)
    {
    case LogLevel::INIT:
        return "INIT";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARNING:
        return "WARNING";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::FATAL:
        return "FATAL";
    case LogLevel::SHUTDOWN:
        return "SHUTDOWN";
    default:
        return "UNKNOWN";
    }
}
} // namespace

bool PingMLogger::init()
{
    if (log_file.is_open())
    {
        return true;
    }
    log_file.open("pingM.log", std::ios::out | std::ios::app);
    return log_file.is_open();
}

void PingMLogger::info(const std::string &message)
{
    write(LogLevel::INFO, message);
}

void PingMLogger::warning(const std::string &message)
{
    write(LogLevel::WARNING, message);
}

void PingMLogger::error(const std::string &message)
{
    write(LogLevel::ERROR, message);
}

void PingMLogger::fatal(const std::string &message)
{
    write(LogLevel::FATAL, message);
}

void PingMLogger::shutdown()
{
    if (log_file.is_open())
    {
        log_file.close();
    }
}

void PingMLogger::write(LogLevel level, const std::string &message)
{
    if (!log_file.is_open())
    {
        std::cerr << "PingM logger: Log file not open" << std::endl;
        return;
    }
    log_file << to_string(level) << " " << message << std::endl;
}
