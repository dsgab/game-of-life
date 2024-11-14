#include "pool.h"
#include <stdlib.h>

//creates a thread pool for the board
pool_board* create_pool(int num_threads, board_t* b){
    pool_board* pb = malloc(sizeof(pool_board));
    pb->b = b;
    pb->tids = malloc(sizeof(pthread_t) * num_threads);
    return pb;
}