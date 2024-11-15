#pragma once

//Definindo um board, assim como feito no programa em go

typedef struct _board_t{
    int* boards[2];
    int sideLenght;
}board_t;

int is_power2(int side);
board_t *create_board(int sideLength);
int num_neighbors(board_t* b, int x, int y);
int subtable_work(board_t *b, int x, int y, int width, int height);
void swap_boards(board_t *b);
void iterate_sequentially(board_t *b);
void destroy_board(board_t *b);