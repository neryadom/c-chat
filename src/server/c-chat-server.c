#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <signal.h>
#include <stdatomic.h>
#include <string.h>

#include "../../include/signal-handling.h"
#include "../../include/error-handling.h"
#include "../../include/messages.h"
#include "../../include/config_load.h"

/* server socket file descriptor that clients talk to*/
atomic_int_least32_t main_server_sockfd;

parsed_config_t main_config_create() {
    parsed_config_t server_config;
    memset(server_config.ip, 0, sizeof(server_config.ip));
    server_config.ip_int = 0;
    server_config.ip_size = 0;
    server_config.port = 0;
    server_config.buffer_size = 0;
    server_config.client_limit = 0;
    server_config.idle_timeout = 0;
    return server_config;
}

int32_t main_config_populate(int argc, char** argv, parsed_config_t* server_config) {
    const char* filepath;
    if (argc <= 1) filepath = "/home/nedaleko/c-chat/server.config";
    else filepath = argv[1];
    printf("Loading config in path -> %s\n", filepath);
    const int32_t ret = load_config(filepath, server_config);
    return ret;
}

int32_t main_server_startup(parsed_config_t server_config) {
    printf("\n\n=============   Starting server   =============\n\n");
    int32_t sockfd_ret = socket(AF_INET, SOCK_STREAM, 0);
    main_server_sockfd = sockfd_ret;

    /* signal handling: passing sockfd number to signal handling layer */
    signal_get_sockfd(main_server_sockfd);
    /* signal handling: registering signal handler to ensure proper shutdown in case of sigint */
    signal(SIGINT, signal_handler);

    if (sockfd_ret < 0) error_handler(errno, "socket");
    printf("sockfd: %d  <- Success!\n", sockfd_ret);

    /* this seems to definitely prevent bind() call error code 98 upon quick restart of server*/
    int32_t sockopt_flag = 1;
    setsockopt(main_server_sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt_flag, sizeof(sockopt_flag));

    const struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(server_config.port),
        .sin_addr.s_addr = server_config.ip_int
    };

    int32_t bind_ret = bind(main_server_sockfd, (struct sockaddr*) &address, sizeof(address));
    if (bind_ret < 0) error_handler(errno, "bind");
    printf("bind_ret: %d  <- Success!\n", bind_ret);

    int32_t listen_ret = listen(main_server_sockfd, 2);
    if (listen_ret < 0) error_handler(errno, "listen");
    printf("listen_ret: %d  <- Success!\n", listen_ret);

    int32_t success_ret = sockfd_ret | bind_ret | listen_ret;
    printf("(sockfd_ret) %d | (bind_ret) %d | (listen_ret) %d = %d", sockfd_ret, bind_ret, listen_ret, success_ret);
    if (success_ret >= 0) {
        printf(" <- Success!\n");
        printf("Server running on ip address %s and on port %d\n", server_config.ip, server_config.port);
    } else {
        printf(" <- Failure!\n");
    }
    return success_ret;
}

int main(int argc, char** argv){

    parsed_config_t server_config = main_config_create();

    int32_t config_built_ret = main_config_populate(argc, argv, &server_config);

    if (config_built_ret < 0) {
        printf("Config did not get parsed properly, terminating, see startup notes on stdout\n");
        return 1;
    }

    int32_t startup_ret = main_server_startup(server_config);

    if (startup_ret < 0) {
        printf("Server did not set up properly, terminating, see startup notes on stdout\n");
        return 1;
    }
    printf("\n\n=============   Server running   =============\n\n");

    while (true) {
        int32_t clientfd = accept(main_server_sockfd, nullptr, nullptr);
        if (clientfd < 0) {
            error_handler(errno, "accept");
            return 1;
        }
        printf("clientfd: %d  <- Client connected - Success!\n", clientfd);

        send(clientfd, messages_clientside_client_connected, strlen(messages_clientside_client_connected), 0);
        send(clientfd, message_clientside_welcome_tips, strlen(message_clientside_welcome_tips), 0);

        struct pollfd fds[2] = {
            { .fd = 0 /* this is stdin */,
                .events = POLLIN /* requested events */,
                .revents = 0 /* returned events */},
            { .fd = clientfd /* this is the client terminal */,
                .events = POLLIN /* requested events */,
                .revents = 0 /* returned events */},
        };

        // running loop to recv and send
        while (1) {
            poll(fds, 2, 100 /* in ms */);
            uint32_t buf_size = server_config.buffer_size;
            char buffer[buf_size];
            memset(buffer, 0, buf_size);

            if (fds[0].revents & POLLIN) {
                size_t read_size = read(0, buffer, buf_size - 1);
                size_t send_size = send(clientfd, buffer, read_size, 0);
                printf("Sending message of read size %lu, send size %lu! (should match up)\n", read_size, send_size);
            } else if (fds[1].revents & POLLIN) {
                size_t recv_size = recv(clientfd, buffer, buf_size -1, 0);
                if (recv_size == 0) {
                    printf("Other party closed the socket, exiting...\n");
                    // shutdown(sockfd, SHUT_RDWR);
                    // close(sockfd);
                    // return 0;
                    break;
                }
                printf("Received message of recv size %lu\n", recv_size);
                printf("Client %d: %s", clientfd, buffer);
            }
        }
    }
}
