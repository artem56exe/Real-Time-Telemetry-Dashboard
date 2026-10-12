# Real-Time Telemetry Dashboard

A C++ system monitoring tool for Windows that collects and displays real-time hardware and network performance metrics using the Windows Performance Data Helper (PDH) API.

## Features
- **CPU Usage** — Overall processor utilisation.
- **GPU Usage** — GPU engine utilisation.
- **Disk Activity** — Overall disk activity percentage.
- **RAM Usage** — Memory usage, total RAM, and available RAM.
- **Network Traffic** — Bytes sent and received per second.

## Requirements
- Windows
- C++ compiler (e.g. MSVC)
- Windows SDK with PDH support

## Build
Compile with the PDH library linked:

```bash
g++ main.cpp -o telemetry.exe -lpdh
```

## Status
Currently a basic console-based prototype for collecting system telemetry, with potential for continuous monitoring and graphical dashboard integration.
