#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX 5


pthread_mutex_t verrou[MAX];

void *eat(void *arg){
  int *i = (int *)arg;

  while(1){
    pthread_mutex_trylock(&verrou[*i]);
    printf("P%d : Stick G\n",*i);
    pthread_mutex_trylock(&verrou[(*i+1)%MAX]);
    printf("P%d : Stick D\n",*i);
    printf("P%d : I'm eating \n",*i);
    sleep(rand()%3);
    
    pthread_mutex_unlock(&verrou[(*i+1)%MAX]);
    pthread_mutex_unlock(&verrou[*i]);
  }
  
  return NULL;
}


int main(){

  pthread_t threads[MAX];  
 
  int *id = malloc(MAX*sizeof(int));
  int j=0;
  for(j=0; j<MAX; j++){
    pthread_mutex_init(&verrou[j], NULL);
  }
  int i=0;
  for(i=0;i<MAX;i++){
    id[i] = i;
  }
  for(i=0;i<MAX;i++){
    pthread_create(&threads[i] , NULL, eat, (void *)&id[i]);
  }

  for(i=0;i<MAX;i++){
    pthread_join(threads[i],NULL);
  }
  exit(EXIT_SUCCESS);
}
