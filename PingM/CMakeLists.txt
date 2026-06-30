cmake_minimum_required(VERSION 3.16)
project(PingM LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pingM
    pingM_main.cpp
    pingM_parser.cpp
    pingM_engine.cpp
    pingM_mapper.cpp
    pingM_logger.cpp
    pingM_output_parser.cpp
    pingM_report.cpp
    pingM_colour.cpp
    pingM_colour.h
)

target_include_directories(pingM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
