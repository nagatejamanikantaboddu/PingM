# PingM

CLI wrapper around the system `ping` command.

## Build (CMake)

```bash
cd PingM
cmake -B build
cmake --build build
```

## Build (direct)

```bash
g++ -std=c++17 pingM_main.cpp pingM.cpp pingM_parser.cpp pingM_engine.cpp pingM_mapper.cpp pingM_logger.cpp pingM_output_parser.cpp -o pingM.exe
```

## Run

```bash
pingM.exe ping 8.8.8.8
```

Logs are written to `pingM.log`.
