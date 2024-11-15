#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "board.h"
#include "pool.h"

void print_boards(board_t *b){
    // printf("board 0:\n");
    for(int i = 0; i < b->sideLenght; i++){
        for(int j = 0; j < b->sideLenght; j++){
            if(b->boards[0][i*b->sideLenght + j]) printf("x");
            else printf("_");
        }
        printf("\n");
    }
}

int check_if_boards_different(board_t* b1, board_t* b2){
    if(b1->sideLenght != b2->sideLenght){
        return 1;
    }

    int d = 0;
    for(int i = 0; i < b1->sideLenght*b1->sideLenght; i++){
        d += (b1->boards[0][i] != b2->boards[0][i]);
    }
    return d;
}


int main(void){
    int sideLength = 1<<10;
    int iterations = 1<<9;
    int nWorkers = 8;
    double p = 0.5;

    board_t* b1 = create_board(sideLength);
    board_t* b2 = create_board(sideLength);

    int alive = 0;

    printf("Board Size: %d, Iterations: %d, nWorkers: %d\n",sideLength,iterations,nWorkers);

    //seeds random!
    srand(time(NULL));
    //lets populate randomly manually
    for(int i = 0; i < sideLength*sideLength; i++){
        b1->boards[0][i] =  ((((double) rand())/RAND_MAX) <= p);
        b2->boards[0][i] = b1->boards[0][i];
        alive += b1->boards[0][i];
    }

    printf("diffs ? : %d\n",check_if_boards_different(b1,b2));
    
    struct timeval tv1, tv2;
        

    //concurrent application!
    gettimeofday(&tv1,NULL);
    pool_board* pb = create_pool(nWorkers,b2);
    start_pool(pb);

    for(int i = 0; i < iterations; i++){
        iterate_concurrently(pb);
    }    
    liberate_threads(pb);
    gettimeofday(&tv2,NULL);
    printf ("Concurrent program elapsed: %lfs\n",
        (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
        (double) (tv2.tv_sec - tv1.tv_sec));
    
    // sequential application!
    gettimeofday(&tv1,NULL);
    for(int i = 0; i < iterations; i++){
        iterate_sequentially(b1);
    }
    gettimeofday(&tv2,NULL);

    printf ("Sequential program elapsed: %lfs\n",
        (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 +
        (double) (tv2.tv_sec - tv1.tv_sec));
    
    //freeing space
    destroy_board(b1);
    destroy_board(b2);
    destroy_pool(pb);

    return 0;
}