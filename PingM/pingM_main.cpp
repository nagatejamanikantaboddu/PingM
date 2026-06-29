#include "pingM_logger.h"
#include "pingM_mapper.h"

#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (!g_logger.init())
    {
        std::cerr << "PingM: failed to initialize logger" << std::endl;
        return 1;
    }

    if (argc < 2)
    {
        g_logger.error("PingM: usage: pingM ping <host> [option]");
        g_logger.shutdown();
        return 1;
    }

    std::string cmd;
    for (int i = 1; i < argc; ++i)
    {
        if (i > 1)
        {
            cmd += " ";
        }
        cmd += argv[i];
    }

    if (argv[1] == std::string("ping"))
    {
        g_logger.info("PingM ping");
        pingM_ping_mapper.ping(cmd);
    }
    else
    {
        g_logger.error("PingM: unknown command");
        g_logger.shutdown();
        return 1;
    }

    g_logger.shutdown();
    return 0;
}
