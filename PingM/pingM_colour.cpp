#include "pingM_colour.h"
#include <string>
namespace
{
    // Formatting
    const char* RESET = "\033[0m";
    const char* BOLD  = "\033[1m";

    // Colors
    const char* RED    = "\033[91m";
    const char* GREEN  = "\033[92m";
    const char* YELLOW = "\033[93m";
    const char* CYAN   = "\033[96m";
    const char* WHITE  = "\033[97m";
}

std::string colorize_status(const std::string& text){
    if(text == "Reachable"){
        return std::string(GREEN) + text + std::string(RESET);
    }
    else if(text == "Host Unreachable"){
        return std::string(RED) + text + std::string(RESET);
    }
    else if(text == "Partially Reachable"){
        return std::string(YELLOW) + text + std::string(RESET);
    }
    else{
        return text;
    }
}
std::string colorize_recommendation(const std::string& text){
    if(text.find("Host is unreachable.") != std::string::npos){
        return std::string(RED) + text + std::string(RESET);
    }
    else if(text.find("High packet loss detected.") != std::string::npos){
        return std::string(YELLOW) + text + std::string(RESET);
    }
    else if(text.find("High latency detected.") != std::string::npos){
        return std::string(YELLOW) + text + std::string(RESET);
    }
    else if(text.find("Unstable connection detected.") != std::string::npos){
        return std::string(YELLOW) + text + std::string(RESET);
    }
    else if(text.find("Network is healthy.") != std::string::npos){
        return std::string(GREEN) + text + std::string(RESET);
    }
    else{
        return text;
    }
}
std::string colorize_host(const std::string& text){
    return std::string(YELLOW) + text + std::string(RESET);
}
std::string colorize_packets(const std::string& text){
    return std::string(CYAN) + text + std::string(RESET);
}
std::string colorize_rtt(const std::string& text){
    return std::string(CYAN) + text + std::string(RESET);
}
std::string colorize_jitter(const std::string& text){
    return std::string(CYAN) + text + std::string(RESET);
}
std::string colorize_ttl(const std::string& text){
    return std::string(CYAN) + text + std::string(RESET);
}
std::string colorize_ttl_hint(const std::string& text){
    return std::string(CYAN) + text + std::string(RESET);
}
std::string colorize_connection(const std::string& text){
    return std::string(CYAN) + text + std::string(RESET);
}
std::string colorize_latency(const std::string& text){
    return std::string(CYAN) + text + std::string(RESET);
}
std::string colorize_packet_loss(const std::string& text){
    return std::string(CYAN) + text + std::string(RESET);
}
std::string colorize_network_score(const std::string& text){
    return std::string(CYAN) + text + std::string(RESET);
}

std::string colorize_overall_health(const std::string& text){
    return std::string(CYAN) + text + std::string(RESET);
}