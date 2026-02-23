#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>


int main(){
    printf("hi\n");
    int32_t sockfd = socket(AF_INET, SOCK_STREAM, 0);

    const struct sockaddr_in address = {
        AF_INET,
        htons(3456),
        0
    };

    int32_t bind_ret = bind(sockfd, (struct sockaddr*) &address, sizeof(address));
    printf("bind_ret: %d\n", bind_ret);

    int32_t listen_ret = listen(sockfd, 2);
    printf("listen_ret: %d\n", listen_ret);

    int32_t clientfd = accept(sockfd, 0, 0);
    printf("clientfd: %d\n", clientfd);

    struct pollfd fds[2] = {
        {0, POLLIN, 0},
        {clientfd, POLLIN, 0}
    };

    while (1) {
        // 100ms timeout
        poll(fds, 2, 100);

        char buffer[256] = { 0 };

        if (fds[0].revents & POLLIN) {
            read(0, buffer, 255);
            send(clientfd, buffer, 255, 0);
        } else if (fds[1].revents & POLLIN) {
            if (recv(clientfd, buffer, 255, 0) == 0) {
                printf("Other party closed the socket, exiting...");
                return 0;
            };
            printf("%s", buffer);
        }
    }
    return 0;
}
