#include "ch.h"
#include <stdlib.h>

ch_t* create_ch(int buffer_size){
    ch_t* ch = malloc(sizeof(ch_t));
    ch->buffer = malloc(sizeof(ch_message)*buffer_size);
    sem_init(&ch->mutex_in,0,1);
    sem_init(&ch->mutex_out,0,1);
    sem_init(&ch->cons,0,0);
    sem_init(&ch->prod,0,buffer_size);
    ch->in = 0;
    ch->out = 0;
    ch->buf_size = buffer_size;
    return ch;
}

void destroy_ch(ch_t *ch){
    sem_destroy(&ch->cons);
    sem_destroy(&ch->prod);
    sem_destroy(&ch->mutex_out);
    free(ch->buffer);
    free(ch);
}

void insert(ch_t *ch, ch_message sa){
    sem_wait(&ch->prod);
    sem_wait(&ch->mutex_in);
    int k = ch->in;
    ch->in = (ch->in + 1)%ch->buf_size;
    sem_post(&ch->mutex_in);
    ch->buffer[k] = sa;
    sem_post(&ch->cons);
}

ch_message take_out(ch_t *ch) {
    sem_wait(&ch->cons);
    sem_wait(&ch->mutex_out);
    int k = ch->out;
    ch->out = (ch->out + 1)%ch->buf_size;
    sem_post(&ch->mutex_out);
    ch_message sa = ch->buffer[k];
    sem_post(&ch->cons);
}
