#include "../../include/client.h"

pthread_mutex_t user_array_lock;

atomic_uint_least32_t user_id_pool = 0;

user_t create_user(uint32_t port, int32_t fd) {
    user_t user = {++user_id_pool, port, fd};
    printf("\nCreated user id=%d port=%d fd=%d\n", user.user_id, user.port, user.fd);
    return user;
}

int32_t add_user_to_array(user_array_t* user_array, const user_t user) {
    pthread_mutex_lock(&user_array_lock);
    const size_t arr_size = user_array->array_size;
    if (arr_size >= user_array->capacity) return 1;
    user_array->user_array_start[arr_size] = user;
    user_array->array_size++;
    pthread_mutex_unlock(&user_array_lock);
    printf("Added user id=%d port=%d fd=%d TO user array size=%lu capacity=%lu\n\n", user.user_id, user.port, user.fd, user_array->array_size, user_array->capacity);
    return 0;
}

