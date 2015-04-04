#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define N 15
#define C 5

int counter = 0;
int semaphore_place = 0;

pthread_mutex_t mtx_geton_passenger, mtx_getdown_passenger;
pthread_cond_t cond_passenger, cond_bus, cond_stay;

void *passengerAction(void *ptr){
    int *id = (int *)ptr;
    while (1) {
        pthread_mutex_lock(&mtx_geton_passenger);
        printf("[%d] is waitting!\n", *id);
        pthread_cond_wait(&cond_bus, &mtx_geton_passenger);
        
        if (semaphore_place == 0 && counter < C) {
            semaphore_place = 1;
            printf("[%d] gets on the bus!\n", *id);
            counter++;
        }
        
        if (counter == C) {
            printf("Driver, we can go now!\n");
            pthread_cond_signal(&cond_passenger);
        } else {
            semaphore_place = 0;
            pthread_cond_signal(&cond_bus);
        }
        pthread_mutex_unlock(&mtx_geton_passenger);
        
        
        while (1) {
            pthread_mutex_lock(&mtx_getdown_passenger);
            printf("[%d] is waitting for destination to get down the bus!\n", *id);
            pthread_cond_wait(&cond_stay, &mtx_getdown_passenger);
            
            if (semaphore_place == 1 && counter > 0) {
                semaphore_place = 0;
                printf("[%d] gets down the bus!\n", *id);
                counter--;
            }
            
            if (counter == 0) {
                printf("Driver, we have already left!\n");
                pthread_cond_signal(&cond_passenger);
            } else {
                semaphore_place = 1;
                pthread_cond_signal(&cond_stay);
            }
            pthread_mutex_unlock(&mtx_getdown_passenger);
            break;
        }
        return NULL;
    }
	return NULL;
}

void *busAction(void *bptr){
    while (1) {
        pthread_mutex_lock(&mtx_geton_passenger);
        printf("I am waitting for passenger to get on the bus!\n");
        pthread_cond_signal(&cond_bus);
        pthread_cond_wait(&cond_passenger, &mtx_geton_passenger);
        
        printf("I am driving!\n");
        sleep(2);
        
        
        printf("We have arrived! All passengers can get down the bus\n");
        pthread_mutex_lock(&mtx_getdown_passenger);
        printf("I am waitting for passenger to get down the bus\n");
        pthread_cond_signal(&cond_stay);
        pthread_cond_wait(&cond_passenger, &mtx_getdown_passenger);
        
        printf("Now the others can get on the bus!\n");
        pthread_mutex_unlock(&mtx_getdown_passenger);
        pthread_mutex_unlock(&mtx_geton_passenger);
    }
	return NULL;
}



int main(){
    int i;
    pthread_t passenger[N], bus;
    pthread_mutex_init(&mtx_getdown_passenger, NULL);
    pthread_mutex_init(&mtx_geton_passenger, NULL);
    pthread_cond_init(&cond_passenger, NULL);
    pthread_cond_init(&cond_bus, NULL);
    pthread_cond_init(&cond_stay, NULL);
    int *id = malloc(N*sizeof(int));
    
    for (i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&passenger[i], NULL, passengerAction, (void *)&id[i]);
    }
	pthread_create(&bus, NULL, busAction, NULL);
    pthread_join(bus, NULL);
    for (i = 0; i < N; i++) {
        pthread_join(passenger[i], NULL);
    }
    
	return 0;
}
