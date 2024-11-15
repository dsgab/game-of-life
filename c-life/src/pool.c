#include "pool.h"
#include "wg.h"
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

//creates a thread pool for the board
pool_board* create_pool(int num_threads, board_t* b){
    pool_board* pb = malloc(sizeof(pool_board));
    pb->b = b;
    pb->num_threads = num_threads;
    pb->tids = malloc(sizeof(pthread_t) * num_threads);
    pb->work_channel = create_ch(num_threads);
    init_wait_group(&pb->wg_done);
    return pb;
}

void *thread_work(void *t_arg) {
    pool_board* pb = (pool_board*) t_arg;
    while(1) {
        ch_message msg = take_out(pb->work_channel);
        // printf("thread received message: %d %d %d %d %d\n",msg.x,msg.y,msg.width,msg.height,msg.ok);
        if(msg.ok == 0){
            // printf("received msg.ok = 0!\n");
            break;
        }
        subtable_work(pb->b,msg.x,msg.y,msg.width,msg.height);
        // printf("thread finished: %d %d %d %d!\n",msg.x,msg.y,msg.width,msg.height);
        wait_group_done(&pb->wg_done);
    }
    pthread_exit(NULL);
}

void start_pool(pool_board* pb){
    for(int i = 0; i < pb->num_threads; i++){
        // printf("creating thread %d\n",i);
        if(pthread_create(&pb->tids[i],NULL,thread_work,(void *) pb)){
            fprintf(stderr, "error: creating thread %d\n",i);
        }
    }
}

//later lets test with close memory
void iterate_concurrently(pool_board* pb){
    // printf("main adding %d to wg!\n",pb->b->sideLenght);
    wait_group_add(&pb->wg_done,pb->b->sideLenght);
    for(int i = 0; i < pb->b->sideLenght; i++){
        insert(pb->work_channel, (ch_message){0,i,pb->b->sideLenght,1,1}); //i'm asking a thread to do a single line
        // printf("main sent msg into channel! line = %d\n",i);
    }

    wait_group_wait(&pb->wg_done);
    // printf("main liberated!\n");

    swap_boards(pb->b);
}

void liberate_threads(pool_board* pb){
    for(int i = 0; i < pb->num_threads; i++){
        insert(pb->work_channel, (ch_message){0,0,0,0,0}); // ok = 0!
    }

    for(int i = 0; i < pb->num_threads; i++){
        pthread_join(pb->tids[i],NULL); //wait for all to finish!
    }
}


void destroy_pool(pool_board* pb){
    free(pb->tids);
    destroy_ch(pb->work_channel);
    destroy_wait_group(&pb->wg_done);
}
