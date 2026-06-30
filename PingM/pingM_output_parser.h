#pragma once

#include <string>
#include <vector>

enum class Reachability
{
    REACHABLE,
    UNREACHABLE,
    PARTIAL,
    UNKNOWN
};

struct PingMetrics
{
    std::string host;
    Reachability reachability = Reachability::UNKNOWN;
    int sent = 0;
    int received = 0;
    int lost = 0;
    double loss_percent = 0.0;
    int min_rtt = -1;
    int max_rtt = -1;
    int avg_rtt = -1;
    std::vector<int> samples;
    double jitter = 0.0;
    int ttl = -1;
    std::string raw_output;
};

class pingM_output_parser
{
public:
    PingMetrics analyse(const std::string &output);
};
