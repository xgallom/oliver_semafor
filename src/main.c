#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

// Struktura so zdielanymi premennymi
typedef struct {
	sem_t sem;
	int number;
} threadArgs_t;

// Kedze sme v C mozeme dat pointer na strukturu rovno do argumentu
void *threadFunction1(threadArgs_t *args);
void *threadFunction2(threadArgs_t *args);

// Kedze vytvarame viac ako 1 vlakno spravime si na to funkciu nech nemame duplicitny kod
void createThread(pthread_t *thread, void *(*threadFunction)(threadArgs_t *), threadArgs_t *args)
{
	// A posleme argumenty!
	if(pthread_create(thread, NULL, (void *(*)(void *)) threadFunction, (void *) args)) { // != 0 je zbytocne, pretypujeme kvoli warningu
		perror("Main: Thread creation failed\n");
		exit(EXIT_FAILURE);
	}
}

void joinThread(pthread_t *thread)
{
	void *threadResult; // Zahodime

	if(pthread_join(*thread, &threadResult)) { // != 0 je zbytocne
		perror("Main: Thread join failed\n");
		exit(EXIT_FAILURE);
	}
}

int main() {
	// Premenne nemusia byt globalne a dame ich vlaknam cez void *arg

	// Pripravime
	threadArgs_t args;
	args.number = 0;

	if(sem_init(&args.sem, 0, 0)) { // != 0 je zbytocne
		perror("Main: Semaphore initialization failed");
		exit(EXIT_FAILURE);
	}

	// Vytvorime vlakna
	pthread_t thread1;
	createThread(&thread1, threadFunction1, &args);

	pthread_t thread2;
	createThread(&thread2, threadFunction2, &args);

	printf("Main: Waiting for thread 1 to finish\n");
	joinThread(&thread1);

	printf("Main: Waiting for thread 2 to finish\n");
	joinThread(&thread2);

	printf("Main: Threads joined\n");

	sem_destroy(&args.sem);

	return EXIT_SUCCESS;
}

void *threadFunction1(threadArgs_t *args)
{
	printf("Thread 1: Started\n");

	while(args->number != -1) {
		// Zaspime kym mame input
		sem_wait(&args->sem);

		// A vypiseme
		printf("Thread1: Number entered: %d\n", args->number);
	}

	pthread_exit(NULL);
}

void *threadFunction2(threadArgs_t *args)
{
	printf("Thread 2: Started\n");

	while(args->number != -1) {
		printf("Thread2: Enter an integer, and -1 to quit:\n");

		scanf("%d", &args->number);

		sem_post(&args->sem);
	}

	pthread_exit(NULL);
}

