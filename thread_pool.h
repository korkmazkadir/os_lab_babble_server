#ifndef THREAD_POOL_H
#define THREAD_POOL_H

typedef struct task_t {
    void (*funtion_to_execute)(void *);
    void * parameter;
} task_t;

typedef struct task_queue_t {
    task_t **task_queue;
    int size;
    long startIndex;
    long endIndex;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
} task_queue_t;

typedef struct thread_pool_t {
    pthread_t *worker_threads;
    int size;
    char * name;
    task_queue_t *queue;
} thread_pool_t;


thread_pool_t * create_thread_pool(int pool_size, int max_waiting_task_size, char* pool_name);

void add_task_to_thread_pool(thread_pool_t * pool, void * funtion_to_execute, void * parameter);

void destroy_thread_pool(thread_pool_t * pool);


/* Task Queue Implemantation */

void task_queue_put(task_queue_t * queue, task_t * task);

task_t * task_queue_pop(task_queue_t * queue);

void destroy_task_queue(task_queue_t * queue);

#endif /* THREAD_POOL_H */

