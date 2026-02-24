cmake_minimum_required(VERSION 3.10)

project(c-chat C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(c-chat
    src/server/c-chat-server.c
    src/error-signal-handling/signal-handling.c
    src/error-signal-handling/error-handling.c
)


