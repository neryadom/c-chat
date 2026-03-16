cmake_minimum_required(VERSION 4.00)

project(c-chat C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(c-chat
    src/server/c-chat-server.c
        src/signal-handling/signal-handling.c
        src/error-handling/error-handling.c
        src/config/config_load.c
        src/user/client.c
)


