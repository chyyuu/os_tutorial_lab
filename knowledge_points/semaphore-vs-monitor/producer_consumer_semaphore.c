#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFF_SIZE  3
#define PRODUCE_THREAD_SIZE 5
int g_buff[BUFF_SIZE];
int g_write_index = 0;
int g_read_index = 0;

sem_t lock;
sem_t consume_sem, produce_sem;


void* produce(void *ptr){
    int idx = *(int*)ptr;
    printf("in produce %d %d %d\n",idx, g_write_index, g_read_index);
    while(1){
        sem_wait(&produce_sem); // 限制了生产者并发的数目
        sem_wait(&lock); // 对临界区的访问要加锁
        g_buff[g_write_index] = idx;
        g_write_index = (g_write_index + 1) % BUFF_SIZE;
        sem_post(&lock);

        sem_post(&consume_sem);
    }
    return NULL;
}

void* consume(void *ptr){
    while(1){
        sem_wait(&consume_sem);
        sem_wait(&lock);
        int data = g_buff[g_read_index];
        g_buff[g_read_index] = -1;
        g_read_index = (g_read_index + 1) % BUFF_SIZE;
        printf("consume %d %d %d\n", data, g_read_index, g_write_index);
        sem_post(&lock);
        sem_post(&produce_sem);
    }
    return NULL;
}

int main(int argc, char * argv[]){
    pthread_t con;
    pthread_t pros[PRODUCE_THREAD_SIZE];
    sem_init(&lock, 0, 1);
    sem_init(&consume_sem,0, 0);
    sem_init(&produce_sem,0, BUFF_SIZE);

    pthread_create(&con, NULL, consume, NULL);
    int thread_args[PRODUCE_THREAD_SIZE];
    for(int i = 0; i < PRODUCE_THREAD_SIZE; i++){
        thread_args[i] = i + 1;
        pthread_create(&pros[i], NULL, produce, (thread_args + i));
    }

    pthread_join(con,0);
    for(int i = 0; i < PRODUCE_THREAD_SIZE; i++)
        pthread_join(pros[i],0);

    sem_destroy(&lock);
    sem_destroy(&consume_sem);
    sem_destroy(&produce_sem);

    return 0;
}
