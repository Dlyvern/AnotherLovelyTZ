# CPU Load Monitor

A Linux client-server CPU load monitor.

The console client reads CPU statistics from `/proc/stat`, calculates total and
per-core CPU load, and sends the data to a Qt server over UDP. The server displays the incoming data as a live graph.

## Dependencies

- Linux
- CMake 3.15 or newer
- C compiler
- C++ compiler with C++20 support
- Qt 5 Widgets, Network, and PrintSupport modules
- POSIX threads

## Setup

Clone the repository with submodules, or initialize them after cloning:

```bash
git submodule update --init --recursive
```

## Build

```bash
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build
```

The executables are generated at:

```text
build/Server/Server
build/Client/Client
```

## Run

Start the server first:

```bash
./build/Server/Server
```

Then start the client in another terminal:

```bash
./build/Client/Client
```

The client sends CPU load packets to `localhost:1234` once per second. Press
`Ctrl+C` in the client terminal to stop it cleanly.