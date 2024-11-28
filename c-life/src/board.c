#include <stdlib.h>
#include <stdio.h>
#include "board.h"

board_t *create_board(int sideLength){
    board_t *b = malloc(sizeof(board_t));
    b->boards[0] = malloc(sizeof(int)*sideLength*sideLength);
    b->boards[1] = malloc(sizeof(int)*sideLength*sideLength);
    b->sideLenght = sideLength;
    return b;
}

int num_neighbors(board_t* b, int x, int y){
    int num_neighbors = 0;
    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i == 0 && j == 0) continue;
            int yi = (y + i + b->sideLenght) % b->sideLenght;
            int xj = (x + j + b->sideLenght) % b->sideLenght;
            num_neighbors += (b->boards[0])[yi * b->sideLenght + xj];
        }
    }
    return num_neighbors;
}

int subtable_work(board_t *b, int x, int y, int width, int height){
    int alive = 0;
    int side = b->sideLenght; // just to diminish size
    for(int i = y; i < y + height; i++){
        for(int j = x; j < x + width; j++){
            int nn = num_neighbors(b,j,i);
            if((b->boards[0])[i*side + j]){
                if(nn < 2 || nn > 3) {
                    (b->boards[1])[i*side + j] = 0;
                } else {
                    (b->boards[1])[i*side + j] = 1;
                    alive++;
                }
            } else {
                if (nn == 3){
                    b->boards[1][i*side + j] = 1;
                    alive++;
                } 
                else {
                    b->boards[1][i*side + j] = 0;
                }
            }
        }
    }
    return alive;
}

void swap_boards(board_t *b){
    int* cup = b->boards[0];
    b->boards[0] = b->boards[1];
    b->boards[1] = cup;
}

void iterate_sequentially(board_t *b){
    subtable_work(b,0,0,b->sideLenght,b->sideLenght);
    swap_boards(b);
}

void destroy_board(board_t *b){
    free(b->boards[0]);
    free(b->boards[1]);
}