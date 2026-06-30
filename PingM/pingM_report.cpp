#include "pingM_report.h"

#include <sstream>
#include <string>
#include <iomanip>
#include "pingM_colour.h"
namespace
{
const char *reachabilityLabel(Reachability reachability)
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

std::string lossRating(double loss_percent)
{
    if (loss_percent >= 100.0)
    {
        return "Failed";
    }
    if (loss_percent == 0.0)
    {
        return "No Packet Loss";
    }
    if (loss_percent <= 2.0)
    {
        return "Low Packet Loss";
    }
    if (loss_percent <= 5.0)
    {
        return "Medium Packet Loss";
    }
    if (loss_percent <= 10.0)
    {
        return "High Packet Loss";
    }
    return "Critical Packet Loss";
}

std::string latencyRating(int avg_rtt)
{
    if (avg_rtt < 0)
    {
        return "N/A";
    }
    if (avg_rtt < 20)
    {
        return "Excellent";
    }
    if (avg_rtt <= 50)
    {
        return "Very Good";
    }
    if (avg_rtt <= 100)
    {
        return "Good";
    }
    if (avg_rtt <= 200)
    {
        return "Fair";
    }
    return "Poor";
}

std::string stabilityRating(int min_rtt, int max_rtt)
{
    if (min_rtt < 0 || max_rtt < 0)
    {
        return "N/A";
    }

    const int variation = max_rtt - min_rtt;
    if (variation < 5)
    {
        return "Stable";
    }
    if (variation <= 20)
    {
        return "Moderate";
    }
    return "Unstable";
}

std::string jitterRating(double jitter)
{
    if (jitter < 5.0)
    {
        return "Excellent";
    }
    if (jitter <= 15.0)
    {
        return "Good";
    }
    if (jitter <= 30.0)
    {
        return "Fair";
    }
    return "Poor";
}

std::string ttlOs(int ttl)
{
    if (ttl < 0)
    {
        return "Unknown";
    }
    else if (ttl > 0 && ttl <= 64)
    {
        return "Linux / macOS";
    }
    else if (ttl > 64 && ttl <= 128)
    {
        return "Windows";
    }
    else if (ttl > 128 && ttl <= 255)
    {
        return "Network devices";
    }
    else
    {
        return "Unknown";
    }
}

int computeScore(const PingMetrics &metrics)
{
    if (metrics.reachability == Reachability::UNREACHABLE)
    {
        return 0;
    }

    int score = 100;
    if (metrics.loss_percent > 0.0)
    {
        score -= 20;
    }
    if (metrics.loss_percent > 10.0)
    {
        score -= 40;
    }
    if (metrics.avg_rtt > 100)
    {
        score -= 15;
    }
    if (metrics.avg_rtt > 200)
    {
        score -= 30;
    }
    if (metrics.jitter > 30.0)
    {
        score -= 15;
    }

    if (score < 0)
    {
        score = 0;
    }
    if (score > 100)
    {
        score = 100;
    }
    return score;
}

std::string overallRating(int score)
{
    if (score >= 90)
    {
        return "Excellent";
    }
    if (score >= 75)
    {
        return "Good";
    }
    if (score >= 60)
    {
        return "Fair";
    }
    if (score >= 40)
    {
        return "Poor";
    }
    return "Critical";
}

std::string recommendation(const PingMetrics &metrics, int score)
{
    if (metrics.reachability == Reachability::UNREACHABLE)
    {
        return "Host is unreachable.\n"
               "Check network connectivity.\n"
               "Verify firewall configuration.\n"
               "Confirm the target host is online.";
    }

    if (metrics.loss_percent > 10.0)
    {
        return "High packet loss detected.\n"
               "Check Wi-Fi signal.\n"
               "Verify cable connection.\n"
               "Retry after a few minutes.\n"
               "Check firewall configuration.";
    }

    if (metrics.avg_rtt > 200)
    {
        return "High latency detected.\n"
               "Check VPN.\n"
               "Try another network.\n"
               "Test nearby servers.";
    }

    if (stabilityRating(metrics.min_rtt, metrics.max_rtt) == "Unstable" ||
        metrics.jitter > 30.0)
    {
        return "Unstable connection detected.\n"
               "Run continuous monitoring.\n"
               "Check signal strength.\n"
               "Monitor packet loss over time.";
    }

    if (score >= 90)
    {
        return "Network is healthy.\n"
               "No corrective action required.";
    }

    return "Network performance is acceptable.\n"
           "Monitor the connection if issues persist.";
}

std::string formatRtt(int value)
{
    if (value < 0)
    {
        return "N/A";
    }
    return std::to_string(value) + " ms";
}

std::string formatDouble(double value, int precision = 0)
{
    std::ostringstream stream;
    stream.setf(std::ios::fixed);
    stream.precision(precision);
    stream << value;
    return stream.str();
}

void appendLine(std::ostringstream &stream, const std::string &label, const std::string &value, const char *label_color = "\033[93m")
{
    const char *RESET = "\033[0m";
    stream << label_color << label << RESET;
    const std::size_t width = 18;
    if (label.size() < width)
    {
        stream << std::string(width - label.size(), ' ');
    }
    stream << ": " << value << "\n";
}
} 

