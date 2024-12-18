#pragma once
#include <semaphore.h>
#include <pthread.h>

typedef struct {
    int barrier_count;
    int some_waiting; //starts at 0
    pthread_mutex_t mut;
    pthread_cond_t wait;
}wait_group;


void init_wait_group(wait_group *wg);
void wait_group_add(wait_group *wg, int i);
void wait_group_wait(wait_group *wg);
void wait_group_done(wait_group* wg);
void destroy_wait_group(wait_group *wg);



typedef struct {
    int barrier_count;
    int some_waiting; //starts at 0
    sem_t mut;
    sem_t wait;
}wait_group_sema;


void init_wait_group_sema(wait_group_sema *wg);
void wait_group_add_sema(wait_group_sema *wg, int i);
void wait_group_wait_sema(wait_group_sema *wg);
void wait_group_done_sema(wait_group_sema *wg);
void destroy_wait_group_sema(wait_group_sema *wg);

