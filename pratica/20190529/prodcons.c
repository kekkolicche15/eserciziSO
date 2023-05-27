#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/eventfd.h>
#include <unistd.h>

int val;
int empty, full;

void *producer(void *s){
    eventfd_t res;
    while(1){
        eventfd_read(full, &res);
        val = rand();
        printf("PRODUTTORE  --> %d\n", val);
        eventfd_write(empty, 1);
        sleep(rand()%5);
    }
}

void *consumer(void *s){
    eventfd_t res;
    while(1){
        eventfd_read(empty, &res);
        printf("CONSUMATORE --> %d\n", val);
        eventfd_write(full, res);
        sleep(rand()%5);
    }
}

int main(){
    srand(time(NULL));
    empty = eventfd(0, EFD_SEMAPHORE);
    full = eventfd(1, EFD_SEMAPHORE);
    pthread_t prod, cons;
    if(pthread_create(&prod, NULL, producer, NULL))
        perror("Error while creating prod thread\n");
    if(pthread_create(&cons, NULL, consumer, NULL))
        perror("Error while creating cons thread\n");
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);
    exit(EXIT_SUCCESS);
}