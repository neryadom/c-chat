cmake_minimum_required(VERSION 3.8)

project(c-chat C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(c-chat
    c-chat-server.c
    signal-handling.c
    error-handling.c
)


