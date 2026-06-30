#pragma once

#include <string>

#include "pingM_output_parser.h"

class pingM_report
{
public:
    std::string generate(const PingMetrics &metrics);
};
