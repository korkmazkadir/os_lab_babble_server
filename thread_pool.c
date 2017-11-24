#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "thread_pool.h"

int THREAD_NAME_SIZE = 64;

void idle_function(thread_pool_t *pool) {
    
    task_queue_t *queue = pool->queue;
    
    while (1) {
        char *thread_name = malloc(THREAD_NAME_SIZE * sizeof(char));
        pthread_getname_np(pthread_self(),thread_name, THREAD_NAME_SIZE);
        
        printf("Waiting for task %s - %s\n", pool->name ,thread_name);
        
        task_t *task = task_queue_pop(queue);
        if (task == NULL) {
            continue;
        }

        printf("--->Assigning task %s - %s\n", pool->name ,thread_name);
        task->funtion_to_execute(task->parameter);
        printf("Task finished %s - %s\n", pool->name ,thread_name);
        
        free(thread_name);
    }
}

thread_pool_t * create_thread_pool(int pool_size, int max_waiting_task_size, char* pool_name) {
    
    printf(">> Creating pool %s with %d threads\n",pool_name, pool_size);
    
    thread_pool_t * pool = malloc(sizeof (thread_pool_t));

    pool->worker_threads = malloc(pool_size * sizeof (pthread_t));
    pool->size = pool_size;
    pool->name = pool_name;
    
    pool->queue = malloc(sizeof (task_queue_t));
    pool->queue->task_queue = malloc( max_waiting_task_size * sizeof ( task_t * ) );
    pool->queue->size = max_waiting_task_size;
    pool->queue->startIndex = 0;
    pool->queue->endIndex = 0;

    pthread_mutex_init(&(pool->queue->mutex), NULL);
    pthread_cond_init(&(pool->queue->cond), NULL);

    for (int i = 0; i < pool_size; i++) {
        pthread_create(&(pool->worker_threads[i]), NULL, (void*) idle_function, pool);
        char *thread_name = malloc(THREAD_NAME_SIZE * sizeof(char));
        sprintf(thread_name, "Thread-%d", (i+1) ); 
        pthread_setname_np(pool->worker_threads[i], thread_name );
    }

    return pool;
}

void add_task_to_thread_pool(thread_pool_t * pool, void * funtion_to_execute, void * parameter) {
    task_t * task = malloc(sizeof (task_t));
    task->funtion_to_execute = funtion_to_execute;
    task->parameter = parameter;
    task_queue_put(pool->queue, task);
}

void destroy_thread_pool(thread_pool_t * pool) {
    free(pool->worker_threads);
    destroy_task_queue(pool->queue);
    free(pool);
}

/* Task Queue Implemantation */

void task_queue_put(task_queue_t * queue, task_t * task) {
    pthread_mutex_lock(&(queue->mutex));

    while (queue->endIndex - queue->startIndex > queue->size) {
        pthread_cond_wait(&(queue->cond), &(queue->mutex));
    }

    queue->task_queue[ (queue->endIndex % queue->size) ] = task;
    queue->endIndex++;

    printf("Waiting tasks : %ld \n", (queue->endIndex - queue->startIndex));

    pthread_cond_signal(&(queue->cond));
    pthread_mutex_unlock(&(queue->mutex));
}

task_t * task_queue_pop(task_queue_t * queue) {
    pthread_mutex_lock(&(queue->mutex));

    while (queue->endIndex - queue->startIndex <= 0) {
        pthread_cond_wait(&(queue->cond), &(queue->mutex));
    }

    task_t * task = queue->task_queue[ (queue->startIndex % queue->size) ];
    queue->startIndex++;

    printf("-Waiting tasks : %ld \n", (queue->endIndex - queue->startIndex));

    pthread_cond_signal(&(queue->cond));
    pthread_mutex_unlock(&(queue->mutex));

    return task;
}

void destroy_task_queue(task_queue_t * queue) {
    free(queue->task_queue);
    free(queue);
}