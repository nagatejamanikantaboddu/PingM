#include "pingM_mapper.h"
#include "pingM_logger.h"
#include "pingM_parser.h"
#include "pingM.h"
#include "pingM_engine.h"

pingM_ping pingM_ping_mapper;

void pingM_ping::ping(std::string &cmd)
{
    g_logger.info("PingM ping: " + cmd);

    std::string ip_address;
    std::string option;

    pingM_parser parser;
    pingM cmd_builder;
    pingM_engine engine;

    parser.parse(cmd, ip_address, option);
    std::string ping_cmd = cmd_builder.generate_cmd(ip_address, option);
    engine.execute_cmd(ping_cmd);
}
