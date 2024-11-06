#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// the table revolves, left cells are neighbors to right cells
int num_neighbors(int n, int i, int j, int* table){
    int num_neighbors = 0;
    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            if (x == 0 && y == 0) continue;
            int row = ((j + y) + n)%n;
            int col = ((i + x) + n)%n;
            if(table[n*row + col]) num_neighbors++;
        }
    }
    return num_neighbors;
}

void iterate(int n, int* first_table, int* second_table){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int nn = num_neighbors(n,i,j,first_table);
            if(first_table[n*i + j] && (nn < 2 || n > 3)) second_table[n*i + j] = 0;
            if(!first_table[n*i + j] && nn == 3) second_table[n*i + j] = 1;
        }
    }
}

int main(int argc, char* argv[]){
    int N = 1<<10;
    int iterations = 1<<30;

    //allocate a contiguous piece of memory 64x64
    int *tables[2];
    for(int i = 0; i < 2; i++){
        tables[i] = malloc(N*N*sizeof(int));
    }

    //randomly populate table with probability p
    double p = 0.3;
    srand(time(NULL));
    for(int i = 0; i < N*N; i++){
        tables[0][i] = (((double) rand() / RAND_MAX) <= p);
    }

    for(int i = 0; i < iterations; i++){
        iterate(N, tables[i%2], tables[(i+1)%2]);
    }

    for(int i = 0; i < 2; i++){
        free(tables[i]);
    }

    return 0;
}