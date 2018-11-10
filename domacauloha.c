#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void *vlakno1_function(void *arg);
void *vlakno2_function(void *arg);
sem_t bin_sem;
 
int number = 0;
 
int main() {
    int res;
    pthread_t vlakno1;
    pthread_t vlakno2;
   
    void *thread_result;
 
    res = sem_init(&bin_sem, 0, 0);
    if (res != 0) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    res = pthread_create(&vlakno2, NULL,vlakno2_function, NULL);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&vlakno1, NULL, vlakno1_function, NULL);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Input some number. Enter -1 to finish\n");
    while(number != -1) {
	sleep(1); 
    }

    printf("\nWaiting for thread 1 to finish...\n");
	sem_post(&bin_sem);
    res = pthread_join(vlakno1, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    printf("\nWaiting for thread 2 to finish...\n");
    res = pthread_join(vlakno2, &thread_result);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
      
    printf("Thread joined\n");
    sem_destroy(&bin_sem);
    exit(EXIT_SUCCESS);
}
 
void *vlakno2_function(void *arg){ 
       printf("vlakno 2\n\r");   
	sem_wait(&bin_sem);	

	while(number != -1){
		printf("zadane cislo je :%d\n\r", number);  
		sem_post(&bin_sem);
		sleep(1);
		sem_wait(&bin_sem);
	}
        pthread_exit(NULL);
}  
void *vlakno1_function(void *arg) {
       printf("vlakno 1\n\r");
//      sem_wait(&bin_sem);

	while(number != -1){
		printf("Zadaj cele cislo, a -1 pre ukoncenie\n\r");
		scanf("%d", &number);
		sem_post(&bin_sem);
		sleep(1);
		sem_wait(&bin_sem);
	}
        pthread_exit(NULL);
}  
