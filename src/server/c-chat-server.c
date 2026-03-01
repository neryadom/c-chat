#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <signal.h>
#include <stdatomic.h>

#include "../../include/signal-handling.h"
#include "../../include/error-handling.h"
#include "../../include/messages.h"

/* server socket file descriptor that clients talk to*/
atomic_int_least32_t main_server_sockfd;

int32_t main_server_startup() {
    printf("=============   Starting server   =============\n\n");
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
        AF_INET,
        htons(3456),
        0
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
    } else {
        printf(" <- Failure!\n");
    }
    return success_ret;
}

int main(){
    int32_t startup_ret = main_server_startup();
    if (startup_ret < 0) {
        printf("Server did not set up properly, terminating, see startup notes on stdout\n");
        return 1;
    }
    printf("\n\n=============   Server running   =============\n\n");

    while (true) {
        int32_t clientfd = accept(main_server_sockfd, 0, 0);
        if (clientfd < 0) {
            error_handler(errno, "accept");
            return 1;
        }
        printf("clientfd: %d  <- Client connected - Success!\n", clientfd);

        send(clientfd, messages_clientside_client_connected, 37, 0);
        send(clientfd, message_clientside_welcome_tips, 200, 0);

        struct pollfd fds[2] = {
            { /* this is stdin */ 0, POLLIN, 0},
            {clientfd, POLLIN, 0}
        };

        // running loop to recv and send
        while (1) {
            poll(fds, 2, 100 /* in ms */);

            char buffer[256] = { 0 };

            if (fds[0].revents & POLLIN) {
                read(0, buffer, 255);
                send(clientfd, buffer, 255, 0);
            } else if (fds[1].revents & POLLIN) {
                if (recv(clientfd, buffer, 255, 0) == 0) {
                    printf("Other party closed the socket, exiting...\n");
                    // shutdown(sockfd, SHUT_RDWR);
                    // close(sockfd);
                    // return 0;
                    break;
                };
                printf("Client: %s", buffer);
            }
        }
    }
}
