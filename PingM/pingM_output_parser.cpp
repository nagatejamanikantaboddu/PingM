#include "pingM_output_parser.h"
#include "pingM_logger.h"

#include <cmath>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
namespace
{
std::string trim(const std::string &value)
{
    const auto start = value.find_first_not_of(" \t\r\n");
    if (start == std::string::npos)
    {
        return "";
    }
    const auto end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

int parseIntAfter(const std::string &line, const std::string &token)
{
    const auto pos = line.find(token);
    if (pos == std::string::npos)
    {
        return -1;
    }

    std::istringstream stream(line.substr(pos + token.size()));
    int value = -1;
    stream >> value;
    return value;
}

double parsePercentAfter(const std::string &line, const std::string &token)
{
    const auto pos = line.find(token);
    if (pos == std::string::npos)
    {
        return -1.0;
    }

    std::istringstream stream(line.substr(pos + token.size()));
    double value = -1.0;
    stream >> value;
    return value;
}

int parseRttMs(const std::string &line, const std::string &token)
{
    const auto pos = line.find(token);
    if (pos == std::string::npos)
    {
        return -1;
    }

    const std::string tail = line.substr(pos + token.size());
    if (tail.find('<') != std::string::npos)
    {
        return 0;
    }

    return parseIntAfter(line, token);
}

int parseReplyRtt(const std::string &line)
{
    const auto time_pos = line.find("time=");
    if (time_pos != std::string::npos)
    {
        std::istringstream stream(line.substr(time_pos + 5));
        double value = -1.0;
        stream >> value;
        if (value >= 0.0)
        {
            return static_cast<int>(std::lround(value));
        }
    }

    const auto legacy_time_pos = line.find("time");
    if (legacy_time_pos == std::string::npos)
    {
        return -1;
    }

    const std::string tail = line.substr(legacy_time_pos + 4);
    if (!tail.empty() && tail[0] == '<')
    {
        return 0;
    }

    if (!tail.empty() && tail[0] == '=')
    {
        std::istringstream stream(tail.substr(1));
        int value = -1;
        stream >> value;
        return value;
    }

    return -1;
}

int parseReplyTtl(const std::string &line)
{
    auto ttl_pos = line.find("TTL=");
    if (ttl_pos == std::string::npos)
    {
        ttl_pos = line.find("ttl=");
    }
    if (ttl_pos == std::string::npos)
    {
        return -1;
    }

    std::istringstream stream(line.substr(ttl_pos + 4));
    int value = -1;
    stream >> value;
    return value;
}

int parseIntBefore(const std::string &line, const std::string &token)
{
    const auto pos = line.find(token);
    if (pos == std::string::npos)
    {
        return -1;
    }

    std::size_t end = pos;
    while (end > 0 && line[end - 1] == ' ')
    {
        --end;
    }

    std::size_t start = end;
    while (start > 0 && std::isdigit(static_cast<unsigned char>(line[start - 1])))
    {
        --start;
    }

    std::istringstream stream(line.substr(start, end - start));
    int value = -1;
    stream >> value;
    return value;
}

double parseLinuxLossPercent(const std::string &line)
{
    const auto pos = line.find("% packet loss");
    if (pos == std::string::npos)
    {
        return -1.0;
    }

    std::size_t end = pos;
    std::size_t start = end;
    while (start > 0 &&
           (std::isdigit(static_cast<unsigned char>(line[start - 1])) || line[start - 1] == '.'))
    {
        --start;
    }

    std::istringstream stream(line.substr(start, end - start));
    double value = -1.0;
    stream >> value;
    return value;
}

bool parseLinuxRttSummary(const std::string &line, int &min_rtt, int &max_rtt, int &avg_rtt)
{
    const auto rtt_pos = line.find("rtt min/avg/max");
    const auto round_pos = line.find("round-trip min/avg/max");
    std::size_t eq_pos = std::string::npos;

    if (rtt_pos != std::string::npos)
    {
        eq_pos = line.find('=', rtt_pos);
    }
    else if (round_pos != std::string::npos)
    {
        eq_pos = line.find('=', round_pos);
    }

    if (eq_pos == std::string::npos)
    {
        return false;
    }

    std::istringstream stream(line.substr(eq_pos + 1));
    double min_value = -1.0;
    double avg_value = -1.0;
    double max_value = -1.0;
    char slash1 = '\0';
    char slash2 = '\0';
    stream >> min_value >> slash1 >> avg_value >> slash2 >> max_value;
    if (stream.fail() || slash1 != '/' || slash2 != '/')
    {
        return false;
    }

    min_rtt = static_cast<int>(std::lround(min_value));
    avg_rtt = static_cast<int>(std::lround(avg_value));
    max_rtt = static_cast<int>(std::lround(max_value));
    return true;
}

std::string parseHost(const std::string &line)
{
    const std::string prefix = "Pinging ";
    const auto start = line.find(prefix);
    if (start != std::string::npos)
    {
        const auto host_start = start + prefix.size();
        const auto end = line.find(" with", host_start);
        if (end != std::string::npos)
        {
            return trim(line.substr(host_start, end - host_start));
        }
    }

    const std::string stats_prefix = "Ping statistics for ";
    const auto stats_start = line.find(stats_prefix);
    if (stats_start != std::string::npos)
    {
        const auto host_start = stats_start + stats_prefix.size();
        const auto end = line.find(':', host_start);
        if (end != std::string::npos)
        {
            return trim(line.substr(host_start, end - host_start));
        }
    }

    const std::string ping_prefix = "PING ";
    if (line.rfind(ping_prefix, 0) == 0)
    {
        const auto host_start = ping_prefix.size();
        const auto end = line.find('(', host_start);
        if (end != std::string::npos)
        {
            return trim(line.substr(host_start, end - host_start));
        }
    }

    if (line.rfind("---", 0) == 0)
    {
        const auto end = line.find(" ping statistics ---");
        if (end != std::string::npos)
        {
            return trim(line.substr(4, end - 4));
        }
    }

    return "";
}

double computeJitter(const std::vector<int> &samples)
{
    if (samples.size() < 2)
    {
        return 0.0;
    }

    double total = 0.0;
    for (size_t i = 1; i < samples.size(); ++i)
    {
        total += std::abs(samples[i] - samples[i - 1]);
    }
    return total / static_cast<double>(samples.size() - 1);
}

Reachability computeReachability(int sent, int received)
{
    if (sent > 0 && received == sent)
    {
        return Reachability::REACHABLE;
    }
    if (received == 0)
    {
        return Reachability::UNREACHABLE;
    }
    if (sent > 0 && received > 0 && received < sent)
    {
        return Reachability::PARTIAL;
    }
    return Reachability::UNKNOWN;
}

const char *reachabilityToString(Reachability reachability)
{
    switch (reachability)
    {
    case Reachability::REACHABLE:
        return "Reachable";
    case Reachability::UNREACHABLE:
        return "Host Unreachable";
    case Reachability::PARTIAL:
        return "Partially Reachable";
    default:
        return "Unknown";
    }
}
} // namespace

PingMetrics pingM_output_parser::analyse(const std::string &output)
{
    PingMetrics metrics;
    metrics.raw_output = output;

    std::istringstream stream(output);
    std::string line;
    while (std::getline(stream, line))
    {
        if (!line.empty() && line.back() == '\r')
        {
            line.pop_back();
        }

        if (metrics.host.empty())
        {
            metrics.host = parseHost(line);
        }

        if (line.find("Reply from") != std::string::npos ||
            (line.find("bytes from") != std::string::npos && line.find("icmp_seq") != std::string::npos))
        {
            const int sample = parseReplyRtt(line);
            if (sample >= 0)
            {
                metrics.samples.push_back(sample);
            }

            const int ttl = parseReplyTtl(line);
            if (ttl >= 0)
            {
                metrics.ttl = ttl;
            }
        }

        if (line.find("Packets:") != std::string::npos)
        {
            metrics.sent = parseIntAfter(line, "Sent = ");
            metrics.received = parseIntAfter(line, "Received = ");
            metrics.lost = parseIntAfter(line, "Lost = ");
            metrics.loss_percent = parsePercentAfter(line, "(");
        }

        if (line.find("packets transmitted") != std::string::npos)
        {
            metrics.sent = parseIntBefore(line, " packets transmitted");
            metrics.received = parseIntBefore(line, " received");
            metrics.loss_percent = parseLinuxLossPercent(line);
            if (metrics.sent >= 0 && metrics.received >= 0)
            {
                metrics.lost = metrics.sent - metrics.received;
            }
        }

        if (line.find("Minimum =") != std::string::npos)
        {
            metrics.min_rtt = parseRttMs(line, "Minimum = ");
            metrics.max_rtt = parseRttMs(line, "Maximum = ");
            metrics.avg_rtt = parseRttMs(line, "Average = ");
        }

        if (line.find("rtt min/avg/max") != std::string::npos ||
            line.find("round-trip min/avg/max") != std::string::npos)
        {
            int min_rtt = -1;
            int max_rtt = -1;
            int avg_rtt = -1;
            if (parseLinuxRttSummary(line, min_rtt, max_rtt, avg_rtt))
            {
                metrics.min_rtt = min_rtt;
                metrics.max_rtt = max_rtt;
                metrics.avg_rtt = avg_rtt;
            }
        }
    }

    metrics.jitter = computeJitter(metrics.samples);
    metrics.reachability = computeReachability(metrics.sent, metrics.received);

    if (metrics.samples.size() >= 2 && metrics.min_rtt < 0)
    {
        metrics.min_rtt = metrics.samples.front();
        metrics.max_rtt = metrics.samples.front();
        metrics.avg_rtt = 0;
        for (int sample : metrics.samples)
        {
            metrics.min_rtt = std::min(metrics.min_rtt, sample);
            metrics.max_rtt = std::max(metrics.max_rtt, sample);
            metrics.avg_rtt += sample;
        }
        metrics.avg_rtt /= static_cast<int>(metrics.samples.size());
    }

    g_logger.info(std::string("PingM output parser: host=") + metrics.host +
                  " status=" + reachabilityToString(metrics.reachability) +
                  " loss=" + std::to_string(static_cast<int>(metrics.loss_percent)) + "%");

    return metrics;
}
