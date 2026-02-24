#include "../../include/signal-handling.h"
#include <unistd.h>
#include <sys/socket.h>

int32_t signal_handler_server_sockfd;

void signal_get_sockfd(const int32_t sockfd) {
    signal_handler_server_sockfd = sockfd;
}

void signal_handler(const int signal) {
    write(STDOUT_FILENO, "\n\n##########\n", 12);
    const char msg[50] = "\nCaught SIGINT, shutting down server gracefully:\n";
    write(STDOUT_FILENO, msg, 49);
    write(STDOUT_FILENO, "close(server_sockfd);\n", 22);
    close(signal_handler_server_sockfd);
    write(STDOUT_FILENO, "shutdown(server_sockfd, 0);\n", 28);
    shutdown(signal_handler_server_sockfd, 0);
    write(STDOUT_FILENO, "_exit(signal);\n", 15);
    write(STDOUT_FILENO, "##########\n\n", 11);
    _exit(signal);
}