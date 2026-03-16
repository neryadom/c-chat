#pragma once
#include <stdatomic.h>
#include <unistd.h>
#include <sys/socket.h>

/* GET SOCKET FD
 * Gets server sockfd for use in the signal handler.
 */
void signal_get_sockfd(atomic_int_least32_t sockfd);

/* SIGINT HANDLER
 * Ensures the server is properly shutdown upon a sigint signal.
 */
void signal_handler(int signal);
