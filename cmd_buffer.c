#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "cmd_buffer.h"

cmd_buffer_t * createBuffer(int size) {
    cmd_buffer_t *buffer = malloc(size * sizeof (cmd_buffer_t *));

    buffer->cmdArray = malloc(size * sizeof (command_t));

    buffer->size = size;

    pthread_mutex_init(&(buffer->mutex), NULL);
    pthread_cond_init(&(buffer->cond), NULL);

    buffer->startIndex = 0;
    buffer->endIndex = 0;

    return buffer;
}

void put(cmd_buffer_t *buffer, command_t *cmd) {
    pthread_mutex_lock(&(buffer->mutex));

    while (buffer->endIndex - buffer->startIndex > buffer->size) {
        pthread_cond_wait(&(buffer->cond), &(buffer->mutex));
    }

    buffer->cmdArray[ (buffer->endIndex % buffer->size) ] = cmd;
    buffer->endIndex++;

    printf("Waiting commands : %ld \n", (buffer->endIndex - buffer->startIndex));
    
    pthread_cond_broadcast(&(buffer->cond));
    pthread_mutex_unlock(&(buffer->mutex));
}

command_t * pop(cmd_buffer_t *buffer) {
    pthread_mutex_lock(&(buffer->mutex));

    while (buffer->endIndex - buffer->startIndex <= 0) {
        pthread_cond_wait(&(buffer->cond), &(buffer->mutex));
    }

    command_t * cmd = buffer->cmdArray[ (buffer->startIndex % buffer->size) ];
    buffer->startIndex++;

    printf("-Waiting commands : %ld \n", (buffer->endIndex - buffer->startIndex));

    pthread_cond_broadcast(&(buffer->cond));
    pthread_mutex_unlock(&(buffer->mutex));

    return cmd;
}

void destroyBuffer(cmd_buffer_t *buffer) {
    printf("Destroying cmd buffer\n");
    free(buffer->cmdArray);
    free(buffer);
}