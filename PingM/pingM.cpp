#include "pingM.h"

std::string pingM::generate_cmd(std::string &ip_address, std::string &option)
{
    std::string cmd_str = "ping " + ip_address;
    if (!option.empty())
    {
        cmd_str += " -" + option;
    }
    return cmd_str;
}
