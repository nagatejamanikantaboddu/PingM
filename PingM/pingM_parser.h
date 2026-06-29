#pragma once

#include <string>

class pingM_parser
{
public:
    void parse(std::string &cmd, std::string &ip_address, std::string &option);
};
