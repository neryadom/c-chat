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

    listen(sockfd, 2);

    int32_t clientfd = accept(sockfd, 0, 0);

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
            recv(clientfd, buffer, 255, 0);
            printf("%s", buffer);
        }

        printf("Listening...");
    }

    return 0;
}
