#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "board.h"

void print_boards(board_t *b){
    // printf("board 0:\n");
    for(int i = 0; i < b->sideLenght; i++){
        for(int j = 0; j < b->sideLenght; j++){
            printf("%d",b->boards[0][i*b->sideLenght + j]);
        }
        printf("\n");
    }
}


int main(void){
    int sideLength = 1 << 7;
    int iterations = 1 << 10;
    int nWorkers = 16;
    double p = 0.5;
    board_t* b = create_board(sideLength);
    int alive = 0;

    printf("Board Size: %d, Iterations: %d, nWorkers: %d\n",sideLength,iterations,nWorkers);

    //seeds random!
    srand(1);
    //lets populate randomly manually
    for(int i = 0; i < sideLength*sideLength; i++){
        b->boards[0][i] =  ((((double) rand())/RAND_MAX) <= p);
        alive += b->boards[0][i];
    }

    struct timeval tv1, tv2;
    gettimeofday(&tv1,NULL);
    for(int i = 0; i < iterations; i++){
        iterate_sequentially(b);
    }
    gettimeofday(&tv2,NULL);

    printf ("Sequential program elapsed: %lfs\n",
        (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
        (double) (tv2.tv_sec - tv1.tv_sec));

    return 0;
}