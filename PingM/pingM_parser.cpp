#include "pingM_parser.h"
#include "pingM_logger.h"

#include <sstream>
#include <vector>

void pingM_parser::parse(std::string &cmd, std::string &ip_address, std::string &option)
{
    g_logger.info("PingM parser: " + cmd);

    if (cmd.empty())
    {
        g_logger.error("PingM parser: Empty command");
        return;
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
        return;
    }

    if (tokens[0] != "ping")
    {
        g_logger.error("PingM parser: Unknown command");
        return;
    }

    if (tokens.size() < 2)
    {
        g_logger.error("PingM parser: Missing IP address or hostname");
        return;
    }

    ip_address = tokens[1];
    option = (tokens.size() >= 3) ? tokens[2] : "";

    g_logger.info("Command : " + tokens[0]);
    g_logger.info("Host    : " + ip_address);

    if (!option.empty())
    {
        g_logger.info("Option  : " + option);
    }
}
