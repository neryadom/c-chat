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

    int32_t connect_ret = connect(sockfd, (struct sockaddr*) &address, sizeof(address));
    printf("connect_ret: %d\n", connect_ret);

    struct pollfd fds[2] = {
        {0, POLLIN, 0},
        {sockfd, POLLIN, 0}
    };

    // running loop to recv and send
    while (1) {
        poll(fds, 2, 100 /* in ms */);

        char buffer[256] = { 0 };

        if (fds[0].revents & POLLIN) {
            read(0, buffer, 255);
            send(sockfd, buffer, 255, 0);
        } else if (fds[1].revents & POLLIN) {
            if (recv(sockfd, buffer, 255, 0) == 0) {
                printf("Other party closed the socket, exiting...");
                return 0;
            };
            printf("%s", buffer);
        }
    }
    return 0;
}
