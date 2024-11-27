#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "board.h"
#include "pool.h"

void print_boards(board_t *board){
    // printf("board 0:\n");
    for(int i = 0; i < board->sideLenght; i++){
        for(int j = 0; j < board->sideLenght; j++){
            if(board->boards[0][i*board->sideLenght + j]) printf("x");
            else printf("_");
        }
        printf("\n");
    }
}

int check_if_boards_different(board_t* board1, board_t* board2){
    if(board1->sideLenght != board2->sideLenght){
        return 1;
    }

    int d = 0;
    for(int i = 0; i < board1->sideLenght*board1->sideLenght; i++){
        d += (board1->boards[0][i] != board2->boards[0][i]);
    }
    return d;
}


int main(void){
    int sideLength = 1<<10;
    int iterations = 1<<9;
    int nWorkers = 8;
    double probability = 0.5;

    board_t* board1 = create_board(sideLength);
    board_t* board2 = create_board(sideLength);

    int alive = 0;

    printf("Board Size: %d, Iterations: %d, nWorkers: %d\n",sideLength,iterations,nWorkers);

    //seeds random!
    srand(time(NULL));
    //lets populate randomly manually
    for(int i = 0; i < sideLength*sideLength; i++){
        board1->boards[0][i] =  ((((double) rand())/RAND_MAX) <= probability);
        board2->boards[0][i] = board1->boards[0][i];
        alive += board1->boards[0][i];
    }

    printf("diffs ? : %d\n",check_if_boards_different(board1,board2));
    
    struct timeval timeValue1, timeValue2;
        

    //concurrent application!
    gettimeofday(&timeValue1,NULL);
    pool_board* poolBoard = create_pool(nWorkers,board2);
    start_pool(poolBoard);

    for(int i = 0; i < iterations; i++){
        iterate_concurrently(poolBoard);
    }    
    liberate_threads(poolBoard);
    gettimeofday(&timeValue2,NULL);
    printf ("Concurrent program elapsed time: %lfs\n",
        (double) (timeValue2.tv_usec - timeValue1.tv_usec) / 1000000 +
        (double) (timeValue2.tv_sec - timeValue1.tv_sec));
    
    // sequential application!
    gettimeofday(&timeValue1,NULL);
    for(int i = 0; i < iterations; i++){
        iterate_sequentially(board1);
    }
    gettimeofday(&timeValue2,NULL);

    printf ("Sequential program elapsed time: %lfs\n",
        (double) (timeValue2.tv_usec - timeValue1.tv_usec) / 1000000 +
        (double) (timeValue2.tv_sec - timeValue1.tv_sec));
    
    //freeing space
    destroy_board(board1);
    destroy_board(board2);
    destroy_pool(poolBoard);

    return 0;
}