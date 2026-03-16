#pragma once

#include <stdint.h>
#include <stdatomic.h>
#include <stdio.h>
#include <pthread.h>

/* This file contains definitions and structs for the user connections.
 *
 *
*/

/* Represents a user connected to the server with attributes such as user_id, port and fd
 */
typedef struct User_T {
    uint32_t user_id;
    uint32_t port;
    int32_t fd;
} user_t;

/* Represents an array of users
 */
typedef struct User_Array_T {
    user_t* user_array_start;
    size_t array_size;
    size_t capacity;
} user_array_t;

user_t create_user(uint32_t port, int32_t fd);

int32_t add_user_to_array(user_array_t* user_array, user_t user);

// need to implement this. hashmap (clientfd -> user_id) maybe ?
int32_t delete_user_from_array(user_array_t user_array, user_t user);