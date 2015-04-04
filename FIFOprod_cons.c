#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define PROD 2
#define CONS 2

//Exo3
typedef struct file FIFO;
struct file {
    int entier;
    FIFO *next;
    int readed;
};

FIFO *initial_fifo(int num) {
    FIFO *fifo = (FIFO *)malloc(sizeof(FIFO));
    fifo->entier = num;
    fifo->readed = 0;
    fifo->next = NULL;
    return fifo;
}

void add_file(FIFO **head, int num) {
    FIFO *fifo = initial_fifo(num);
    fifo->next = *head;
    *head = fifo;
}

void remove_fifo(FIFO *fifo, FIFO *remove) {
    FIFO *current = fifo;
    FIFO *front = current;
    while (current->next != NULL) {
        if (current == remove) {
            front->next = current->next;
            current->next = NULL;
            if (front == current) {
                front = NULL;
            }
            break;
        }
        front = current;
        current = current->next;
    }
}

FIFO *getElement(FIFO *fifo, int i) {
    if (i < 0) {
        return NULL;
    }
    if (i == 0 || fifo == NULL) {
        return fifo;
    }
    return getElement(fifo->next, i-1);
}

int is_fifo_empty(FIFO *fifo) {
    return fifo == NULL;
}

int sizeof_fifo(FIFO *fifo) {
    if (fifo == NULL) {
        return 0;
    }
    
    if (fifo->next == NULL) {
        return 1;
    }
    
    return 1 + sizeof_fifo(fifo->next);
}

void print_fifo(FIFO *head) {
    if (head == NULL) {
        printf("[]\n");
        return;
    }
    
    if (head->next == NULL) {
        printf("%d, \n", head->entier);
        return;
    }
    
    printf("%d, ", head->entier);
    print_fifo(head->next);
}

/* Thread */
pthread_mutex_t mtx_prod, mtx_cons;
FIFO *global_fifo = NULL;
int flag_cons = 10;
int flag_prod = 10;

void *factory(void *fact) {
    while (1) {
        pthread_mutex_lock(&mtx_cons);
        add_file(&global_fifo, rand()%20);
        flag_prod--;
        if (flag_prod <= 0) {
            pthread_mutex_unlock(&mtx_cons);
            break;
        }
        pthread_mutex_unlock(&mtx_cons);
    }
    return NULL;
}

void *market(void *ptr) {
    while (1) {
        int i = 0;
        pthread_mutex_lock(&mtx_cons);
        i = (rand()%(sizeof_fifo(global_fifo) - 1));
        printf("i = %d, size : %d \n", i, sizeof_fifo(global_fifo));
        
        /*while (global_fifo == NULL) {
            printf("Restore me!");
        }*/
        printf("Valeur lue : [%d]\n", getElement(global_fifo, i)->entier);
        getElement(global_fifo, i)->readed++;
        if (getElement(global_fifo, i)->readed == CONS) {
            printf("Valeur consommee %d\n", getElement(global_fifo, i)->entier);
            remove_fifo(global_fifo, getElement(global_fifo, i));
            flag_cons--;
        }
        if (flag_cons <= 0) {
            pthread_mutex_unlock(&mtx_cons);
            break;
        }
        pthread_mutex_unlock(&mtx_cons);
        
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&mtx_prod, NULL);
    pthread_mutex_init(&mtx_cons, NULL);
    
    pthread_t prod[PROD], cons[CONS];
    //print_fifo(global_fifo);
    int i;
    for (i = 0; i < PROD; i++) {
        pthread_create(&prod[i], NULL, factory, NULL);
    }
    
    for (i = 0; i < CONS; i++) {
        pthread_create(&cons[i], NULL, market, NULL);
    }
    
    for (i = 0; i < PROD; i++) {
        pthread_join(prod[i], NULL);
    }
    
    for (i = 0; i < CONS; i++) {
        pthread_join(cons[i], NULL);
    }
    
    print_fifo(global_fifo);
    
    /*FIFO *f1 = initial_fifo(0);
    int i;
    for (i = 1; i <= 10; i++) {
        add_file(&f1, i);
    }
    print_fifo(f1);
    print_fifo(getElement(f1, 3));
    remove_fifo(f1, getElement(f1,1));
    print_fifo(f1->next);
    */
    return 0;
}




//Exo2
/*
pthread_mutex_t verrou1;
pthread_mutex_t verrou2;
int flag1 = 5;
int flag2 = 5;

void *process2() {
    
    while (flag1 != 0) {
        pthread_mutex_lock(&verrou1);
        printf("I am pro2, I have token the verrou 1\n");
        flag1--;
    }
    
    return NULL;
}

void *process11() {
    while (flag2 != 0) {
        pthread_mutex_unlock(&verrou1);
        printf("I am pro22, I have released the verrou 1\n");
        flag2--;
        sleep(2);
    }
    return NULL;
}

void *process1() {
 pthread_mutex_lock(&verrou1);
 pthread_mutex_lock(&verrou2);
 while (flag1 > 0) {
 printf("I am pro1, I have the verrou 1 and verrou 2\n");
 flag1--;
 sleep(2);
 }
 pthread_mutex_unlock(&verrou2);
 pthread_mutex_unlock(&verrou1);
 
 return NULL;
 }
 
void *process22() {
 
    printf("I am pro22 and try to take the verrou 1 and 2, but I am bloc\n");
    pthread_mutex_lock(&verrou2);
    pthread_mutex_lock(&verrou1);
    printf("Now, I have the verrou 1 and 2\n");
    pthread_mutex_unlock(&verrou1);
    pthread_mutex_unlock(&verrou2);
    return NULL;
}


int main() {
    pthread_t pro1, pro2, pro11, pro22;
    pthread_mutex_init(&verrou1, NULL);
    pthread_mutex_init(&verrou2, NULL);
    
    pthread_create(&pro2, NULL, process2, NULL);
    pthread_create(&pro1, NULL, process11, NULL);
   // pthread_create(&pro2, NULL, process2, NULL);
    
   // pthread_create(&pro11, NULL, process11, NULL);
    
    
    pthread_join(pro2, NULL);
    //pthread_join(pro2, NULL);
    //pthread_join(pro11, NULL);
    pthread_join(pro1, NULL);
    return 0;
}
*/