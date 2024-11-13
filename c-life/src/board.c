#include <stdlib.h>
#include "board.h"

int is_power2(int side){
    if(side == 1) return 1;
    while(side > 1) {
        if(side%2) return 0;
        side /= 2;
    }
    return 1;
}

board_t *create_board(int sideLength){
    if(!is_power2(sideLength)){
        return NULL;
    }

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
            int xi = (x + i + b->sideLenght) % b->sideLenght;
            int yj = (y + j + b->sideLenght) % b->sideLenght;
            num_neighbors += b->boards[0][yj * b->sideLenght + xi];
        }
    }
    return num_neighbors;
}

int subtablework(board_t *b, int x, int y, int width, int height){
    int alive = 0;
    int side = b->sideLenght; // just to diminish size
    for(int i = y; i < y + height; i++){
        for(int j = x; j < x + width; j++){
            int nn = num_neighbors(b,i,j);
            if(b->boards[0][i*side + j]){
                //se vivo com poucos vizinhos morre
                if(nn < 2 || nn > 3) b->boards[1][i*side + j] = 0;
                else alive++;
            } else if(nn == 3){
                //se morto com 3 vizinhos vive
                b->boards[1][i*side + j] = 1;
                alive++;
            }    
        }
    }
    return alive;
}
