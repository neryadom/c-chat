#pragma once

#include <stdint.h>

/* GET SOCKET FD
 * Gets server sockfd for use in the signal handler.
 */
void get_sockfd(int32_t sockfd);

/* SIGINT HANDLER
 * Ensures the server is properly shutdown upon a sigint signal.
 */
void signal_handler(int signal);
