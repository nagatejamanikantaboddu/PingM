#include "pingM_engine.h"
#include "pingM_logger.h"
#include "pingM_output_parser.h"

#include <cstdio>
#include <iostream>
#include <string>

void pingM_engine::execute_cmd(std::string &cmd)
{
    g_logger.info("PingM engine: " + cmd);

    FILE *pipe = _popen(cmd.c_str(), "r");
    if (pipe == nullptr)
    {
        g_logger.error("PingM engine: failed to open pipe");
        return;
    }

    char buffer[256];
    std::string output;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
    {
        output += buffer;
        std::cout << buffer;
    }

    _pclose(pipe);

    g_logger.info("PingM engine: " + output);

    pingM_output_parser output_parser;
    PingResult result = output_parser.evaluate(output);

    switch (result)
    {
    case PingResult::REACHABLE:
        std::cout << "IP reachable" << std::endl;
        break;
    case PingResult::UNREACHABLE:
        std::cout << "IP unreachable" << std::endl;
        break;
    default:
        std::cout << "IP status unknown" << std::endl;
        break;
    }
}
