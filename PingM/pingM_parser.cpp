#include "pingM_parser.h"
#include "pingM_logger.h"

#include <sstream>
#include <vector>

std::string pingM_parser::parse(std::string &cmd)
{
    if (cmd.empty())
    {
        g_logger.error("PingM parser: Empty command");
        return "";
    }

    std::stringstream ss(cmd);
    std::vector<std::string> tokens;
    std::string token;

    while (ss >> token)
    {
        tokens.push_back(token);
    }

    if (tokens.empty())
    {
        g_logger.error("PingM parser: No command found");
        return "";
    }

    if (tokens[0] != "ping")
    {
        g_logger.error("PingM parser: Unknown command");
        return "";
    }

    if (tokens.size() < 2)
    {
        g_logger.error("PingM parser: Missing IP address or hostname");
        return "";
    }

    const std::string &host = tokens[1];

#ifdef _WIN32
    const std::string ping_cmd = "ping -n 4 " + host;
#else
    const std::string ping_cmd = "ping -c 4 " + host;
#endif

    g_logger.info("Built command: " + ping_cmd);
    return ping_cmd;
}
