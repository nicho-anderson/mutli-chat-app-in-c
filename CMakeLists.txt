cmake_minimum_required(VERSION 3.16)

project(MultiChatApp C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Build type:
#   cmake -S . -B build -DBUILD_TYPE=client
#   cmake -S . -B build -DBUILD_TYPE=server

set(BUILD_TYPE "client" CACHE STRING "Build client or server")
set_property(CACHE BUILD_TYPE PROPERTY STRINGS client server)

# Include header files
include_directories(${PROJECT_SOURCE_DIR}/inc)

# Common source
set(COMMON_SOURCES
    src/main.c
)

if(BUILD_TYPE STREQUAL "client")

    message(STATUS "Building CLIENT")

    add_executable(client
        ${COMMON_SOURCES}
        src/client.c
    )

    target_compile_definitions(client PRIVATE BUILD_CLIENT)

elseif(BUILD_TYPE STREQUAL "server")

    message(STATUS "Building SERVER")

    add_executable(server
        ${COMMON_SOURCES}
        src/server.c
    )

    target_compile_definitions(server PRIVATE BUILD_SERVER)

else()

    message(FATAL_ERROR
        "Invalid BUILD_TYPE='${BUILD_TYPE}'. "
        "Use -DBUILD_TYPE=client or -DBUILD_TYPE=server"
    )

endif()
