#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include "wg.h"

void init_wait_group(wait_group *wg){
    wg->barrier_count = 0;
    wg->some_waiting = 0;
    pthread_mutex_init(&wg->mut, NULL);
    pthread_cond_init(&wg->wait,NULL);
}

void wait_group_add(wait_group *wg, int i){
    pthread_mutex_lock(&wg->mut);
    wg->barrier_count+=i;
    if(wg->barrier_count < 0){
        fprintf(stderr,"wg below 0!\n");
        exit(-1);
    }
    pthread_mutex_unlock(&wg->mut);
}

void wait_group_wait(wait_group *wg){
    pthread_mutex_lock(&wg->mut);
    if(wg->barrier_count != 0){
        // printf("main: waiting for other threads: %d\n",wg->barrier_count);
        wg->some_waiting = 1;
        pthread_cond_wait(&wg->wait,&wg->mut);
        wg->some_waiting = 0;
    }
    pthread_mutex_unlock(&wg->mut);
}

void wait_group_done(wait_group* wg){
    pthread_mutex_lock(&wg->mut);
    if(wg->barrier_count <= 0){
        fprintf(stderr,"decreasing wg below 0!\n");
        exit(-1);
    }
    wg->barrier_count--;
    if(wg->barrier_count == 0 && wg->some_waiting){
        pthread_cond_broadcast(&wg->wait);
    }
    pthread_mutex_unlock(&wg->mut);
}

void destroy_wait_group(wait_group* wg){
    pthread_mutex_destroy(&wg->mut);
    pthread_cond_destroy(&wg->wait);
}

void init_wait_group_sema(wait_group_sema *wg){
    wg->barrier_count = 0;
    wg->some_waiting = 0;
    sem_init(&wg->mut, 0, 1);
    sem_init(&wg->wait, 0, 0);

}


void wait_group_add_sema(wait_group_sema *wg, int i){
    sem_wait(&wg->mut);
    wg->barrier_count+=i;
    if(wg->barrier_count < 0){
        fprintf(stderr,"wg below 0!\n");
        exit(-1);
    }
    sem_post(&wg->mut);
}


void wait_group_wait_sema(wait_group_sema *wg){
    sem_wait(&wg->mut);
    if(wg->barrier_count != 0){
        // printf("main: waiting for other threads: %d\n",wg->barrier_count);
        wg->some_waiting = 1;
        sem_post(&wg->mut);
        sem_wait(&wg->wait);
        wg->some_waiting = 0;
        return;
    }
    sem_post(&wg->mut);
}


void wait_group_done_sema(wait_group_sema *wg){
    sem_wait(&wg->mut);
    if(wg->barrier_count <= 0){
        fprintf(stderr,"decreasing wg below 0!\n");
        exit(-1);
    }
    wg->barrier_count--;
    if(wg->barrier_count == 0 && wg->some_waiting){
        sem_post(&wg->wait);
    }
    sem_post(&wg->mut);
}

void destroy_wait_group_sema(wait_group_sema* wg){
    sem_destroy(&wg->mut);
    sem_destroy(&wg->wait);
}