#include "pingM_mapper.h"
#include "pingM_logger.h"
#include "pingM_parser.h"
#include "pingM_engine.h"

pingM_ping pingM_ping_mapper;

void pingM_ping::ping(std::string &cmd)
{
    g_logger.info("PingM ping: " + cmd);

    pingM_parser parser;
    pingM_engine engine;

    std::string ping_cmd = parser.parse(cmd);
    if (!ping_cmd.empty())
    {
        engine.execute_cmd(ping_cmd);
    }
}
