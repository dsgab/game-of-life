#include <iostream>
#include <cstdlib>
#include <time.h>


__global__
void calculate_neighbors(int sideLength, int *b, int *nb){
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    for(int i = index; i < sideLength*sideLength; i+=stride){
        nb[i] = 0;

        int y = i/sideLength;
        int x = i%sideLength;

        for(int dy = -1; dy <= 1; dy++){
            int ny = (y + dy + sideLength)%sideLength;

            for(int dx = -1; dx <= 1; dx++){
                if(!dx && !dy) continue;
                int nx = (x + dx + sideLength)%sideLength;

                nb[i] += b[ny*sideLength + nx];
            }
        }
    }
}

__global__
void iterate_boards(int sideLength, int *b1, int *b2, int *nb){
    //determine stride
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;

    for(int i = index; i < sideLength*sideLength; i+=stride){
        if(b1[i]){
            if(nb[i] < 2 || nb[i] > 3){
                b2[i] = 0;
            } else b2[i] = 1;
        } else {
            if(nb[i] == 3){
                b2[i] = 1;
            } else {
                b2[i] = 0;
            }
        }
    }
}

void swap(int* &x, int* &y){
    int* cup = x;
    x = y;
    y = cup;
}

int main(int argc, char* argv[]){
    int sideLength = 1<<10;
    int iterations = 1<<9;
    double p = 0.5;

    //allocate unified memory -- accessible by both CPU and GPU
    int *board_1, *board_2, *neighbors_board;
    cudaMallocManaged(&board_1, sideLength*sideLength);
    cudaMallocManaged(&board_2, sideLength*sideLength);
    cudaMallocManaged(&neighbors_board, sideLength*sideLength);


    //initialize random board
    srand(1);
    for(int i = 0; i < sideLength; i++){
        for(int j = 0; j < sideLength; j++){
            board_1[i* sideLength + j] = ((((double) rand())/RAND_MAX) <= p);

        }
    }

    int blockSize = 256; //blockSize must be a power of 2
    int numBlocks = (sideLength*sideLength + blockSize - 1) / blockSize; //first let's separate cell by cell.


    for(int i = 0; i < iterations; i++){
        calculate_neighbors<<<numBlocks,blockSize>>>(sideLength,board_1,neighbors_board);
        cudaDeviceSynchronize();
        iterate_boards<<<numBlocks,blockSize>>>(sideLength,board_1,board_2,neighbors_board);
        cudaDeviceSynchronize();
        swap(board_1,board_2);
    }

    //free allocated memory
    cudaFree(board_1);
    cudaFree(board_2);
    cudaFree(neighbors_board);

    return 0;
}