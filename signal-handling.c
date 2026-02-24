#include "signal-handling.h"
#include <unistd.h>
#include <sys/socket.h>

int32_t server_sock_fd;

void get_sockfd(const int32_t sockfd) {
    server_sock_fd = sockfd;
}

void signal_handler(const int signal) {
    const char msg[48] = "Caught SIGINT, shutting down server gracefully\n";
    write(STDOUT_FILENO, msg, 48);
    shutdown(server_sock_fd, 0);
    _exit(signal);
}