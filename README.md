PingM

PingM is a command line wrapper around the system ping command. It runs ping for a host you give it and prints a simple network analysis report.

Build with CMake

```bash
cd PingM
cmake -B build
cmake --build build
```

The executable is created at build/pingM.

Build with g++

```bash
g++ -std=c++17 pingM_main.cpp pingM_parser.cpp pingM_engine.cpp pingM_mapper.cpp pingM_logger.cpp pingM_output_parser.cpp pingM_report.cpp pingM_colour.cpp -o pingM.exe
```

Build and run with Docker

```bash
docker build -t pingm .
docker run --rm pingm ping 8.8.8.8
```

Run

Pass the ping subcommand followed by a host.

```bash
pingM ping 8.8.8.8
```

Logs

Logs are written to pingM.log.
