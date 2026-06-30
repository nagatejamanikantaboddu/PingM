#include "pingM_engine.h"
#include "pingM_logger.h"
#include "pingM_output_parser.h"
#include "pingM_report.h"

#include <cstdio>
#include <iostream>
#include <string>

namespace
{
FILE *openPipe(const std::string &command)
{
#ifdef _WIN32
    return _popen(command.c_str(), "r");
#else
    return popen(command.c_str(), "r");
#endif
}

void closePipe(FILE *pipe)
{
#ifdef _WIN32
    _pclose(pipe);
#else
    pclose(pipe);
#endif
}
} // namespace

void pingM_engine::execute_cmd(std::string &cmd)
{
    g_logger.info("PingM engine: " + cmd);

    FILE *pipe = openPipe(cmd);
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
    }

    closePipe(pipe);

    g_logger.info("PingM engine: captured ping output");

    pingM_output_parser output_parser;
    PingMetrics metrics = output_parser.analyse(output);

    pingM_report report;
    const std::string report_text = report.generate(metrics);
    std::cout << report_text << std::endl;
    g_logger.info(report_text);
}
