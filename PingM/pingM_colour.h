#include <string>
#pragma once

std::string colorize_status(const std::string& text);
std::string colorize_host(const std::string& text);
std::string colorize_packets(const std::string& text);
std::string colorize_rtt(const std::string& text);
std::string colorize_jitter(const std::string& text);
std::string colorize_ttl(const std::string& text);
std::string colorize_ttl_hint(const std::string& text);
std::string colorize_connection(const std::string& text);
std::string colorize_latency(const std::string& text);
std::string colorize_packet_loss(const std::string& text);
std::string colorize_network_score(const std::string& text);
std::string colorize_overall_health(const std::string& text);
std::string colorize_recommendation(const std::string& text);