# Multi-Client Chat Application in C

A multi-client chat application written in **C** using **socket programming**.

The project contains separate client and server implementations and uses **CMake** to build either the client or server based on the `BUILD_TYPE` argument.

## Project Structure

```text
mutli-chat-app-in-c/
├── CMakeLists.txt
├── README.md
├── inc/
│   ├── client.h
│   ├── define.h
│   └── server.h
└── src/
    ├── client.c
    ├── main.c
    └── server.c
```

### Directory and File Description

| Path             | Description                                            |
| ---------------- | ------------------------------------------------------ |
| `CMakeLists.txt` | CMake configuration used to build the client or server |
| `README.md`      | Project documentation                                  |
| `inc/client.h`   | Client-side declarations                               |
| `inc/server.h`   | Server-side declarations                               |
| `inc/define.h`   | Common definitions                                     |
| `src/client.c`   | Client implementation                                  |
| `src/server.c`   | Server implementation                                  |
| `src/main.c`     | Application entry point                                |

## Build Setup

This project uses **CMake**.

### Prerequisites

Make sure CMake and a C compiler are installed:

```bash
cmake --version
gcc --version
```

## Build Client

Create a build directory and configure CMake with `BUILD_TYPE=client`:

```bash
cmake -S . -B build -DBUILD_TYPE=client
```

Build the client:

```bash
cmake --build build
```

The client executable will be generated at:

```text
build/client
```

Run the client:

```bash
./build/client
```

## Build Server

Configure CMake with `BUILD_TYPE=server`:

```bash
cmake -S . -B build -DBUILD_TYPE=server
```

Build the server:

```bash
cmake --build build
```

The server executable will be generated at:

```text
build/server
```

Run the server:

```bash
./build/server
```

## Build Commands Summary

### Client

```bash
cmake -S . -B build -DBUILD_TYPE=client
cmake --build build
./build/client
```

### Server

```bash
cmake -S . -B build -DBUILD_TYPE=server
cmake --build build
./build/server
```

## Clean Build

To remove the existing build directory and perform a fresh build:

```bash
rm -rf build
```

Then configure and build again.

For example:

```bash
rm -rf build
cmake -S . -B build -DBUILD_TYPE=client
cmake --build build
```

## Build Type

The `BUILD_TYPE` CMake argument determines which application is built:

```text
BUILD_TYPE=client  →  builds client
BUILD_TYPE=server  →  builds server
```

An invalid build type will result in a CMake configuration error.

## Technologies

* **C**
* **Socket Programming**
* **TCP/IP**
* **CMake**
* **Linux/POSIX**

## Note

The project is intended as a multi-client chat application where the server handles connections from multiple clients using socket programming.



