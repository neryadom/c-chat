#include "error-handling.h"
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int32_t error_handler_server_sockfd;

void error_get_sockfd(const int32_t sockfd) {
    error_handler_server_sockfd
    = sockfd;
}

void error_handler(int32_t ret, char* stage) {
    printf("Error: %s call error number %d\n", stage, ret);
    perror("Error message");
}
