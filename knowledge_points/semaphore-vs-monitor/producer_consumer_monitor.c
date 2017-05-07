#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>


#define BUFF_SIZE  3
#define PRODUCE_THREAD_SIZE 5
int g_buff[BUFF_SIZE];
int g_write_index = 0;
int g_read_index = 0;

pthread_mutex_t lock;
pthread_cond_t consume_cond, produce_cond;


void* produce(void *ptr){
    int idx = *(int*)ptr;
    printf("in produce %d %d %d\n",idx, g_write_index, g_read_index);
    while(1){
        pthread_mutex_lock(&lock);
        while((g_write_index + 1) % BUFF_SIZE == g_read_index)
            pthread_cond_wait(&produce_cond, &lock);

        g_buff[g_write_index] = idx;
        g_write_index = (g_write_index + 1) % BUFF_SIZE;

        pthread_cond_signal(&consume_cond);
        pthread_mutex_unlock(&lock);

    }
    return NULL;
}

void* consume(void *ptr){
    while(1){
        pthread_mutex_lock(&lock);
        while(g_read_index == g_write_index)
             pthread_cond_wait(&consume_cond, &lock);

        int data = g_buff[g_read_index];
        g_buff[g_read_index] = -1;
        g_read_index = (g_read_index + 1) % BUFF_SIZE;
        printf("consume %d\n", data);

        pthread_cond_signal(&produce_cond);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char * argv[]){
    pthread_t con;
    pthread_t pros[PRODUCE_THREAD_SIZE];

    srand((unsigned)time(NULL));
    pthread_mutex_init(&lock, 0);
    pthread_cond_init(&consume_cond,0);
    pthread_cond_init(&produce_cond,0);

    pthread_create(&con, NULL, consume, NULL);
    int thread_args[PRODUCE_THREAD_SIZE];
    for(int i = 0; i < PRODUCE_THREAD_SIZE; i++){
        thread_args[i] = i + 1;
        pthread_create(&pros[i], NULL, produce, (thread_args + i));
    }

    pthread_join(con,0);
    for(int i = 0; i < PRODUCE_THREAD_SIZE; i++)
        pthread_join(pros[i],0);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&consume_cond);
    pthread_cond_destroy(&produce_cond);

    return 0;
}
