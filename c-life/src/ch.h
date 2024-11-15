#pragma once
#include <semaphore.h>

typedef struct{
    int x,y;
    int width,height;
    int ok; // just as in go, channel returns two things
}ch_message;


//several producers, several consumers
typedef struct{
    int buf_size;
    int in, out;
    ch_message *buffer;
    sem_t mutex_in;
    sem_t mutex_out;
    sem_t cons;
    sem_t prod;
}ch_t;

ch_t* create_ch(int buffer_size);
void destroy_ch(ch_t *ch);
void insert(ch_t *ch, ch_message sa);
ch_message take_out(ch_t *ch);


