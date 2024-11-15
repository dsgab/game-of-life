#pragma once
#include "ch.h"
#include "board.h"
#include "wg.h"
#include <pthread.h>

typedef struct{
    board_t* b;
    ch_t *work_channel;
    int num_threads;
    pthread_t* tids;
    wait_group wg_done;
}pool_board;

pool_board* create_pool(int num_threads, board_t* b);
void start_pool(pool_board* pb);
void iterate_concurrently(pool_board* pb);
void liberate_threads(pool_board* pb);
void destroy_pool(pool_board* pb);

