#include "ch.h"
#include "board.h"
#include <pthread.h>

typedef struct{
    ch_t channel;

    board_t* b;
    
    int num_threads;
    pthread_t* tids;
}pool_board;