std::string pingM_report::generate(const PingMetrics &metrics)
{
    const int score = computeScore(metrics);
    const std::string overall = overallRating(score);
    const std::string loss_eval = lossRating(metrics.loss_percent);
    const std::string latency_eval = latencyRating(metrics.avg_rtt);
    const std::string stability_eval = stabilityRating(metrics.min_rtt, metrics.max_rtt);
    const std::string jitter_eval = jitterRating(metrics.jitter);

    std::ostringstream report;
    report << "\033[95m";
    report << "==============================\n";
    report << "PingM Network Analysis Report\n";
    report << "==============================\n";
    report << "\033[0m\n";

    appendLine(report, "Host", colorize_host(metrics.host.empty() ? "Unknown" : metrics.host));
    appendLine(report, "Status", colorize_status(reachabilityLabel(metrics.reachability)));
    report << "\n";

    appendLine(report, "Packets Sent", colorize_packets(std::to_string(metrics.sent)));
    appendLine(report, "Packets Received", colorize_packets(std::to_string(metrics.received)));
    appendLine(report, "Packet Loss", colorize_packet_loss(formatDouble(metrics.loss_percent) + "%"));
    report << "\n";

    appendLine(report, "Minimum RTT", colorize_rtt(formatRtt(metrics.min_rtt)));
    appendLine(report, "Maximum RTT", colorize_rtt(formatRtt(metrics.max_rtt)));
    appendLine(report, "Average RTT", colorize_rtt(formatRtt(metrics.avg_rtt)));
    appendLine(report, "Jitter", colorize_jitter(formatDouble(metrics.jitter, 1) + " ms"));
    appendLine(report, "TTL", colorize_ttl(metrics.ttl < 0 ? "N/A" : std::to_string(metrics.ttl)));
    appendLine(report, "TTL Hint", colorize_ttl_hint(ttlOs(metrics.ttl)));
    report << "\n";

    appendLine(report, "Connection", colorize_connection(stability_eval));
    appendLine(report, "Latency", colorize_latency(latency_eval));
    appendLine(report, "Packet Loss", colorize_packet_loss(loss_eval));
    appendLine(report, "Jitter", colorize_jitter(jitter_eval));
    report << "\n";

    appendLine(report, "Network Score", colorize_network_score(std::to_string(score) + "/100"));
    appendLine(report, "Overall Health", colorize_overall_health(overall));
    report << "\n";

    report << "Recommendation:\n";
    report << colorize_recommendation(recommendation(metrics, score)) << "\n";

    return report.str();
}
