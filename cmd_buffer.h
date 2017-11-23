
#ifndef CMD_BUFFER_H
#define CMD_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "babble_types.h"

    typedef struct buffer {
        command_t **cmdArray;
        int size;

        pthread_mutex_t mutex;
        pthread_cond_t cond;

        long startIndex;
        long endIndex;
    } cmd_buffer_t;


    cmd_buffer_t * createBuffer(int size);

    void put(cmd_buffer_t *buffer, command_t *cmd);

    command_t * pop(cmd_buffer_t *buffer);

    void destroyBuffer(cmd_buffer_t *buffer);

#ifdef __cplusplus
}
#endif

#endif /* CMD_BUFFER_H */

