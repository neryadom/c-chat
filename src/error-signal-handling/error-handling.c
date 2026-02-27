#include "../../include/error-handling.h"
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>


void error_handler(int32_t ret, char* stage) {
    printf("Error: %s call error number %d\n", stage, ret);
    perror("Error message");
}
